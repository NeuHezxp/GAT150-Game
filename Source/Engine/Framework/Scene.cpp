#include "Scene.h"
#include "Actor.h"

namespace kiko
{
    // This function updates the scene by updating all actors in the scene.
    // It iterates through all actors, updates each one, and removes any destroyed actors from the scene.
    // Additionally, it checks for collisions between actors and calls their OnCollision functions if a collision is detected.
    void Scene::Update(float dt)
    {
        auto iter = m_actors.begin();
        while (iter != m_actors.end())
        {
            (*iter)->Update(dt);
            // If an actor is destroyed, remove it from the scene by erasing it from the actors vector.
            // Otherwise, move to the next actor.
            ((*iter)->m_destroyed) ? iter = m_actors.erase(iter) : iter++;

            // Check for collisions between actors
            for (auto iter1 = m_actors.begin(); iter1 != m_actors.end(); iter1++)
            {
                for (auto iter2 = std::next(iter1, 1); iter2 != m_actors.end(); iter2++)
                {
                    float distance = (*iter1)->m_transform.position.Distance((*iter2)->m_transform.position);
                    float radius = (*iter1)->GetRadius() + (*iter2)->GetRadius();

                    if (distance <= radius)
                    {
                        // If a collision is detected, call the OnCollision function for both actors involved in the collision.
                        (*iter1)->OnCollision(iter2->get());
                        (*iter2)->OnCollision(iter1->get());
                        // ... do something for the collision, e.g., boom.
                    }
                }
            }
        }
    }

    // This function draws the scene by calling the Draw function for each actor in the scene.
    void Scene::Draw(Renderer& renderer)
    {
        for (auto& actor : m_actors)
        {
            actor->Draw(renderer);
        }
    }

    // This function adds an actor to the scene by moving it into the m_actors vector.
    void Scene::Add(std::unique_ptr<Actor> actor)
    {
        actor->m_scene = this;
        m_actors.push_back(std::move(actor));
    }

    // This function removes all actors from the scene by clearing the m_actors vector.
    void Scene::RemoveAll()
    {
        m_actors.clear();
    }
}
