#pragma once
#include "Geometrie.h"
#include "Shader.h"
#include "Transform.h"
#include "UploadBuffer.h"

struct ObjectData {
    DirectX::XMFLOAT4X4 world;
};

class RenderObject
{
public:
    RenderObject(ID3D12Device* device, Geometrie const& geometrie, Shader const& shader, float x, float y, float z);
    
    void Update();

    TRANSFORM& GetTransform();

    const Geometrie& GetGeometrie();
    const Shader& GetShader();

    UploadBuffer<ObjectData>* GetUploadedData();
    
private:
    TRANSFORM mTransform;
    const Geometrie* mGeometrie;
    const Shader* mShader;

    ObjectData mObjConstants;
    UploadBuffer<ObjectData>* mObjectData;
};
