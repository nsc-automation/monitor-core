#pragma once

class WindowViewport {
public:
    void Initialize(HWND hWnd) noexcept;
    void Release() noexcept;

    void Present() noexcept;
private:
    ID3D11Device* m_Device = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;
    IDXGISwapChain* m_SwapChain = nullptr;

    ID3D11Texture2D* m_BackBuffer = nullptr;
    ID3D11RenderTargetView* m_BackbufferRTV = nullptr;
};
