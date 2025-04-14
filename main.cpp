#include <Windows.h>
#include "RenderWindow.h"
#include "Geometrie.h"
#include "Transform.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    RenderWindow window = RenderWindow(hInst);
    window.Initialize();

    Geometrie geo = Geometrie();
    geo.InitializeAsCube(window.GetDevice(), window.GetCommandList());
    window.CloseCommandList();

    Shader shader = Shader(window.GetDevice(), window.GetBackBufferFormat(), window.GetDepthStencilFormat());
    
    window.cam.Reset();
    window.cam.SetPosition(XMFLOAT3{ 0.0f, 0.0f, -5.0f });
    window.cam.SetRotationYPR(XMFLOAT3{ 0.0f, 0.0f, 0.0f });


    TRANSFORM transform;
    transform.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
    transform.SetScaling(XMFLOAT3(1.0f, 1.0f, 1.0f));
    transform.SetRotationYPR(XMFLOAT3(45.0f, 0.0f, 0.0f));
    transform.UpdateMatrix();

    ObjectData objData;
    XMStoreFloat4x4(&objData.World, XMMatrixTranspose(transform.GetMatrix()));

    auto constantBuffer = UploadBuffer<ObjectData>(window.GetDevice(), 1, true);
    constantBuffer.CopyData(0, objData);


    while (window.mIsOpen) {

        window.Update();

        window.BeginDraw();
        window.Draw(shader, geo, &constantBuffer);
        window.EndDraw();
    }
}