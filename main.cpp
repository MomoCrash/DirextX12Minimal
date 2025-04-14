#include <Windows.h>
#include "RenderWindow.h"
#include "Geometrie.h"
#include "RenderObject.h"
#include "Transform.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    RenderWindow window = RenderWindow(hInst);
    window.Initialize();

    Geometrie geo = Geometrie();
    geo.InitializeAsCube(window.GetDevice(), window.GetCommandList());

    Shader shader = Shader(window.GetDevice(), window.GetBackBufferFormat(), window.GetDepthStencilFormat());;
    
    window.CloseCommandList();

    std::vector<RenderObject> sceneObjects {
        { window.GetDevice(), geo, shader, 10, 10, 10 },
        { window.GetDevice(), geo, shader, 12, 10, 10 },
        { window.GetDevice(), geo, shader, 13, 10, 10 },
        { window.GetDevice(), geo, shader, 14, 10, 10 }
    };

    bool closed = false;
    float angle = 0;
    float time = 0;
    while (!closed) {
        time += 0.001f;
        window.Update();
        int event;
        while (window.PollWindowEvents(event)) {
            if (event == WM_QUIT) {
                closed = true;
            }
        }

        XMFLOAT3 forward = window.cam.Forward();
        XMFLOAT3 right = window.cam.Right();
        XMFLOAT3 up = window.cam.Up();

        if (GetAsyncKeyState('Z') < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ forward.x * 0.005f, 0, forward.z * 0.005f });
        }
        if (GetAsyncKeyState('Q') < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ -right.x * 0.005f, 0, -right.z * 0.005f });
        }
        if (GetAsyncKeyState('S') < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ -forward.x * 0.005f, 0, -forward.z * 0.005f });
        }
        if (GetAsyncKeyState('D') < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ right.x * 0.005f, 0, right.z * 0.005f });
        }

        if (GetAsyncKeyState('A') < 0) {
            angle -= 0.001f;
        }
        if (GetAsyncKeyState('E') < 0) {
            angle += 0.001f;
        }
        window.cam.SetRotationYPR(XMFLOAT3{ angle, 0, 0 });

        if (GetAsyncKeyState(VK_SPACE) < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ 0, up.y * 0.005f, 0 });
        }
        if (GetAsyncKeyState(VK_SHIFT) < 0) {
            window.cam.OffsetPosition(XMFLOAT3{ 0, -up.y * 0.005f, 0 });
        }

        window.BeginDraw();

        for (RenderObject& object : sceneObjects)
        {
            window.Draw(object.GetShader(), object.GetGeometrie(), object.GetUploadedData());
        }

        window.EndDraw();
    }

    // Clear objects memory
    for (RenderObject& object : sceneObjects)
    {
        delete object.GetUploadedData();
    }
    
}