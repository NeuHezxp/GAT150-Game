#include "SpriteComponent.h"

#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(SpriteComponent)


	bool SpriteComponent::Initialize()
	{
		m_texture = GET_RESOURCE(Texture, textureName, g_renderer);
		return true;
	}

	void SpriteComponent::Update(float dt)
	{
	}
	///old
	/*void SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(),m_owner->transform.position.x, m_owner->transform.position.y, RadiansToDegrees(m_owner->transform.rotation));
	}*/
	void kiko::SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), m_owner->transform);
	}
	void SpriteComponent::Read(const json_t& value)
	{
		READ_DATA(value, textureName);
	}

}