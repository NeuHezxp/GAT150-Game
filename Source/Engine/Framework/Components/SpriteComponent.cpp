#include "SpriteComponent.h"

#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(SpriteComponent)

		bool SpriteComponent::Initialize()
	{
		if (!textureName.empty()) m_texture = GET_RESOURCE(Texture, textureName, g_renderer);//  if not emp
		if (source.w == 0 && source.h == 0)
		{
			if (m_texture)
			{
				source.x = 0;
				source.y = 0;
				source.w = (int)m_texture->GetSize().x;
				source.h = (int)m_texture->GetSize().y;
			}
		}
		return true;
	}

	void SpriteComponent::Update(float dt)
	{
	}

	/*void SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(),m_owner->transform.position.x, m_owner->transform.position.y, RadiansToDegrees(m_owner->transform.rotation));
	}*/
	void kiko::SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), source, m_owner->transform);

	}

	void SpriteComponent::Read(const json_t& value)
	{
		READ_DATA(value, textureName);
		READ_DATA(value, source);
	}
}