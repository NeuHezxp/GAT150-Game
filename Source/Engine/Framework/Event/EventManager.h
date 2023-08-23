#pragma once
#include <functional>

#include "Event.h"
#include "Framework/Singleton.h"
#include <map>
#include <list>
#define EVENT_SUBSCRIBE(id, function) kiko::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1))
#define EVENT_UNSUBSCRIBE(id)  kiko::EventManager::Instance().Unsubscribe(id, this);
namespace kiko
{
	class IEventListener
	{
	public:
		
	};
	class EventManager : public Singleton<EventManager>
	{
	public:
	using eventFunction = std::function<void(const Event&)>;

		struct EventDispatcher
		{
			IEventListener* listener = nullptr; // object that can listen for events
			eventFunction function; 
		
		};
	public:
		friend class Singleton<EventManager>;
		void Update(float dt);

		void Subscribe(Event::id_t id, IEventListener* listener, eventFunction function); // add listener
		void Unsubscribe(Event::id_t id, IEventListener* listener); // remove listener
		void DispatchEvent(Event::id_t id, Event::data_t data); 

	private:
		EventManager() = default;
	private:
		std::map<Event::id_t, std::list<EventDispatcher>> m_dispatchers;
	};
	
}
