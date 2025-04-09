﻿#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#define D3D_DEBUG_INFO
#include <crtdbg.h>
#endif
#include <windows.h>
#include <WindowsX.h>
#include <iostream>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dx12.h"
#include <DirectXColors.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class d3dUtils
{
public:
    static UINT CalcConstantBufferByteSize(UINT byteSize);

    static bool IsKeyDown(int vkeyCode);

    static ID3D12Resource* CreateBuffer(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* commandList,
        const void* initData,
        UINT64 byteSize,
        ID3D12Resource* uploadBuffer);
    
};
