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

        if (d3dUtils::IsKeyDown('Z'))
        {
            DirectX::XMFLOAT3 cube1 = transform.position;
            transform.SetPosition(cube1.x, cube1.y, cube1.z + 0.0005f);
            transform.UpdateMatrix();
        }

        if (d3dUtils::IsKeyDown('S'))
        {
            DirectX::XMFLOAT3 cube1 = transform.position;
            transform.SetPosition(cube1.x, cube1.y, cube1.z - 0.0005f);
            transform.UpdateMatrix();
        }


        ObjectData objConstants;
        XMStoreFloat4(&objConstants.color, {1.0, 1.0, 1.0, 1.0f});
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform.GetMatrix()));
        constantBuffer->CopyData(0, objConstants);
        
        DirectX::XMStoreFloat4x4(&objConstants.world, DirectX::XMMatrixTranspose(transform2.GetMatrix()));
        XMStoreFloat4(&objConstants.color, {1.0, 0.0, 0.0, 1.0f});
        cb2->CopyData(0, objConstants);

        window->BeginDraw();

        window->Draw(*window->mShader, *geo, constantBuffer);
        window->Draw(*window->mShader, *geo, cb2);

        window->EndDraw();

    }
    
}
