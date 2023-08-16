#include "SpriteComponent.h"

#include "Framework/Actor.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(SpriteComponent)


	void SpriteComponent::Update(float dt)
	{
	}
	///old
	/*void SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(),m_owner->m_transform.position.x, m_owner->m_transform.position.y, RadiansToDegrees(m_owner->m_transform.rotation));
	}*/
	void kiko::SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), m_owner->m_transform);
	}
}