#pragma once

#include <imgui.h>

#include "System/ModelRenderer.h"
#include "System/Input.h"
#include "System/AudioSource.h"

#include "Character.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "Effect.h"

class Player : public Character {
private:
	Model*				model				= nullptr;
	Model*				arrow               = nullptr;
	//ProjectileManager	projectileManager;
	//Effect*				hitEffect			= nullptr;

	AudioSource* hitSE[4] = { nullptr, nullptr, nullptr, nullptr };

	float moveSpeed = 0.0f;
	float turnSpeed = 0;
	float jumpSpeed = 0.0f;
	float gravity	= -30.0f;	
	int   jumpCount = 0;
	int   jumpLimit = 0;

	int hitCount = 0;
	int hitLimit = 10;

	// Arrow
	enum { arrowInactive, arrowRotate, arrowCharge } arrowState = arrowInactive;

	float arrowAngle = 0.0f;
	float arrowDistance = 2.0f;
	float arrowRotateSpeed = DirectX::XM_2PI;
	bool r_direction = true;
	
	float charge = 0.0f;
	float min_charge = 0.0f;
	float max_charge = 1.0f;
	float delta_charge = 1.0f;
	bool c_direction = true;

	static constexpr float hit_strength = 15.0f;

	float cooldown = 1.0f;
	float max_cooldown = 1.0f;

	DirectX::XMFLOAT3 GetMoveVec() const;

	void InputMove(float elapsedTime);
	void InputJump();
	void InputProjectile();

	void OnLanding() override;

	void CollisionPlayerVsEnemies();
	void CollisionProjectilesVsEnemies();

	void UpdateArrow(float elapsedTime);
	void UpdateRotate(float elapsedTime);

	Player() {}
	~Player() override {}
public:
	static Player& Instance() { static Player instance; return instance; }

	void Initialize();
	void Finalize();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) override;

	void DrawDebugGUI();

	bool finished() { return hitCount >= hitLimit && (velocity.x + velocity.y + velocity.z < 0.01f); }
	int getHits() { return hitCount; }
	int getLimit() { return hitLimit; }
	
};