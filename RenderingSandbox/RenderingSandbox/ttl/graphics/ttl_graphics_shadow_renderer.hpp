#pragma once

#include <array>
#include <ttl/graphics/ttl_graphics_shader.hpp>

namespace ttl::ghi
{


	class shadow_renderer
	{
	public:
		static constexpr size_t SHADOW_WIDTH = 512;

		static constexpr size_t MAX_VSHADER = 2;

	public:
		shadow_renderer();
		~shadow_renderer()
		{
			destroy();
		}

		bool initialize(msl::gu::VideoDevice* pDevice);
		void destroy();

		void before_render(Scene* pScene, msl::gu::VideoDevice* pDevice);
		void set_transform(Scene* pScene, msl::gu::VideoDevice* pDevice, SceneModel* pModel);
		void after_render(Scene* pScene, msl::gu::VideoDevice* pDevice);

	private:
		std::array<shader_resources, MAX_VSHADER>	m_res_vshader;
		shader_resources							m_res_phhader;
		std::array<vertex_shader*, MAX_VSHADER>		m_vshader;
		pixel_shader*		m_pPShader;
		std::array<vertex_layout*, MAX_VSHADER>		m_pVertexLayout;

		D3D11_VIEWPORT	m_OriginalVP;
	};


}
