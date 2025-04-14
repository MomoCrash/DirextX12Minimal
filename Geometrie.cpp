#include "Geometrie.h"

#include "d3dUtils.h"

void Geometrie::InitializeAsCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{

	Vertex v[24] {
		// FRONT
		{{ -.5f, -.5f, -.5f }, {  0, 0, 0, 1 }},	// 0
		{{ -.5f,  .5f, -.5f }, {  0, 1, 0, 1 }},	// 1
		{{  .5f,  .5f, -.5f }, {  1, 1, 0, 1 }},	// 2
		{{  .5f, -.5f, -.5f }, {  1, 0, 0, 1 }},	// 3

		// BACK
		{{ -.5f, -.5f,  .5f }, {  0, 0, 1, 1 }},	// 4
		{{  .5f, -.5f,  .5f }, {  1, 0, 1, 1 }},	// 5
		{{  .5f,  .5f,  .5f }, {  1, 1, 1, 1 }},	// 6
		{{ -.5f,  .5f,  .5f }, {  0, 1, 1, 1 }},	// 7 

		// LEFT
		{{ -.5f, -.5f,  .5f }, { 0, 0, 1, 1 }},		// 8
		{{ -.5f,  .5f,  .5f }, { 0, 1, 1, 1 }},		// 9
		{{ -.5f,  .5f, -.5f }, { 0, 1, 0, 1 }},		// 10
		{{ -.5f, -.5f, -.5f }, { 0, 0, 0, 1 }},		// 11

		// RIGHT
		{{  .5f, -.5f, -.5f }, {  1, 0, 0, 1 }},	// 12
		{{  .5f,  .5f, -.5f }, {  1, 1, 0, 1 }},	// 13
		{{  .5f,  .5f,  .5f }, {  1, 1, 1, 1 }},	// 14
		{{  .5f, -.5f,  .5f }, {  1, 0, 1, 1 }},	// 15

		// TOP
		{{ -.5f,  .5f, -.5f }, {  0, 1, 0, 1 }},	// 16
		{{ -.5f,  .5f,  .5f }, {  0, 1, 1, 1 }},	// 17
		{{  .5f,  .5f,  .5f }, {  1, 1, 1, 1 }},	// 18
		{{  .5f,  .5f, -.5f }, {  1, 1, 0, 1 }},	// 19

		// BOTTOM
		{{ -.5f, -.5f,  .5f }, {  0, 0, 1, 1 }},	// 20
		{{ -.5f, -.5f, -.5f }, {  0, 0, 0, 1 }},	// 21
		{{  .5f, -.5f, -.5f }, {  1, 0, 0, 1 }},	// 22
		{{  .5f, -.5f,  .5f }, {  1, 0, 1, 1 }},	// 23

	};

	uint16 i[36] {
		// FRONT
		0, 1, 2,
		0, 2, 3,
		
		// BACK
		4, 5, 6,
		4, 6, 7,
		
		// LEFT
		8, 9, 10,
		8, 10, 11,

		// RIGHT
		12, 13, 14,
		12, 14, 15,

		// TOP 
		16, 17, 18,
		16, 18, 19,

		// BOTTOM
		20, 21, 22,
		20, 22, 23
	};

	const size_t vbByteSize = _countof(v) * sizeof(Vertex);
	const size_t ibByteSize = _countof(i) * sizeof(uint16);

	// Copy the triangle data to the vertex buffer.
	VertexBufferGPU = d3dUtils::CreateBuffer(device, commandList, v, vbByteSize);
	VertexBufferGPU->SetName(L"VERTEX_BUFFER");
	
	// Copy the triangle data to the indices buffer.
	IndexBufferGPU = d3dUtils::CreateBuffer(device, commandList, i, ibByteSize);
	IndexBufferGPU->SetName(L"INDEX_BUFFER");

	// Initialize the vertex buffer view.
	VertexByteStride = sizeof(Vertex);
	VertexBufferByteSize = vbByteSize;

	// Initialize the indices buffer view.
	IndexFormat = DXGI_FORMAT_R16_UINT;
	IndexBufferByteSize = ibByteSize;
	IndicesCount = _countof(i);
}

D3D12_VERTEX_BUFFER_VIEW Geometrie::VertexBufferView() const
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW Geometrie::IndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	return ibv;
}

Geometrie::~Geometrie()
{
	VertexBufferGPU->Release();
	IndexBufferGPU->Release();
}
