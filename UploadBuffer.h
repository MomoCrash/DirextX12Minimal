#pragma once

#include "d3dUtils.h"
#include "d3dx12.h"

class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, size_t size, UINT elementCount, bool isConstantBuffer);

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer();

    ID3D12Resource* Resource()const;

    void CopyData(int elementIndex, void* data);

private:
    ID3D12Resource* mUploadBuffer;
    BYTE* mMappedData = nullptr;

    size_t mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};
