#pragma once

class WindowViewport {
public:
    void Initialize(HWND hWnd) noexcept;
    void Release() noexcept;
    void Reconfigure(size_t width, size_t height) noexcept;

    void Present() noexcept;

    ID3D11Device* GetDevice() noexcept;
    std::pair<UINT, UINT> GetResolution() noexcept;
private:
    static constexpr auto ms_SwapchainFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

    ID3D11Device* m_Device = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;
    IDXGISwapChain* m_SwapChain = nullptr;

    ID3D11Texture2D* m_BackBuffer = nullptr;
    ID3D11RenderTargetView* m_BackbufferRTV = nullptr;
};
