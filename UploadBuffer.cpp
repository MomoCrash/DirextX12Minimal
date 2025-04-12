#include "UploadBuffer.h"

UploadBuffer::UploadBuffer(ID3D12Device* device, size_t size, UINT elementCount, bool isConstantBuffer)
: mIsConstantBuffer(isConstantBuffer), mElementByteSize(size)
{

    // Constant buffer elements need to be multiples of 256 bytes.
    if(isConstantBuffer)
        mElementByteSize = d3dUtils::CalcConstantBufferByteSize(size);

    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC buffer = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize*elementCount);
    device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &buffer,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mUploadBuffer));

    mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
    
}

UploadBuffer::~UploadBuffer()
{
    if(mUploadBuffer != nullptr)
        mUploadBuffer->Unmap(0, nullptr);

    mMappedData = nullptr;
}

ID3D12Resource* UploadBuffer::Resource() const
{
    return mUploadBuffer;
}

void UploadBuffer::CopyData(int elementIndex, void* data)
{
    memcpy(&mMappedData[elementIndex*mElementByteSize], data, mElementByteSize);
}