#pragma once

#include <ttl/file/ttl_resources.hpp>
#include <cassert>
#include <array>
#include <d3d10.h>
#include <d3d11.h>

namespace ttl::ghi
{

	class shader_resources
		: public file::resources<byte_t>
	{
		using file::resources<byte_t>::resources;
	};



	template <typename T>
	class shader
	{
		inline bool _initialize(ID3D11Device* pDev, shader_resources& resShader, ID3D11VertexShader*& _shader, ID3D11ClassLinkage* pLinkage)
		{
			const HRESULT hr = pDev->CreateVertexShader(resShader.data(), resShader.size(), pLinkage, &_shader);
			if (FAILED(hr))
			{
				return false;
			}
			m_resource = resShader;
			return true;
		}
		inline bool _initialize(ID3D11Device* pDev, shader_resources& resShader, ID3D11PixelShader*& _shader, ID3D11ClassLinkage* pLinkage)
		{
			const HRESULT hr = pDev->CreatePixelShader(resShader.data(), resShader.size(), pLinkage, &_shader);
			if (FAILED(hr))
			{
				return false;
			}
			m_resource = resShader;
			return true;
		}
		inline bool _initialize(ID3D11Device* pDev, shader_resources& resShader, ID3D11GeometryShader*& _shader, ID3D11ClassLinkage* pLinkage)
		{
			const HRESULT hr = pDev->CreateGeometryShader(resShader.data(), resShader.size(), pLinkage, &_shader);
			if (FAILED(hr))
			{
				return false;
			}
			m_resource = resShader;
			return true;
		}
		inline bool _initialize(ID3D11Device* pDev, shader_resources& resShader, ID3D11ComputeShader*& _shader, ID3D11ClassLinkage* pLinkage)
		{
			const HRESULT hr = pDev->CreateComputeShader(resShader.data(), resShader.size(), pLinkage, &_shader);
			if (FAILED(hr))
			{
				return false;
			}
			m_resource = resShader;
			return true;
		}
	private:

		template<class U>
		inline bool _createLayout(std::false_type, ...)
		{
			assert("[%s] Unimplemented method.\n", __FUNCTION__);
			return false;
		}

		template<class U>
		inline bool _createLayout(std::true_type, ID3D11Device* pDev, const D3D11_INPUT_ELEMENT_DESC* layout, uint32_t numDescs, ID3D11InputLayout** ppLayout)
		{
			if (!is_valid())
			{
				return false;
			}
			const HRESULT	hr = pDev->CreateInputLayout(layout, numDescs, this->m_resource.data(), this->m_resource.size(), ppLayout);
			if (FAILED(hr))
			{
				return false;
			}
			return true;
		}
	public:
		constexpr shader() noexcept = default;

		shader(ID3D11Device* pDev, shader_resources& resShader)
		{
			const bool succeeded = this->initialize(pDev, resShader, nullptr);
			assert(succeeded);
		}
		~shader()
		{
			this->release();
		}

		void release()
		{
			SafeRelease(m_shader);
		}
	
		bool initialize(ID3D11Device* pDev, shader_resources& resShader, ID3D11ClassLinkage* pLinkage)
		{
			return this->_initialize(pDev, resShader, m_shader, pLinkage);
		}

		bool createLayout(ID3D11Device* pDev, const D3D11_INPUT_ELEMENT_DESC* layout, uint32_t numDescs, ID3D11InputLayout** ppLayout)
		{
			return _createLayout(std::is_same<ID3D11VertexShader, T>(), pDev, layout, numDescs, ppLayout);
		}

		constexpr bool is_valid() const noexcept 
		{
			return this->m_shader != nullptr; 
		}

		T* get_shader() noexcept
		{
			return m_shader; 
		}
		constexpr const T* get_shader() const noexcept
		{
			return m_shader; 
		}
		const shader_resources& get_resource() const noexcept
		{
			return m_resource;
		}

	private:
		
		T*					m_shader = nullptr;
		shader_resources	m_resource = {};
	};

	using vertex_shader = shader<ID3D11VertexShader>;
	using pixel_shader = shader<ID3D11PixelShader>;
	using geometry_shader = shader<ID3D11GeometryShader>;
	using compute_shader = shader<ID3D11ComputeShader>;


}