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

    UploadBuffer<ObjectData>* constantBuffer = new UploadBuffer<ObjectData>(window->GetDevice(), 1, true);
    constantBuffer->Resource()->SetName(L"PASS_BUFFER");

    window->CloseCommandList();

    UploadBuffer<ObjectData>* cb2 = new UploadBuffer<ObjectData>(window->GetDevice(), 1, true);
    constantBuffer->Resource()->SetName(L"PASS_BUFFER");

    TRANSFORM transform;
    transform.Identity();
    transform.SetPosition(0.0f, 0.0f, 0.0f);
    transform.Rotate(0.0f, 0.0f, 0.0f);
    transform.UpdateMatrix();

    TRANSFORM transform2;
    transform.Identity();
    transform.SetPosition(3.0f, 0.0f, 0.0f);
    transform.Rotate(0.0f, 0.0f, 0.0f);
    transform.UpdateMatrix();

    bool closed = false;

    while(!closed) {

        window->Update();
        int event;
        while (window->PollWindowEvents(event)) {


            if (event == WM_QUIT) {
                closed = true;
            }

        }

        ObjectData objConstants;
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform.GetMatrix()));
        constantBuffer->CopyData(0, objConstants);
        
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform2.GetMatrix()));
        cb2->CopyData(0, objConstants);

        window->BeginDraw();

        window->Draw(*window->mShader, *geo, constantBuffer);
        window->Draw(*window->mShader, *geo, cb2);

        window->EndDraw();

    }
    
}
