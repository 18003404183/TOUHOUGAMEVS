# pragma once

#include<unordered_map>
#include<functional>
#include"Event.h"
#include<vector>

using EventCallback = std::function<void(const Event&)>;

class EventManager{
public:
    static EventManager* get_instance(){
        if(!eventmanager){
            eventmanager = new EventManager;
        }
        return eventmanager;
    }

    void subscribe(EventType type,EventCallback callback){
        linsteners[type].push_back(std::move(callback));
    }
    
    void publish(const Event& event){
        auto it = linsteners.find(event.type);
        if(it != linsteners.end()){
            for(auto& callback : it->second){
                callback(event);
            }
        }
    }

private:
    
    EventManager() = default;

    static EventManager* eventmanager;

    std::unordered_map<EventType,std::vector<EventCallback>> linsteners;

};