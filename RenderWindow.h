#pragma once

#include "d3dUtils.h"
#include "Shader.h"
#include "Transform.h"
#include "UploadBuffer.h"
#include "Window.h"

class Shader;
class Geometrie;

struct ObjectData {
    DirectX::XMFLOAT4X4 world;
};

struct GlobalInformation
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
    void Draw(Shader const& shader, Geometrie const& geo, UploadBuffer<ObjectData> const& objectBuffer);
    void EndDraw() override;
    
    void OnMouseDown(WPARAM btnState, int x, int y) override{}
    void OnMouseUp(WPARAM btnState, int x, int y) override{}
    void OnMouseMove(WPARAM btnState, int x, int y) override{}
    void OnKeyPressed(WPARAM btnState, int x, int y) override{}
    
    TRANSFORM cam;
    
    DirectX::XMFLOAT4X4 mView;
    DirectX::XMFLOAT4X4 mProj;
    UploadBuffer<GlobalInformation>* mGlobalConstantBuffer;
};
