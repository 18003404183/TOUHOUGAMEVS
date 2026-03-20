#pragma once
#include"includes.h"

enum class EntityType{
	Player,
	Enemy,
	Danmaku
};

class IEntity
{
protected:
	glm::vec2 position{0,0};
	glm::vec2 velocity{0,0};
	// 实体是否活跃 不活跃的实体不参与任何运算
	bool active = true;
	//也许每个entity都应该持有自己的type? 方便运行时类型转换
	EntityType entity_type;	

public:
	virtual bool isActive() const = 0;
	virtual EntityType get_entity_type() const {return this->entity_type;};
	virtual ~IEntity() = default;
	virtual glm::vec2 get_position() const = 0;
	virtual glm::vec2* get_position_ptr(){ return &this->position; }
	virtual void set_position(const glm::vec2& newPosition) = 0;
	virtual glm::vec2 get_velocity() const = 0;
	virtual void set_velocity(const glm::vec2& newVelocity) = 0;

};

