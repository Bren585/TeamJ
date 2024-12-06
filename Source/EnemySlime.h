#pragma once

#include "System/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"
#include "Effect.h"

class EnemySlime : public Enemy {
public:
	EnemySlime();
	~EnemySlime() override;

	void Update(float elapsedTime) override;
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) override;

	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);


protected:
	void OnDead() override { DirectX::XMFLOAT3 p(position); p.y += height / 2.0f; deathEffect->Play(p); Destroy(); }

private:
	enum class State {
		Wander,
		Idle,
		Attack,
	}					state				= State::Wander;
	Model*				model				= nullptr;
	DirectX::XMFLOAT3	targetPosition		= { 0, 0, 0 };
	DirectX::XMFLOAT3   territoryOrigin		= { 0, 0, 0 };
	float				territoryRange		= 10.0f;
	float				moveSpeed			= 2.0f;
	float				turnSpeed			= DirectX::XMConvertToRadians(360);
	float				stateTimer			= 0.0f;
	float				searchRange			= 5.0f;
	ProjectileManager   projectileManager;
	Effect*				deathEffect			= nullptr;

	void SetRandomTargetPosition();
	void MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate);

	void SetWanderState();
	void UpdateWanderState(float elapsedTime);

	void SetIdleState();
	void UpdateIdleState(float elapsedTime);

	bool SearchPlayer();
	void SetAttackState();
	void UpdateAttackState(float elapsedTime);
};