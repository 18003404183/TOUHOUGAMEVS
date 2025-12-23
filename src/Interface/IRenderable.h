#pragma once

class IRenderable {
public:

	virtual ~IRenderable() = default;
	virtual void render() const = 0;

protected:
	//优先级 优先级高的后绘制(显示在最上层)
	float render_priority;
};