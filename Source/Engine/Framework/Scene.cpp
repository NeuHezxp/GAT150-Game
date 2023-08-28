#include "Scene.h"
#include "Actor.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko
{
	bool Scene::Initialize()
	{
		for (auto& actor : m_actors) actor->Initialize();

		return true;
	}

	// This function updates the scene by updating all actors in the scene.
	 //It iterates through all actors, updates each one, and removes any destroyed actors from the scene.
	 //Additionally, it checks for collisions between actors and calls their OnCollisionEnter functions if a collision is detected.
	void Scene::Update(float dt)
	{
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			if ((*iter)->active) (*iter)->Update(dt);
			// If an actor is destroyed, remove it from the scene by erasing it from the actors vector.
			// Otherwise, move to the next actor.
			((*iter)->destroyed) ? iter = m_actors.erase(iter) : iter++;

			///Check Collisions
			//check collisions
			/*for (auto iter1 = m_actors.begin(); iter1 != m_actors.end(); iter1++)
			{
				for (auto iter2 = std::next(iter1, 1); iter2 != m_actors.end(); iter2++)
				{
					auto* collision1 = (*iter1)->GetComponent<CollisionComponent>();
					auto* collision2 = (*iter2)->GetComponent<CollisionComponent>();

					if (!collision1 || !collision2) continue;

					if (collision1->CheckCollision(collision2))

					{
						(*iter1)->OnCollisionEnter(iter2->get());
						(*iter2)->OnCollisionEnter(iter1->get());
					}
				}
			}*/
		}
	}

	// This function draws the scene by calling the Draw function for each actor in the scene.
	void Scene::Draw(Renderer& renderer)
	{
		for (auto& actor : m_actors)
		{
			if (actor->active) actor->Draw(renderer);
		}
		
	}
	

	// This function adds an actor to the scene by moving it into the m_actors vector.
	void Scene::Add(std::unique_ptr<Actor> actor)
	{
		actor->m_scene = this;
		m_actors.push_back(std::move(actor));
		
	}

	// This function removes all actors from the scene by clearing the m_actors vector.
	void Scene::RemoveAll(bool force)
	{
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			// If an actor is destroyed, remove it from the scene by erasing it from the actors vector.
			// Otherwise, move to the next actor.
			(force || !(*iter)->persistent) ? iter = m_actors.erase(iter) : iter++;
		}
	}

	bool Scene::Load(const std::string& filename)
	{
		rapidjson::Document document;
		if (!Json::Load(filename, document))
		{
			ERROR_LOG("Could not load scene file: " << filename);
			return false;
		}
		Read(document);
		return true;
	}

	void Scene::Read(const json_t& value)
	{
		if (HAS_DATA(value, actors) && GET_DATA(value, actors).IsArray())
		{
			for (auto& actorValue : GET_DATA(value, actors).GetArray())
			{
				std::string type;
				READ_DATA(actorValue, type);
				auto actor = CREATE_CLASSBASE(Actor, type);
				actor->Read(actorValue);

				if (actor->prototype)
				{
					std::string name = actor->name;
					Factory::Instance().RegisterPrototype(name, std::move(actor));
				}
				else
				{
					Add(std::move(actor));
				}
			}
		}
	}
}