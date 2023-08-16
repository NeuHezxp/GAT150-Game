#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"
#include "Renderer/texture.h"

namespace kiko
{
	class ModelRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(ModelRenderComponent)
		void Update(float dt);
		void Draw(class Renderer& renderer) override;
		virtual float GetRadius() override { return m_model->GetRadius(); }
	public:
		res_t<Model> m_model;
	};
}
