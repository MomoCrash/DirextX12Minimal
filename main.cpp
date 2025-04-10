#include <Windows.h>
#include "RenderWindow.h"
#include "Geometrie.h"
#include "Transform.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    RenderWindow* window = new RenderWindow(hInst);
    window->Initialize();

    Geometrie* geo = new Geometrie();
    geo->CreateCube(window->GetDevice(), window->GetCommandList(), 1.0f, 1.0f, 1.0f);
    window->CloseCommandList();

    UploadBuffer<ObjectData>* constantBuffer = new UploadBuffer<ObjectData>(window->GetDevice(), 1, true);
    constantBuffer->Resource()->SetName(L"PASS_BUFFER");

    UploadBuffer<ObjectData>* constantBuffer2 = new UploadBuffer<ObjectData>(window->GetDevice(), 1, true);
    constantBuffer->Resource()->SetName(L"PASS_BUFFER");

    TRANSFORM transform;
    transform.Reset();
    transform.SetPosition(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
    transform.SetRotationYPR(XMFLOAT3{ 0.0f, 0.0f, 0.0f });

    TRANSFORM transform2;
    transform2.Reset();
    transform2.SetPosition(XMFLOAT3{ 2.0f, 0.0f, 0.0f });
    transform2.SetRotationYPR(XMFLOAT3{ 0.0f, 0.0f, 0.0f });

    bool closed = false;
    float angle = 0;
    float time = 0;
    while (!closed) {
        time += 0.001f;
        window->Update();
        int event;
        while (window->PollWindowEvents(event)) {
            if (event == WM_QUIT) {
                closed = true;
            }
        }

        bool Z = GetAsyncKeyState('Z') < 0;
        bool Q = GetAsyncKeyState('Q') < 0;
        bool S = GetAsyncKeyState('S') < 0;
        bool D = GetAsyncKeyState('D') < 0;

        bool A = GetAsyncKeyState('A') < 0;
        bool E = GetAsyncKeyState('E') < 0;

        XMFLOAT3 forward = window->cam.Forward();
        XMFLOAT3 right = window->cam.Right();

        if (Z) {
            window->cam.OffsetPosition(XMFLOAT3{ forward.x * 0.005f, 0, forward.z * 0.005f });
        }
        if (Q) {
            window->cam.OffsetPosition(XMFLOAT3{ -right.x * 0.005f, 0, -right.z * 0.005f });
        }
        if (S) {
            window->cam.OffsetPosition(XMFLOAT3{ -forward.x * 0.005f, 0, -forward.z * 0.005f });
        }
        if (D) {
            window->cam.OffsetPosition(XMFLOAT3{ right.x * 0.005f, 0, right.z * 0.005f });
        }

        if (A) {
            angle -= 0.001f;
        }
        if (E) {
            angle += 0.001f;
        }
        window->cam.SetRotationYPR(XMFLOAT3{ angle, 0, 0 });

        transform.SetRotationYPR(XMFLOAT3{ time, time, 0 });

        ObjectData objConstants;
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform.GetMatrix()));
        constantBuffer->CopyData(0, objConstants);
        
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform2.GetMatrix()));
        constantBuffer2->CopyData(0, objConstants);

        window->BeginDraw();

        window->Draw(*window->mShader, *geo, constantBuffer);
        window->Draw(*window->mShader, *geo, constantBuffer2);

        window->EndDraw();
    }
    
}
