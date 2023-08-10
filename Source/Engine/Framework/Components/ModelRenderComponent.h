#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"
#include "Renderer/texture.h"

namespace kiko
{
	class ModelRenderComponent : public RenderComponent
	{
	public:
		void Update(float dt);
		void Draw(class Renderer& renderer) override;
	public:
		res_t<Model> m_model;
	};

	
}
