#pragma once

#include <unordered_map>
#include <functional>
#include "Event.h"
#include <vector>

using EventCallback = std::function<void(const Event&)>;

class EventManager
{
public:
    static EventManager* get_instance()
    {
        if (!event_manager_)
        {
            event_manager_ = new EventManager;
        }
        return event_manager_;
    }

    void subscribe(EventType type, EventCallback callback)
    {
        listeners_[type].push_back(std::move(callback));
    }

    void publish(const Event& event)
    {
        auto it = listeners_.find(event.type);
        if (it != listeners_.end())
        {
            for (auto& callback : it->second)
            {
                callback(event);
            }
        }
    }

private:
    EventManager() = default;

    static EventManager* event_manager_;

    std::unordered_map<EventType, std::vector<EventCallback>> listeners_;
};