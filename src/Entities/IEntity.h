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
	virtual glm::vec2 getPosition() const = 0;
	virtual void setPosition(const glm::vec2& newPosition) = 0;
	virtual glm::vec2 getVelocity() const = 0;
	virtual void setVelocity(const glm::vec2& newVelocity) = 0;




};

