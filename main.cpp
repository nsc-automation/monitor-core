#include "Renderer.h"
#include "WindowViewport.h"
#include "pch.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

//void CompileShaders() {
//    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined(DEBUG) || defined(_DEBUG)
//    flags |= D3DCOMPILE_DEBUG;// add more debug output
//#endif
//    ID3DBlob *vs_blob_ptr = NULL, *ps_blob_ptr = NULL, *error_blob = NULL;
//
//    // COMPILE VERTEX SHADER
//    HRESULT hr = D3DCompileFromFile(
//            L"shaders.hlsl",
//            nullptr,
//            D3D_COMPILE_STANDARD_FILE_INCLUDE,
//            "vs_main",
//            "vs_5_0",
//            flags,
//            0,
//            &vs_blob_ptr,
//            &error_blob);
//    if (FAILED(hr)) {
//        if (error_blob) {
//            OutputDebugStringA((char *) error_blob->GetBufferPointer());
//            error_blob->Release();
//        }
//        if (vs_blob_ptr) { vs_blob_ptr->Release(); }
//        assert(false);
//    }
//
//    // COMPILE PIXEL SHADER
//    hr = D3DCompileFromFile(
//            L"shaders.hlsl",
//            nullptr,
//            D3D_COMPILE_STANDARD_FILE_INCLUDE,
//            "ps_main",
//            "ps_5_0",
//            flags,
//            0,
//            &ps_blob_ptr,
//            &error_blob);
//    if (FAILED(hr)) {
//        if (error_blob) {
//            OutputDebugStringA((char *) error_blob->GetBufferPointer());
//            error_blob->Release();
//        }
//        if (ps_blob_ptr) { ps_blob_ptr->Release(); }
//        assert(false);
//    }
//
//    hr = device_ptr->CreateVertexShader(
//            vs_blob_ptr->GetBufferPointer(),
//            vs_blob_ptr->GetBufferSize(),
//            NULL,
//            &vertex_shader_ptr);
//    assert(SUCCEEDED(hr));
//
//    hr = device_ptr->CreatePixelShader(
//            ps_blob_ptr->GetBufferPointer(),
//            ps_blob_ptr->GetBufferSize(),
//            NULL,
//            &pixel_shader_ptr);
//    assert(SUCCEEDED(hr));
//
//    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
//            {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//            /*
//  { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//  { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//  { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//  */
//    };
//    hr = device_ptr->CreateInputLayout(
//            inputElementDesc,
//            ARRAYSIZE(inputElementDesc),
//            vs_blob_ptr->GetBufferPointer(),
//            vs_blob_ptr->GetBufferSize(),
//            &input_layout_ptr);
//    assert(SUCCEEDED(hr));
//}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
    LPCTSTR lpzClass = TEXT("My Window Class!");

    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    int x = screen_rect.right / 2 - 150;
    int y = screen_rect.bottom / 2 - 75;

    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

    HWND hWnd = CreateWindow(lpzClass, TEXT("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL,
                             hInstance, nullptr);
    if (!hWnd) return 2;

    WindowViewport windowViewport{};
    windowViewport.Initialize(hWnd);
    Renderer renderer{};
    renderer.Initialize();

    MSG msg = {nullptr};
    bool shouldClose = false;
    while (!shouldClose) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) { break; }

        {
            renderer.DrawGeo();
        }
    }

    renderer.Release();
    windowViewport.Release();

    return msg.wParam;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName) {
    WNDCLASS wcWindowClass = {0};
    wcWindowClass.lpfnWndProc = (WNDPROC) WndProc;
    wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpzClassName;
    wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcWindowClass.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE;
    return RegisterClass(&wcWindowClass);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
