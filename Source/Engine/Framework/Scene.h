#pragma once
#include <list>
#include "Actor.h"

namespace kiko //apart of engine gets namespace
{
	class Renderer;

	class Scene
	{
	public:
		Scene() = default;
		bool Initialize();
		void Update(float dt);
		void Draw(Renderer& renderer);

		void Add(std::unique_ptr<Actor> actor);
		void RemoveAll(); // clears scene

		bool Load(const std::string& filename);
		void Read(const json_t& value);

		template<typename T>
		T* GetActor();

		friend class Actor;

	private:
		std::list <std::unique_ptr<Actor>> m_actors; //advatage over list: quick insert and remove certain elements
	};

	template <typename T>
	inline T* Scene::GetActor()
	{
		for (auto& actor : m_actors)
		{
			T* result = dynamic_cast<T*> (actor.get());//getting pointer
			if (result) return result;
		}
		return nullptr;
	}
}
