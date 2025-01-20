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
	ProjectileManager	projectileManager;
	Effect*				hitEffect			= nullptr;
	AudioSource*		hitSE				= nullptr;

	float moveSpeed = 5.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 12.0f;
	float gravity	= -30.0f;	
	int   jumpCount = 0;
	int   jumpLimit = 2;

	DirectX::XMFLOAT3 GetMoveVec() const;

	void InputMove(float elapsedTime);
	void InputJump();
	void InputProjectile();

	void OnLanding() override;

	void CollisionPlayerVsEnemies();
	void CollisionProjectilesVsEnemies();

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
	
};