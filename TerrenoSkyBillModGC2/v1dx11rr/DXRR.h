#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "ModeloEspecular.h"

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	TerrenoRR *terreno;
	SkyDome *skydome;
	BillboardRR *billboard;
	BillboardRR* Arbusto1;
	BillboardRR* Arbusto2;
	BillboardRR* Arbusto3;
	BillboardRR* Moon;
	Camara *camara;
	ModeloRR* modelazo;
	ModeloEspecular* cottage;
	ModeloRR* cow;
	ModeloRR* agua;
	ModeloEspecular* avion;
	ModeloRR* horse;
	ModeloEspecular* house01;
	ModeloEspecular* house02;
	ModeloEspecular* house03;
	float izqder;
	float arriaba;
	float vel;
	float tiempo = 0.0;
	int tiempo2 = 0;
	D3DXVECTOR3 DireccionLuz;
	D3DXVECTOR3 ColorLuz;
	float FA;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		camara = new Camara(D3DXVECTOR3(27,80,67), D3DXVECTOR3(66,80,-19), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(400, 400, d3dDevice, d3dContext);
		//skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"SkyDome.png");
		DireccionLuz[0] = -190;
		DireccionLuz[1] = 0;
		DireccionLuz[2] = -190;

		ColorLuz[0] = 1;
		ColorLuz[1] = 1;
		ColorLuz[2] = 1;

		FA = 0;


		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Skydome/SkyDome1.5.jpg", L"Skydome/SkyDome2.5.jpg", L"Skydome/SkyDome3.jpg");


		billboard = new BillboardRR(L"Billboard/grass.png", L"Billboard/grass_normal.png", d3dDevice, d3dContext, 1);
		Arbusto1 = new BillboardRR(L"Billboard/Farola1.png", L"Billboard/Farola1_MN.png", d3dDevice, d3dContext, 7);
		Arbusto2 = new BillboardRR(L"Billboard/Arbusto2_Base.png", L"Billboard/Arbusto2_Normal.png", d3dDevice, d3dContext, 3);
		Arbusto3 = new BillboardRR(L"Billboard/Arbusto3_Base.png", L"Billboard/Arbusto3_Normal.png", d3dDevice, d3dContext, 7);
		Moon = new BillboardRR(L"Billboard/Moon.png", L"Billboard/Moon_Normal.png", d3dDevice, d3dContext, 10);

		avion = new ModeloEspecular(d3dDevice, d3dContext, "Modelos/A3/A3.obj", L"Modelos/A3/texture.jpg", L"Modelos/A3/texture_nm.jpg", L"Modelos/A3/texture_esp.jpg", 5.4, 3, 4.7, 47.191, 20, -132.762);
		house01 = new ModeloEspecular(d3dDevice, d3dContext, "Modelos/A5/House01.obj", L"Modelos/A5/texture.jpg", L"Modelos/A5/texture_nm.jpg", L"Modelos/A5/texture_esp.jpg", 3, 1, 4.7, -91.152, 9.5, -67.900);
		house02 = new ModeloEspecular(d3dDevice, d3dContext, "Modelos/A6/House02.obj", L"Modelos/A6/texture1.jpg", L"Modelos/A6/texture1_normal.jpg", L"Modelos/A6/texture1_esp.jpg", 3, 3, 0, -18.559, 12, 82.141);
		house03 = new ModeloEspecular(d3dDevice, d3dContext, "Modelos/A7/House03.obj", L"Modelos/A7/texture 2.jpg", L"Modelos/A7/texture 2_normal.jpg", L"Modelos/A7/texture 2_especular.jpg", 3, 0, 0, -108.825, 13, 26.023);
		horse = new ModeloRR(d3dDevice, d3dContext, "Modelos/A4/A4.obj", L"Modelos/A4/Horse.jpg", L"Modelos/A4/Horse_NM.jpg", 1);
		cottage = new ModeloEspecular(d3dDevice, d3dContext, "Modelos/A1/Co.obj", L"Modelos/A1/cottage_diffuse.jpg", L"Modelos/A1/cottage_normal.jpg", L"Modelos/A1/cottage_diffuse_esp.jpg", .25, 4.5, 4.71, 50.068, 11, 125);
		cow = new ModeloRR(d3dDevice, d3dContext, "Modelos/A2/A2.obj", L"Modelos/A2/colorOpacityCow.jpg", L"Modelos/A2/colorOpacityCowNorm.jpg", 1);
		agua = new ModeloRR(d3dDevice, d3dContext, "Modelos/A8/Agua2.obj", L"Modelos/A8/Agua_B.jpg", L"Modelos/A8/Agua_MN.jpg", 15);

	}

	~DXRR()
	{
		LiberaD3D();
	}

	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		tiempo = tiempo + 0.0005;
		if (tiempo > 0.0 && tiempo < 1.0) {
			DireccionLuz[0] = DireccionLuz[0] + 0.12;
			DireccionLuz[1] = DireccionLuz[1] + 0.0095;
			DireccionLuz[2] = DireccionLuz[2] + 0.12;

			ColorLuz[0] = ColorLuz[0] - 0.00001;
			ColorLuz[1] = ColorLuz[1] - 0.00001;
			ColorLuz[2] = ColorLuz[2] - 0.0001;

			FA = FA + 0.0005;
		}
		else if (tiempo > 1.0 && tiempo < 2.0) {
			tiempo2 = 1;
			DireccionLuz[0] = DireccionLuz[0] + 0.12;
			DireccionLuz[1] = DireccionLuz[1] - 0.0095;
			DireccionLuz[2] = DireccionLuz[2] + 0.12;

			ColorLuz[0] = ColorLuz[0] + 0.00001;
			ColorLuz[1] = ColorLuz[1] + 0.00001;
			ColorLuz[2] = ColorLuz[2] + 0.0001;

			FA = FA - 0.0005;
		}
		else if (tiempo > 2.0 && tiempo < 3.0) {
			tiempo2 = 2;
			
			ColorLuz[0] = 0;
			ColorLuz[1] = 0;
			ColorLuz[2] = 0;

			FA = 0.2;
		}
		else if(tiempo > 3.0 && tiempo < 3.5){			
			tiempo2 = 0;
			tiempo = 0.0;
			DireccionLuz[0] = -190;
			DireccionLuz[1] = 0;
			DireccionLuz[2] = -190;

			ColorLuz[0] = 1;
			ColorLuz[1] = 1;
			ColorLuz[2] = 1;
		}


		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 5 ;
		camara->UpdateCam(vel, arriaba, izqder);
		skydome->Update(camara->vista, camara->proyeccion);

		TurnOffDepth();
		skydome->Render(camara->posCam, tiempo2);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		//TurnOnAlphaBlending();
