#include "Geometrie.h"

#include "d3dUtils.h"

void Geometrie::CreateCube(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, float width, float height, float depth)
{
	
	MeshData meshData;
	
	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
    
	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the top face vertex data.
	v[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0);
	v[17] = Vertex(-w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0);
	v[18] = Vertex(-w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0);
	v[19] = Vertex(-w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f);

	};
	
	meshData.Vertices.assign(&v[0], &v[24]);
 
	//
	// Create the indices.
	//
	uint32_t i[36] = {
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

	meshData.Indices16.assign(&i[0], &i[36]);
	
	std::vector<VERTEX> vertex = meshData.Vertices;
	std::vector<uint16> index = meshData.GetIndices16();

	const UINT vbByteSize = (UINT)vertex.size() * sizeof(VERTEX);
	const UINT ibByteSize = (UINT)index.size() * sizeof(uint16);

	// Copy the triangle data to the vertex buffer.
	VertexBufferGPU = d3dUtils::CreateBuffer(device, commandList, vertex.data(), vbByteSize);
	VertexBufferGPU->SetName(L"VERTEX_BUFFER");
	
	// Copy the triangle data to the indices buffer.
	IndexBufferGPU = d3dUtils::CreateBuffer(device, commandList, index.data(), ibByteSize);
	IndexBufferGPU->SetName(L"INDEX_BUFFER");

	// Initialize the vertex buffer view.
	VertexByteStride = sizeof(VERTEX);
	VertexBufferByteSize = vbByteSize;

	// Initialize the indices buffer view.
	IndexFormat = DXGI_FORMAT_R16_UINT;
	IndexBufferByteSize = ibByteSize;
	
	IndicesCount = index.size();
	
}
