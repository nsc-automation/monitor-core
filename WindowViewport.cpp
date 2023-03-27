#include "pch.h"

#include "WindowViewport.h"

void WindowViewport::Initialize(HWND hWnd) noexcept {
    DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapchainDesc.BufferDesc.Format = ms_SwapchainFormat;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.OutputWindow = hWnd;
    swapchainDesc.Windowed = true;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    D3D_FEATURE_LEVEL featureLevel;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#if defined(DEBUG) || defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               nullptr,
                                               flags,
                                               nullptr,
                                               0,
                                               D3D11_SDK_VERSION,
                                               &swapchainDesc,
                                               &m_SwapChain,
                                               &m_Device,
                                               &featureLevel,
                                               &m_DeviceContext);

    assert(SUCCEEDED(hr));
    assert(m_Device);
    assert(m_DeviceContext);
    assert(m_SwapChain);


    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&m_BackBuffer));
    assert(SUCCEEDED(hr));

    hr = m_Device->CreateRenderTargetView(m_BackBuffer, nullptr, &m_BackbufferRTV);
    assert(SUCCEEDED(hr));
}

void WindowViewport::Release() noexcept {
    m_SwapChain->Release();
    m_Device->Release();
}

void WindowViewport::Reconfigure(size_t width, size_t height) noexcept {
    m_DeviceContext->Flush();
    HRESULT hr = m_SwapChain->ResizeBuffers(3, width, height, ms_SwapchainFormat, 0);
    assert(SUCCEEDED(hr));
}

void WindowViewport::Present() noexcept {
    HRESULT hr = m_SwapChain->Present(1, 0);
    assert(SUCCEEDED(hr));
}

