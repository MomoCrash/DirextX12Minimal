#include "RenderWindow.h"

#include "Geometrie.h"
#include "Shader.h"
#include "Transform.h"

using namespace DirectX ;

RenderWindow::RenderWindow(HINSTANCE hInstance) : Window(hInstance)
{

}

bool RenderWindow::Initialize()
{
    Window::Initialize();

    OpenCommandList();

    cam.Reset();
    cam.SetPosition(XMFLOAT3{ 0.0f, 0.0f, -5.0f });
    cam.SetRotationYPR(XMFLOAT3{ 0.0f, 0.0f, 0.0f });

    mGlobalConstantBuffer = new UploadBuffer<GlobalInformation>(mDevice, 1, true);
    mGlobalConstantBuffer->Resource()->SetName(L"PASS_BUFFER");
    
    mShader = new Shader(GetDevice(), GetBackBufferFormat(), GetDepthStencilFormat());

    return 1;
}

void RenderWindow::Update()
{
    // J'ai rajouter une matrice pour la position de la cam
    XMStoreFloat4x4(&mView, XMMatrixInverse(nullptr, cam.GetMatrix()));
    
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);

    XMMATRIX viewProj = XMMatrixMultiply(view, proj);

    GlobalInformation info;
    XMStoreFloat4x4(&info.ViewProj, XMMatrixTranspose(viewProj));

    mGlobalConstantBuffer->CopyData(0, info);
}

void RenderWindow::OnResize()
{
    Window::OnResize();
    XMMATRIX ProjMatrix = XMMatrixPerspectiveFovLH(0.25f*3.14f, AspectRatio(), 0.1f, 500);
    XMStoreFloat4x4(&mProj, ProjMatrix);
}

void RenderWindow::OpenCommandList()
{
    mDirectCmdListAlloc->Reset();
    mCommandList->Reset(mDirectCmdListAlloc, nullptr);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrier);
}

void RenderWindow::CloseCommandList()
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
    D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier);

    mCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { mCommandList };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	
    FlushCommandQueue();
}

void RenderWindow::BeginDraw()
{

    OpenCommandList();
	
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    CD3DX12_CPU_DESCRIPTOR_HANDLE currentBackBufferView(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize);
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GetDepthStencilView();

    mCommandList->ClearRenderTargetView(currentBackBufferView, DirectX::Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);


}

void RenderWindow::Draw(Shader& shader, Geometrie& geo, UploadBuffer<ObjectData>* buffer)
{

    mCommandList->SetGraphicsRootSignature(shader.mRootSignature);
    mCommandList->SetPipelineState(shader.mPSO);

    mCommandList->SetGraphicsRootConstantBufferView(0, buffer->Resource()->GetGPUVirtualAddress());
    mCommandList->SetGraphicsRootConstantBufferView(1, mGlobalConstantBuffer->Resource()->GetGPUVirtualAddress());

    D3D12_VERTEX_BUFFER_VIEW vertexBuffer = geo.VertexBufferView();
    D3D12_INDEX_BUFFER_VIEW indexBuffer = geo.IndexBufferView();

    mCommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
    mCommandList->IASetIndexBuffer(&indexBuffer);
    mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    mCommandList->DrawIndexedInstanced(geo.IndicesCount, 1, 0, 0, 0);
    
}

void RenderWindow::EndDraw()
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier);
    
    mCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { mCommandList };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	
    mSwapChain->Present(0, 0);
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
	
    FlushCommandQueue();
}