#pragma region Bill

		Moon->Draw(camara->vista, camara->proyeccion, camara->posCam, 152.45, 20, 160.67, DireccionLuz, ColorLuz, FA);
		
		Arbusto1->Draw(camara->vista, camara->proyeccion, camara->posCam, -83.67, 13.5, -128.66, DireccionLuz, ColorLuz, FA);
		Arbusto1->Draw(camara->vista, camara->proyeccion, camara->posCam, -37.03, 12, -107.117, DireccionLuz, ColorLuz, FA);
		Arbusto1->Draw(camara->vista, camara->proyeccion, camara->posCam, -124.77, 22, -157.48, DireccionLuz, ColorLuz, FA);
		Arbusto1->Draw(camara->vista, camara->proyeccion, camara->posCam, 9.5, 12, -86.434, DireccionLuz, ColorLuz, FA);
		Arbusto1->Draw(camara->vista, camara->proyeccion, camara->posCam, 71.095, 12, -85.362, DireccionLuz, ColorLuz, FA);

		Arbusto2->Draw(camara->vista, camara->proyeccion, camara->posCam, -94.687, 11, 1.748, DireccionLuz, ColorLuz, FA);
		Arbusto2->Draw(camara->vista, camara->proyeccion, camara->posCam, -120.73, 11, -45.806, DireccionLuz, ColorLuz, FA);
		Arbusto2->Draw(camara->vista, camara->proyeccion, camara->posCam, 15.104, 12, 110.103, DireccionLuz, ColorLuz, FA);
		Arbusto2->Draw(camara->vista, camara->proyeccion, camara->posCam, 35.908, 13, 146.632, DireccionLuz, ColorLuz, FA);
		Arbusto2->Draw(camara->vista, camara->proyeccion, camara->posCam, -108.363, 11, 75.124, DireccionLuz, ColorLuz, FA);

		Arbusto3->Draw(camara->vista, camara->proyeccion, camara->posCam, -124.094, 12.5, -12.094, DireccionLuz, ColorLuz, FA);
		Arbusto3->Draw(camara->vista, camara->proyeccion, camara->posCam, -179.328, 24, -66.757, DireccionLuz, ColorLuz, FA);
		Arbusto3->Draw(camara->vista, camara->proyeccion, camara->posCam, -104.398, 12, 115.003, DireccionLuz, ColorLuz, FA);
		Arbusto3->Draw(camara->vista, camara->proyeccion, camara->posCam, -129.379, 12.5, 71.725, DireccionLuz, ColorLuz, FA);
		Arbusto3->Draw(camara->vista, camara->proyeccion, camara->posCam, 21.549, 13, 136.258, DireccionLuz, ColorLuz, FA);

		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam, 71.37, 12, -37.081, DireccionLuz, ColorLuz, FA);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam, 41.615, 12, -47.883, DireccionLuz, ColorLuz, FA);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam, 13.309, 12, -63.302, DireccionLuz, ColorLuz, FA);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam, 80.544, 12, -86.881, DireccionLuz, ColorLuz, FA);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam, 1.683, 12, -97.425, DireccionLuz, ColorLuz, FA);

		
		//TurnOffAlphaBlending();
