#include "Character.h"

void Character::Move(float elapsedTime, float vx, float vz, float speed) {
	moveVecX = vx;
	moveVecZ = vz;
	maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;

	float vlength = sqrtf(vx * vx + vz * vz);
	if (vlength < 0.001f) return;

	vx /= vlength;
	vz /= vlength;

	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	float dot = (vx * frontX) + (vz * frontZ);

	float rot = 1.0f - dot;
	if (rot > speed) { rot = speed; }

	float cross = (frontZ * vx) - (frontX * vz);

	if (cross < 0.0f) {
		angle.y -= rot;
	}
	else {
		angle.y += rot;
	}

	if (angle.y > DirectX::XM_PI)	{ angle.y -= DirectX::XM_PI * 2; }
	if (angle.y < -DirectX::XM_PI)	{ angle.y += DirectX::XM_PI * 2; }
}

void Character::Jump(float speed) {
	velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime) {
	UpdateVerticalVelocity		(elapsedTime);
	UpdateHorizontalVelocity	(elapsedTime);
	UpdateVerticalMove			(elapsedTime);
	UpdateHorizontalMove		(elapsedTime);
}

void Character::UpdateVerticalVelocity(float elapsedTime) {
	velocity.y += gravity * elapsedTime;
}

void Character::UpdateVerticalMove(float elapsedTime) {
	position.y += velocity.y * elapsedTime;

	//if (position.y < 0.0f) {
	//	position.y = 0.0f;
	//	velocity.y = 0.0f;

	//	if (!isGround) {
	//		OnLanding();
	//	}
	//	isGround = true;
	//}
	//else {
	//	isGround = false;
	//}
}

void Character::Ground() {
	if (!isGround) OnLanding();
	isGround = true;
	if (fabsf(velocity.y) < 0.01f) { velocity.y = 0; }
	else { velocity.y = -velocity.y * 0.01f; }

}

void Character::UpdateHorizontalVelocity(float elapsedTime) {
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f) {
		float friction = this->friction * elapsedTime;

		if (!isGround) { friction *= airControl; }

		if (length > friction) {
			float vx = velocity.x / length;
			float vz = velocity.z / length;
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
			
		}
		else {
			velocity.x = 0;
			velocity.z = 0;
		}
	}

	if (length <= maxMoveSpeed) {
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f) {
			float acceleration = this->acceleration * elapsedTime;

			if (!isGround) { acceleration *= airControl; }

			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed) {
				velocity.x *= maxMoveSpeed / length;
				velocity.z *= maxMoveSpeed / length;
			}
		}
	}
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime) {
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;
}

void Character::UpdateTransform() {
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Character::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Character::AddImpulse(DirectX::XMFLOAT3& impulse) {
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

bool Character::ApplyDamage(int damage, float invincibleTime) {
	if (damage < 0)				return false;
	if (health <= 0)			return false;
	if (invincibleTimer > 0.0f) return false;

	health			-= damage;
	invincibleTimer += invincibleTime;

	if (health <= 0)	OnDead();
	else				OnDamage();

	return true;
}