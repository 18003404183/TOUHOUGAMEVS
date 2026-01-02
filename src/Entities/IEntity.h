#pragma once
#include"includes.h"
class IEntity
{
protected:
	glm::vec2 position{0,0};
	glm::vec2 velocity{0,0};
	bool active = true;

public:
	virtual bool isActive() const = 0;

	virtual ~IEntity() = default;
	virtual glm::vec2 get_position() const = 0;
	virtual void set_position(const glm::vec2& newPosition) = 0;
	virtual glm::vec2 get_velocity() const = 0;
	virtual void set_velocity(const glm::vec2& newVelocity) = 0;

};

