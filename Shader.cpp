#include "Shader.h"


Shader::Shader(ID3D12Device* device, DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat)
{
    /*std::vector<CD3DX12_ROOT_PARAMETER> slotRootParameter(2);

    slotRootParameter[0].InitAsConstantBufferView(0);
    slotRootParameter[1].InitAsConstantBufferView(1);
    
    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2,
                                            slotRootParameter.data(),
                                            0,
                                            nullptr, 
                                            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig, &errorBlob);*/


    D3D12_INPUT_ELEMENT_DESC inputLayout[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    mVertexShaderCPU = CompileShader(L"Shaders/VS.hlsl", nullptr, "VS", "vs_5_1");
    mPixelShaderCPU = CompileShader(L"Shaders/PS.hlsl", nullptr, "PS", "ps_5_1");
    mRootSignatureCPU = CompileShader(L"Shaders/Rootsignature.hlsl", nullptr, "ROOTSIG", "rootsig_1_1");
    
    device->CreateRootSignature(
        0,
        mRootSignatureCPU->GetBufferPointer(),
        mRootSignatureCPU->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)
    );
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
    psoDesc.pRootSignature = mRootSignature;
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout)};
    psoDesc.VS = {reinterpret_cast<BYTE*>(mVertexShaderCPU->GetBufferPointer()), mVertexShaderCPU->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<BYTE*>(mPixelShaderCPU->GetBufferPointer()), mPixelShaderCPU->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    psoDesc.RTVFormats[0] = rtvFormat;
    psoDesc.DSVFormat = dsvFormat;
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
    
}

Shader::~Shader()
{
    mPSO->Release();
    mRootSignature->Release();

    mRootSignatureCPU->Release();
    mVertexShaderCPU->Release();
    mPixelShaderCPU->Release();
}

ID3DBlob* Shader::CompileShader(const std::wstring& filename,
                                const D3D_SHADER_MACRO* defines,
                                const std::string& entrypoint,
                                const std::string& target)
{

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* byteCode = nullptr;
    ID3DBlob* errors;
    HRESULT hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);
    
    if (FAILED(hr))
    {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        std::wcerr << "Failed to compile shader ! " << errMsg << "\n";
        return nullptr;
    }

    return byteCode;
    
}
