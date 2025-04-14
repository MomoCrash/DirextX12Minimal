#pragma once

#include "d3dUtils.h"

struct Vertex
{
    Vertex() = default;
    Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT4& c) : Position(p), Color(c) {}
    Vertex(float px, float py, float pz, float cx, float cy, float cz, float ca) : Position(px,py,pz), Color(cx, cy, cz, ca) {}
    
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT4 Color;
};

using uint16 = std::uint16_t;
using uint32 = std::uint32_t;


class Geometrie
{
    
public:
    Geometrie() = default;
    ~Geometrie();

    void InitializeAsCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

    // Coter GPU
    ID3D12Resource* VertexBufferGPU = nullptr;
    ID3D12Resource* IndexBufferGPU = nullptr;

    // Les donnees pour dessiner a l'eran
    UINT VertexByteStride = 0;
    UINT VertexBufferByteSize = 0;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT IndexBufferByteSize = 0;

    UINT IndicesCount = 0;

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;

    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;
};

