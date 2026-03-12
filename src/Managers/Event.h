#pragma once

enum class EventType{

    PlayerHit,
    PlayerDead,
    EnemyDsetroyed

};

struct Event
{
    EventType type;
    void* data;
};
