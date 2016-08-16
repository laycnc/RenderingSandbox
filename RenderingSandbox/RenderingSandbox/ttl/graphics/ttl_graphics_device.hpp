#pragma once

#include <cassert>
#include <cstdint>
#include <d3d11.h>

namespace ttl::ghi
{
	class RenderBufferDep
	{
		friend class RenderBuffer;
		friend class VideoDeviceDep;
		friend class VideoContextDep;

	public:
		bool IsValid() const;

		inline uint32_t GetWidth() { return m_Param.width; }
		inline uint32_t GetHeight() { return m_Param.height; }

		// Getter
		ID3D11RenderTargetView* GetTarget() { return m_pTarget; }
		const ID3D11RenderTargetView* GetTarget() const { return m_pTarget; }
		ID3D11Texture2D* GetTexture() { return m_pTexture; }
		const ID3D11Texture2D* GetTexture() const { return m_pTexture; }
		ID3D11ShaderResourceView* GetResourceView() { return m_pResourceView; }
		const ID3D11ShaderResourceView* GetResourceView() const { return m_pResourceView; }

		static bool CreateShaderResourceView(
			ID3D11Device* pD11Dev, ID3D11Resource* pRes, const D3D11_TEXTURE2D_DESC& desc, uint32_t multisampleCount,
			ID3D11ShaderResourceView** ppView);

	private:
		RenderBufferDep()
			: m_pSwapChain(nullptr)
			, m_pTarget(nullptr)
			, m_pTexture(nullptr)
			, m_pResourceView(nullptr)
		{}
		RenderBufferDep(VideoDevice* pDevice, const RenderBufferParam& param)
			: m_pSwapChain(nullptr)
			, m_pTarget(nullptr)
			, m_pTexture(nullptr)
			, m_pResourceView(nullptr)
		{
			const bool isInit = Initialize(allocator, pDevice, param);
			assert(isInit != false);
		}
		virtual ~RenderBufferDep()
		{
			Destroy();
		}

		bool Initialize(VideoDevice* pDevice, const RenderBufferParam& param);
		void Destroy();

		bool CreateSwapChain(ID3D11Device* pD11Dev);
		bool CreateRenderTexture(ID3D11Device* pD11Dev);
		bool CreateRenderTarget(ID3D11Device* pD11Dev);

	private:
		RenderBufferParam			m_Param;

		IDXGISwapChain*				m_pSwapChain;
		ID3D11RenderTargetView*		m_pTarget;
		ID3D11Texture2D*			m_pTexture;
		ID3D11ShaderResourceView*	m_pResourceView;
	};

	struct RenderBufferParam
	{
		enum BufferType
		{
			TYPE_NEW,					//!< �V�K�쐬
			TYPE_NEW_SWAP_CHAIN,		//!< �V�K�X���b�v�`�F�C��
			TYPE_EXIST,					//!< �����̃o�b�t�@���g�p����
		};

		uint32_t				width, height;		//!< ���ƍ���
		DXGI_SWAP_CHAIN_DESC	swapDesc;			//!< �X���b�v�`�F�C�����
		DXGI_FORMAT				format;				//!< �t�H�[�}�b�g
		BufferType				type;				//!< �o�b�t�@�̍쐬�^�C�v
		RenderBuffer*			pSource;			//!< �����̃����_�[�o�b�t�@
		uint32_t				multisampleCount;	//!< �}���`�T���v�����O���s���ꍇ��1���傫�Ȓl����͂���
		uint32_t				multisampleQuality;	//!< �}���`�T���v�����O�̃N�I���e�B
		uint32_t				arraySize;			//!< �o�b�t�@�z��̐�

		RenderBufferParam();
		RenderBufferParam(uint32_t w, uint32_t h);
		RenderBufferParam(uint32_t w, uint32_t h, HWND hWnd, BOOL bWindowed);
	};

	struct VideoParam
	{
		IDXGIAdapter*		pAdapter;
		D3D_DRIVER_TYPE		driverType;
		HMODULE				hSoftware;
		UINT				flags;
		uint32_t			numScreens;
		RenderBufferParam*	pRenderParams;
		DepthBufferParam*	pDepthParams;

		VideoParam()
			: pAdapter(nullptr)
			, driverType(D3D_DRIVER_TYPE_HARDWARE)
			, hSoftware(nullptr)
#if defined(_DEBUG)
			, flags(D3D11_CREATE_DEVICE_DEBUG)
#else
			, flags(0)
#endif
			, numScreens(1)
			, pRenderParams(NULL), pDepthParams(NULL)
		{}
	};

	class VideoDeviceDep
	{
		friend class VideoDevice;

	public:
		bool IsValid() const;

		ID3D11Device* GetD3DDevice() { return m_pDevice; }
		VideoContext* GetContext() { return m_pContext; }
		ID3D11DeviceContext* GetD3DContext();
		FrameBuffer*  GetFrameBuffer(uint32_t screenNo);
		RenderBuffer* GetRenderBuffer(uint32_t screenNo);
		DepthBuffer*  GetDepthBuffer(uint32_t screenNo);

	private:
		VideoDeviceDep(const VideoParam& param)
			: m_pAllocator(&allocator)
			, m_Param(param)
			, m_pDevice(NULL)
			, m_pContext(NULL)
			, m_pFrameBuffers(NULL)
			, m_CurrentScene(-1)
		{
			bool ret = Initialize();
			MSL_ASSERT(ret);
		}
		virtual ~VideoDeviceDep()
		{
			Destroy();
		}

		bool Initialize();
		void Destroy();

		// VideoDevice����Ăяo����郁�\�b�h
		void Clear(uint32_t screenNo);
		void BeginScene(uint32_t screenNo);
		void EndScene(uint32_t screenNo);
		void SwapBuffer(uint32_t screenNo);
		void SetDefaultFrameBuffer(uint32_t screenNo);

	private:
		HeapAllocator*	m_pAllocator;
		VideoParam		m_Param;

		ID3D11Device*		m_pDevice;
		D3D_FEATURE_LEVEL	m_FeatureLevel;

		VideoContext*	m_pContext;
		FrameBuffer*	m_pFrameBuffers;

		int		m_CurrentScene;
	};
}