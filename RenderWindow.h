#pragma once

#include "d3dUtils.h"
#include "Geometrie.h"
#include "Shader.h"
#include "Transform.h"
#include "UploadBuffer.h"
#include "Window.h"

class Shader;
class Geometrie;

struct ObjectData
{
    DirectX::XMFLOAT4X4 World;
};

struct PassData
{
    DirectX::XMFLOAT4X4 ViewProj;
};

class RenderWindow : public Window
{
    
public:
    RenderWindow(HINSTANCE hInstance);
    ~RenderWindow() override;
    bool Initialize() override;
    void Update() override;
    
    void OnResize() override;

    void OpenCommandList();
    void CloseCommandList();
    
    void BeginDraw() override;

    template <class ObjectData>
    void Draw(Shader const& shader, Geometrie const& geo, UploadBuffer<ObjectData>* objectBuffer);
    void EndDraw() override;
    
    TRANSFORM cam;
    
    DirectX::XMFLOAT4X4 mView;
    DirectX::XMFLOAT4X4 mProj;
    UploadBuffer<PassData>* mPassDataBuffer;
};

template <class ObjectData>
void RenderWindow::Draw(Shader const& shader, Geometrie const& geo, UploadBuffer<ObjectData>* objectBuffer)
{
    
    mCommandList->SetGraphicsRootSignature(shader.mRootSignature);
    mCommandList->SetPipelineState(shader.mPSO);

    mCommandList->SetGraphicsRootConstantBufferView(0, objectBuffer->Resource()->GetGPUVirtualAddress());
    mCommandList->SetGraphicsRootConstantBufferView(1, mPassDataBuffer->Resource()->GetGPUVirtualAddress());

    D3D12_VERTEX_BUFFER_VIEW vertexBuffer = geo.VertexBufferView();
    D3D12_INDEX_BUFFER_VIEW indexBuffer = geo.IndexBufferView();

    mCommandList->IASetVertexBuffers(0, 1, &vertexBuffer);
    mCommandList->IASetIndexBuffer(&indexBuffer);
    mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    mCommandList->DrawIndexedInstanced(geo.IndicesCount, 1, 0, 0, 0);
    
}
