#pragma once

#include "WindowViewport.h"

class Renderer {
public:
    void Initialize(WindowViewport* viewport) noexcept;
    void Release() noexcept;

    void CreateGBufferTextures() noexcept;
    void LoadModel() noexcept;

    void DrawGeo() noexcept;
private:
    ID3D11Device* m_Device = nullptr;
    ID3D11DeviceContext* m_DeviceContext = nullptr;

    ID3D11Texture2D* m_GAlbedoRGB_X = nullptr;
    ID3D11RenderTargetView* m_GAlbedoRGB_X_RTV = nullptr;
    ID3D11Texture2D* m_GDepthStencil = nullptr;
    ID3D11DepthStencilView* m_GDepthStencil_DSV = nullptr;

    ID3D11Buffer* m_CameraParamsCB = nullptr;

    WindowViewport* m_Viewport = nullptr;
};
