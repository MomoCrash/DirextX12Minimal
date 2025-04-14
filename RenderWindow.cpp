#include "RenderWindow.h"

#include "Geometrie.h"
#include "Shader.h"
#include "Transform.h"

using namespace DirectX ;

RenderWindow::RenderWindow(HINSTANCE hInstance) : Window(hInstance), mView(), mProj(), mPassDataBuffer(nullptr) {}

RenderWindow::~RenderWindow() 
{
    delete mPassDataBuffer;
}

bool RenderWindow::Initialize()
{
    Window::Initialize();

    OpenCommandList();

    mPassDataBuffer = new UploadBuffer<PassData>(mDevice, 1, true);
    mPassDataBuffer->Resource()->SetName(L"PASS_BUFFER");

    return 1;
}

void RenderWindow::Update()
{

    PollWindowEvents();

    // J'ai rajouter une matrice pour la position de la cam
    XMStoreFloat4x4(&mView, XMMatrixInverse(nullptr, cam.GetMatrix()));
    
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);

    XMMATRIX viewProj = XMMatrixMultiply(view, proj);

    PassData info;
    XMStoreFloat4x4(&info.ViewProj, XMMatrixTranspose(viewProj));

    mPassDataBuffer->CopyData(0, info);
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
}

void RenderWindow::CloseCommandList()
{
    mCommandList->Close();
    ID3D12CommandList* cmdsLists[] = { mCommandList };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	
    FlushCommandQueue();
}

void RenderWindow::BeginDraw()
{

    OpenCommandList();

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrier);
	
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    CD3DX12_CPU_DESCRIPTOR_HANDLE currentBackBufferView(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize);
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GetDepthStencilView();

    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    mCommandList->ClearRenderTargetView(currentBackBufferView, DirectX::Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

}

void RenderWindow::EndDraw()
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier);

    CloseCommandList();

    mSwapChain->Present(0, 0);
    mCurrBackBuffer = 1 - mCurrBackBuffer;
}
