#include "EnemySlime.h"
#include "MathUtils.h"
#include "Player.h"

EnemySlime::EnemySlime() {
	model = new Model("Data/Model/Slime/Slime.mdl");
	deathEffect = new Effect("Data/Effect/skull.efk");
	scale.x = scale.y = scale.z = 0.01f;
	radius = 0.5f;
	height = 1.0f;
	SetWanderState();
}

EnemySlime::~EnemySlime() {
	delete model;
	delete deathEffect;
}

void EnemySlime::Update(float elapsedTime) {
	switch (state) {
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}

	UpdateVelocity(elapsedTime);
	projectileManager.Update(elapsedTime);
	UpdateInvincibleTimer(elapsedTime);
	UpdateTransform();
	model->UpdateTransform();
}

void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer) {
	renderer->Render(rc, transform, model, ShaderId::Lambert);
	projectileManager.Render(rc, renderer);
}

void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
	Enemy::RenderDebugPrimitive(rc, renderer);

	renderer->RenderCylinder(rc, territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));
	renderer->RenderSphere(rc, targetPosition, 1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	renderer->RenderCylinder(rc, position, searchRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range) {
	territoryOrigin = origin;
	territoryRange = range;
}

void EnemySlime::SetRandomTargetPosition() {
	float theta = MathUtils::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MathUtils::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate) {
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}

void EnemySlime::SetWanderState() {
	state = State::Wander;
	SetRandomTargetPosition();
}

void EnemySlime::UpdateWanderState(float elapsedTime) {
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius) {
		SetIdleState();
	}
	MoveToTarget(elapsedTime, 1.0f, 1.0f);

	if (SearchPlayer()) { SetAttackState(); }
}

void EnemySlime::SetIdleState() {
	state = State::Idle;
	stateTimer = MathUtils::RandomRange(3.0f, 5.0f);
}

void EnemySlime::UpdateIdleState(float elapsedTime) {
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f) { SetWanderState(); }
	if (SearchPlayer()) { SetAttackState(); }
}

bool EnemySlime::SearchPlayer() {
	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange) {
		float distXZ = sqrtf(vx * vx + vz * vz);
		vx /= distXZ;
		vz /= distXZ;
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f) { return true; }
	}
	return false;
}

void EnemySlime::SetAttackState() {
	state = State::Attack;
	stateTimer = 0.0f;
}

void EnemySlime::UpdateAttackState(float elapsedTime) {
	targetPosition = Player::Instance().GetPosition();
	
	MoveToTarget(elapsedTime, 0.0f, 1.0f);

	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f) {
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);

		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y + height * 0.5f;
		pos.z = position.z;

		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		
		stateTimer = 2.0f;
	}

	if (!SearchPlayer()) { SetIdleState(); }
}