#include "pch.h"

#include "Renderer.h"

#include <tiny_obj_loader.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

void Renderer::Initialize() noexcept {
    m_Device->GetImmediateContext(&m_DeviceContext);
    CreateGBufferTextures();
}

void Renderer::Release() noexcept {
    m_GAlbedoRGB_X_RTV->Release();
    m_GDepthStencil_DSV->Release();
    m_GAlbedoRGB_X->Release();
    m_GDepthStencil->Release();
}

void Renderer::CreateGBufferTextures() noexcept {
    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
//    texDesc.Width = ;
//    texDesc.Height = ;
    texDesc.MipLevels = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.ArraySize = 1;

    m_Device->CreateTexture2D(&texDesc, nullptr, &m_GAlbedoRGB_X);
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    m_Device->CreateRenderTargetView(m_GAlbedoRGB_X, &rtvDesc, &m_GAlbedoRGB_X_RTV);

    texDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    m_Device->CreateTexture2D(&texDesc, nullptr, &m_GDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    m_Device->CreateDepthStencilView(m_GDepthStencil, &dsvDesc, &m_GDepthStencil_DSV);
}

void Renderer::DrawGeo() noexcept {
    ID3D11RenderTargetView *RTVs[] = {m_GAlbedoRGB_X_RTV};
    ID3D11DepthStencilView *DSV = m_GDepthStencil_DSV;

    for (ID3D11RenderTargetView* RTV : RTVs) {
        static const float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        m_DeviceContext->ClearRenderTargetView(RTV, clearColor);
    }

//    m_DeviceContext->VSSetShader();
//    m_DeviceContext->PSSetShader();

    ID3D11Buffer *CBVs[] = {m_CameraParamsCB};

    m_DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(CBVs), CBVs);

    m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//    m_DeviceContext->IASetInputLayout();
//    m_DeviceContext->IASetVertexBuffers();
//    m_DeviceContext->IAGetIndexBuffer();
//
//    D3D11_VIEWPORT viewport{_, _, _, _};
    // m_DeviceContext->RSSetViewports(1, &viewport);

//    RECT winRect;
//    GetClientRect( hWnd, &winRect );
//    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, ( FLOAT )( winRect.right - winRect.left ), ( FLOAT )( winRect.bottom - winRect.top ), 0.0f, 1.0f };

    m_DeviceContext->OMSetRenderTargets(ARRAYSIZE(RTVs), RTVs, DSV);
    // m_DeviceContext->DrawIndexed(_, 0, 0);
}

void Renderer::LoadModel() noexcept {

}
