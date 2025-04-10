#pragma once

#include "d3dUtils.h"
#include "UploadBuffer.h"
#include "Window.h"
#include "Transform.h"

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
    TRANSFORM cam;

    RenderWindow(HINSTANCE hInstance);
    bool Initialize() override;
    void Update() override;
    
    void OnResize() override;

    void OpenCommandList();
    void CloseCommandList();

public:
    void BeginDraw() override;
    void Draw(Shader& shader, Geometrie& geo, UploadBuffer<ObjectData>* objectBuffer);
    void EndDraw() override;
    
    void OnMouseDown(WPARAM btnState, int x, int y) override{}
    void OnMouseUp(WPARAM btnState, int x, int y) override{}
    void OnMouseMove(WPARAM btnState, int x, int y) override{}
    void OnKeyPressed(WPARAM btnState, int x, int y) override{}

public:
    DirectX::XMFLOAT4X4 mView;
    DirectX::XMFLOAT4X4 mProj;
    UploadBuffer<GlobalInformation>* mGlobalConstantBuffer;
    Shader* mShader;
};
