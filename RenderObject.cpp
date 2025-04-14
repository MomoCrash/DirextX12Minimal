#include "RenderObject.h"

RenderObject::RenderObject(ID3D12Device* device, Geometrie const& geometrie, Shader const& shader, float x, float y, float z)
: mGeometrie(&geometrie), mShader(&shader), mObjectData(nullptr)
{
    mTransform.Reset();
    mTransform.SetPosition(XMFLOAT3(x, y, z));

    mObjectData = new UploadBuffer<ObjectData>(device, 1, true);
    XMStoreFloat4x4(&mObjConstants.world, XMMatrixTranspose(mTransform.GetMatrix()));
    mObjectData->CopyData(0, mObjConstants);
}

void RenderObject::Update()
{
    XMStoreFloat4x4(&mObjConstants.world, XMMatrixTranspose(mTransform.GetMatrix()));
    mObjectData->CopyData(0, mObjConstants);
}

TRANSFORM& RenderObject::GetTransform()
{
    return mTransform;
}

const Geometrie& RenderObject::GetGeometrie()
{
    return *mGeometrie;
}

const Shader& RenderObject::GetShader()
{
    return *mShader;
}

UploadBuffer<ObjectData>* RenderObject::GetUploadedData()
{
    return mObjectData;
}