#pragma endregion
		
#pragma region Modelos
		avion->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		house01->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		house02->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		house03->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		cottage->Draw(camara->vista, camara->proyeccion, DireccionLuz, ColorLuz, FA);
		horse->Draw(camara->vista, camara->proyeccion, 5, 4.7, 68.319, 17, 27.563, DireccionLuz, ColorLuz, FA);
		horse->Draw(camara->vista, camara->proyeccion, 9, 4.7, 75.134, 17, 49.339, DireccionLuz, ColorLuz, FA);
		horse->Draw(camara->vista, camara->proyeccion, 1, 4.7, 102.66, 17, 10.389, DireccionLuz, ColorLuz, FA);
		cow->Draw(camara->vista, camara->proyeccion, 5, 0, 60.067, 16, -37.710, DireccionLuz, ColorLuz, FA);
		cow->Draw(camara->vista, camara->proyeccion, 1, 0, 88.58, 16, -75.61, DireccionLuz, ColorLuz, FA);
		cow->Draw(camara->vista, camara->proyeccion, 9, 0, 50.138, 16, -59.129, DireccionLuz, ColorLuz, FA);
		cow->Draw(camara->vista, camara->proyeccion, 2, 0, 15.57, 16, -98.50, DireccionLuz, ColorLuz, FA);
		cow->Draw(camara->vista, camara->proyeccion, 6, 0, 7.41, 16, -54.52, DireccionLuz, ColorLuz, FA);
		agua->Draw(camara->vista, camara->proyeccion, 4.8, 0, 150, 5, 0, DireccionLuz, ColorLuz, FA);
#pragma endregion
		
				
		swapChain->Present( 1, 0 );
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}


	////////////////////////////////////////////////////
	void Posicion() {
		float campos[3] = { camara->posCam[0], camara->posCam[1], camara->posCam[2] };

	}
};
#endif