#pragma once

#include <DirectXMath.h>
#include "System/ShapeRenderer.h"

class Character {
protected:
	DirectX::XMFLOAT3	position	= { 0, 0, 0 };
	DirectX::XMFLOAT3	velocity	= { 0, 0, 0 };
	DirectX::XMFLOAT3	angle		= { 0, 0, 0 };
	DirectX::XMFLOAT3	scale		= { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform	= {
		 1, 0, 0, 0,
		 0, 1, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1,
	};

	float	gravity			= -30.0f;
	float	friction		= 5.0f;
	float	acceleration	= 50.0f;
	float	maxMoveSpeed	= 5.0f;
	float	airControl		= 0.3f;
	float	moveVecX		= 0.0f;
	float	moveVecZ		= 0.0f;

	float	radius			= 0.5f;
	float	height			= 2.0f;
	bool	isGround		= false;
	int		health			= 5;
	float   invincibleTimer = 1.0f;

	void Move(float elapsedTime, float vx, float vz, float speed);
	void Turn(float elapsedTime, float vx, float vz, float speed);
	void Jump(float speed);

	void UpdateVelocity				(float elapsedTime);
	void UpdateVerticalVelocity		(float elapsedTime);
	void UpdateVerticalMove			(float elapsedTime);
	void UpdateHorizontalVelocity	(float elapsedTime);
	void UpdateHorizontalMove		(float elapsedTime);

	void UpdateInvincibleTimer(float elapsedTime) { if (invincibleTimer > 0.0f) { invincibleTimer -= elapsedTime; } }

	virtual void OnLanding() {}
	virtual void OnDamage() {}
	virtual void OnDead() {}

public:
	Character() {}
	virtual ~Character() {}

	void UpdateTransform();

	const DirectX::XMFLOAT3& GetPosition()	const { return position; }
	const DirectX::XMFLOAT3& GetAngle()		const { return angle; }
	const DirectX::XMFLOAT3& GetScale()		const { return scale; }

	float	GetRadius() const { return radius; }
	float	GetHeight() const { return height; }
	bool	IsGround()	const { return isGround; }

	void SetPosition(DirectX::XMFLOAT3& position) { this->position	= position; }
	void SetAngle	(DirectX::XMFLOAT3& angle	) { this->angle		= angle; }
	void SetScale	(DirectX::XMFLOAT3& scale	) { this->scale		= scale; }

	void AddImpulse(DirectX::XMFLOAT3& impulse);
	void Ground();
	void ResetGround() { isGround = false; }

	bool ApplyDamage(int damage, float invincibleTime);

	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);
};