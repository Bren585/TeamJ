#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"

class Enemy : public Character {
public:
	Enemy() {}
	~Enemy() override {}

	virtual void Update(float elapsedTime) = 0;
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) = 0;
	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) { Character::RenderDebugPrimitive(rc, renderer); }
	void Destroy();
};