#include "pch.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

ID3D11Device* device_ptr                       = NULL;
ID3D11DeviceContext* device_context_ptr        = NULL;
IDXGISwapChain* swap_chain_ptr                 = NULL;
ID3D11RenderTargetView* render_target_view_ptr = NULL;

ID3D11VertexShader* vertex_shader_ptr = NULL;
ID3D11PixelShader* pixel_shader_ptr   = NULL;

ID3D11InputLayout* input_layout_ptr   = NULL;

float vertex_data_array[] = {
        0.0f,  0.5f,  0.0f, // point at top
        0.5f, -0.5f,  0.0f, // point at bottom-right
        -0.5f, -0.5f,  0.0f, // point at bottom-left
};
UINT vertex_stride              = 3 * sizeof( float );
UINT vertex_offset              = 0;
UINT vertex_count               = 3;

void CompileShaders() {
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif
    ID3DBlob *vs_blob_ptr = NULL, *ps_blob_ptr = NULL, *error_blob = NULL;

    // COMPILE VERTEX SHADER
    HRESULT hr = D3DCompileFromFile(
            L"shaders.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "vs_main",
            "vs_5_0",
            flags,
            0,
            &vs_blob_ptr,
            &error_blob );
    if ( FAILED( hr ) ) {
        if ( error_blob ) {
            OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
            error_blob->Release();
        }
        if ( vs_blob_ptr ) { vs_blob_ptr->Release(); }
        assert( false );
    }

    // COMPILE PIXEL SHADER
    hr = D3DCompileFromFile(
            L"shaders.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "ps_main",
            "ps_5_0",
            flags,
            0,
            &ps_blob_ptr,
            &error_blob );
    if ( FAILED( hr ) ) {
        if ( error_blob ) {
            OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
            error_blob->Release();
        }
        if ( ps_blob_ptr ) { ps_blob_ptr->Release(); }
        assert( false );
    }

    hr = device_ptr->CreateVertexShader(
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            NULL,
            &vertex_shader_ptr );
    assert( SUCCEEDED( hr ) );

    hr = device_ptr->CreatePixelShader(
            ps_blob_ptr->GetBufferPointer(),
            ps_blob_ptr->GetBufferSize(),
            NULL,
            &pixel_shader_ptr );
    assert( SUCCEEDED( hr ) );

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            /*
  { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  */
    };
    hr = device_ptr->CreateInputLayout(
            inputElementDesc,
            ARRAYSIZE( inputElementDesc ),
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            &input_layout_ptr );
    assert( SUCCEEDED( hr ) );

}


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


    DXGI_SWAP_CHAIN_DESC swap_chain_descr               = { 0 };
    swap_chain_descr.BufferDesc.RefreshRate.Numerator   = 0;
    swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descr.BufferDesc.Format  = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_descr.SampleDesc.Count   = 1;
    swap_chain_descr.SampleDesc.Quality = 0;
    swap_chain_descr.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descr.BufferCount        = 1;
    swap_chain_descr.OutputWindow       = hWnd;
    swap_chain_descr.Windowed           = true;

    D3D_FEATURE_LEVEL feature_level;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            flags,
            NULL,
            0,
            D3D11_SDK_VERSION,
            &swap_chain_descr,
            &swap_chain_ptr,
            &device_ptr,
            &feature_level,
            &device_context_ptr );
    assert( S_OK == hr && swap_chain_ptr && device_ptr && device_context_ptr );

    ID3D11Texture2D* framebuffer;
    hr = swap_chain_ptr->GetBuffer(
            0,
            __uuidof( ID3D11Texture2D ),
            (void**)&framebuffer );
    assert( SUCCEEDED( hr ) );

    hr = device_ptr->CreateRenderTargetView(
            framebuffer, 0, &render_target_view_ptr );
    assert( SUCCEEDED( hr ) );

    CompileShaders();

    ID3D11Buffer* vertex_buffer_ptr = NULL;
    { /*** load mesh data into vertex buffer **/
        D3D11_BUFFER_DESC vertex_buff_descr     = {};
        vertex_buff_descr.ByteWidth             = sizeof( vertex_data_array );
        vertex_buff_descr.Usage                 = D3D11_USAGE_DEFAULT;
        vertex_buff_descr.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sr_data          = { 0 };
        sr_data.pSysMem                         = vertex_data_array;
        HRESULT hr = device_ptr->CreateBuffer(
                &vertex_buff_descr,
                &sr_data,
                &vertex_buffer_ptr );
        assert( SUCCEEDED( hr ) );
    }



    MSG msg = {nullptr};
    bool shouldClose = false;
    while ( !shouldClose ) {
        /**** handle user input and other window events ****/
        if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        if ( msg.message == WM_QUIT ) { break; }

        { /*** RENDER A FRAME ***/
            /* clear the back buffer to cornflower blue for the new frame */
            float background_colour[4] = { 0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f };
            device_context_ptr->ClearRenderTargetView( render_target_view_ptr, background_colour );

            /**** Rasteriser state - set viewport area *****/
            RECT winRect;
            GetClientRect( hWnd, &winRect );
            D3D11_VIEWPORT viewport = { 0.0f, 0.0f, ( FLOAT )( winRect.right - winRect.left ), ( FLOAT )( winRect.bottom - winRect.top ), 0.0f, 1.0f };
            device_context_ptr->RSSetViewports( 1, &viewport );

            /**** Output Merger *****/
            device_context_ptr->OMSetRenderTargets( 1, &render_target_view_ptr, NULL );

            /***** Input Assembler (map how the vertex shader inputs should be read from vertex buffer) ******/
            device_context_ptr->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
            device_context_ptr->IASetInputLayout( input_layout_ptr );
            device_context_ptr->IASetVertexBuffers( 0, 1, &vertex_buffer_ptr, &vertex_stride, &vertex_offset );

            /*** set vertex shader to use and pixel shader to use, and constant buffers for each ***/
            device_context_ptr->VSSetShader( vertex_shader_ptr, NULL, 0 );
            device_context_ptr->PSSetShader( pixel_shader_ptr, NULL, 0 );

            /*** draw the vertex buffer with the shaders ****/
            device_context_ptr->Draw( vertex_count, 0 );

            /**** swap the back and front buffers (show the frame we just drew) ****/
            swap_chain_ptr->Present( 1, 0 );
        } // end of frame
    } // end of main loop

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
