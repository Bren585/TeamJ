#include "Player.h"
#include "System\Audio.h"
#include "Collision.h"

void Player::Initialize() {
	model = new Model("Data/Model/GolfBall.mdl");
	arrow = new Model("Data/Model/Arrow.mdl");
	//hitEffect = new Effect("Data/Effect/Hit.efk");
	hitSE[0] = Audio::Instance().LoadAudioSource("Data/Sound/putt_1.wav");
	hitSE[1] = Audio::Instance().LoadAudioSource("Data/Sound/putt_2.wav");
	hitSE[2] = Audio::Instance().LoadAudioSource("Data/Sound/putt_3.wav");
	hitSE[3] = Audio::Instance().LoadAudioSource("Data/Sound/putt_4.wav");

	scale.x = scale.y = scale.z = 0.01f;
	position.x = 0; 
	position.y = 0;
	position.z = 0;
	radius = 1;
	hitCount = 0;
}

void Player::Finalize() {
	delete model;
	delete arrow;
	//delete hitEffect;
	for (int i = 0; i < 4; i++) delete hitSE[i];
}

void Player::Update(float elapsedTime) {
	InputMove(elapsedTime);
	InputJump();
	UpdateArrow(elapsedTime);
	//InputProjectile();
	UpdateVelocity(elapsedTime);
	//projectileManager.Update(elapsedTime);
	CollisionPlayerVsEnemies();
	CollisionProjectilesVsEnemies();
	//UpdateTransform();
	UpdateRotate(elapsedTime);
	model->UpdateTransform();
}

void Player::UpdateArrow(float elapsedTime) {

	switch (arrowState) {
	case arrowInactive:
		if (fabsf(velocity.x) < 0.01f && fabsf(velocity.y) < 0.01f && fabsf(velocity.z) < 0.01f && hitCount < hitLimit) {
			cooldown -= elapsedTime;
			if (cooldown < 0.0f) {
				arrowState = arrowRotate;
				r_direction = !r_direction;
			}
		}
		break;
	case arrowRotate:
		arrowAngle += elapsedTime * arrowRotateSpeed * (r_direction ? -1 : 1);
		break;
	case arrowCharge:
		charge += elapsedTime * delta_charge * (c_direction ? 1 : -1);
		if (charge > max_charge) {
			charge = max_charge;
			c_direction = false;
		}
		if (charge < min_charge) {
			charge = min_charge;
			c_direction = true;
		}
		break;
	}

}

void Player::UpdateRotate(float elapsedTime) {
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

	DirectX::XMMATRIX R;

	if (fabsf(length) > 0.01f)
	{ 
		DirectX::XMVECTOR Velocity = { velocity.x / length, velocity.y / length, velocity.z / length };
		DirectX::XMVECTOR Vertical = { 0.0f, 1.0f, 0.0f };
		DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Velocity, Vertical);

		float dist = length * elapsedTime;

		R = DirectX::XMMatrixRotationAxis(Axis, length / (radius/* * DirectX::XM_2PI*/));
	}
	else {
		R = DirectX::XMMatrixIdentity();
	}

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	DirectX::XMMATRIX W = S * R * T;

	DirectX::XMStoreFloat4x4(&transform, W);
}

void Player::Render(const RenderContext& rc, ModelRenderer* renderer) {
	renderer->Render(rc, transform, model, ShaderId::Lambert);
	//projectileManager.Render(rc, renderer);

	if (arrowState == arrowInactive) return;

	float chargeScale = 1.0f;
	if (arrowState == arrowCharge) chargeScale += charge;

	DirectX::XMFLOAT4X4 arrowTranform;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.01f * chargeScale, 0.01f, 0.01f);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, -arrowAngle - DirectX::XM_PI, 0);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x + cos(arrowAngle) * arrowDistance, position.y, position.z + sin(arrowAngle) * arrowDistance);

	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&arrowTranform, W);

	renderer->Render(rc, arrowTranform, arrow, ShaderId::Lambert);
}

void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
	//Character::RenderDebugPrimitive(rc, renderer);
	renderer->RenderSphere(rc, position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.RenderDebugPrimitive(rc, renderer);
}

void Player::DrawDebugGUI() {
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None)) {
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::InputFloat3("Position", &position.x);

			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);

			ImGui::InputFloat3("Scale", &scale.x);

			ImGui::InputFloat3("Velocity", &velocity.x);
		}
	}
	ImGui::End();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const {
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0) {
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0) {
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	DirectX::XMFLOAT3 vec;
	vec.x = ax * cameraRightX + ay * cameraFrontX;
	vec.y = 0;
	vec.z = ax * cameraRightZ + ay * cameraFrontZ;

	return vec;
}

void Player::InputMove(float elapsedTime) {
	//DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	//Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A) {
		switch (arrowState) {
		case arrowInactive:
			break;
		case arrowRotate:
			arrowState = arrowCharge;
			charge = 0;
			c_direction = true;
			break;
		case arrowCharge:
			DirectX::XMFLOAT3 impulse{cos(arrowAngle) * hit_strength * (0.5f + charge), 0, sin(arrowAngle) * hit_strength * (0.5f + charge)};
			AddImpulse(impulse);
			arrowState = arrowInactive;
			hitCount++;
			hitSE[rand() % 4]->Play(false);
			break;
		}
	}
}

void Player::InputJump() {
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A) { 
		if (jumpCount < jumpLimit) {
			jumpCount++;
			Jump(jumpSpeed);
		} 
	}
}

void Player::InputProjectile() {
	return;
	GamePad& gamePad = Input::Instance().GetGamePad();

	//if (gamePad.GetButtonDown() & GamePad::BTN_X) {
	//	DirectX::XMFLOAT3 dir;
	//	dir.x = sinf(angle.y);
	//	dir.y = 0;
	//	dir.z = cosf(angle.y);
	//
	//	DirectX::XMFLOAT3 pos;
	//	pos.x = position.x;
	//	pos.y = position.y + 1.0f;
	//	pos.z = position.z;
	//
	//	ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
	//	projectile->Launch(dir, pos);
	//}
	//if (gamePad.GetButtonDown() & GamePad::BTN_Y) {
	//	DirectX::XMFLOAT3 dir;
	//	dir.x = sinf(angle.y);
	//	dir.y = 0;
	//	dir.z = cosf(angle.y);
	//
	//	DirectX::XMFLOAT3 pos;
	//	pos.x = position.x;
	//	pos.y = position.y + 1.0f;
	//	pos.z = position.z;
	//
	//	DirectX::XMFLOAT3 target;
	//	target.x = pos.x + dir.x * 1000.0f;
	//	target.y = pos.y + dir.y * 1000.0f;;
	//	target.z = pos.z + dir.z * 1000.0f;
	//
	//	float dist = FLT_MAX;
	//	EnemyManager& enemyManager = EnemyManager::Instance();
	//	int enemyCount = enemyManager.GetEnemyCount();
	//	for (int i = 0; i < enemyCount; i++) {
	//		Enemy* enemy = enemyManager.GetEnemy(i);
	//
	//		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos);
	//		DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
	//		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
	//		DirectX::XMVECTOR D = DirectX::XMVector3Length(V);
	//		float distance;
	//		DirectX::XMStoreFloat(&distance, D);
	//
	//		if (distance < dist) {
	//			dist	= distance;
	//			target	= enemy->GetPosition();
	//			target.y += enemy->GetHeight() * 0.5f;
	//		}
	//	}
	//
	//	ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
	//	projectile->Launch(dir, pos, target);
	//}
}

void Player::OnLanding() {
	jumpCount = 0; 
}

void Player::CollisionPlayerVsEnemies() {
	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; i++) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			&outPosition
		)) {
			DirectX::XMVECTOR playerPos			= DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR enemyPos			= DirectX::XMLoadFloat3(&(enemy->GetPosition()));
			DirectX::XMVECTOR angleOfIncident	= DirectX::XMVectorSubtract(playerPos, enemyPos);
			DirectX::XMFLOAT3 AngleOfIncident;
			DirectX::XMStoreFloat3(&AngleOfIncident, DirectX::XMVector3Normalize(angleOfIncident));

			if (AngleOfIncident.y > 0.8f) {
				Jump(jumpSpeed / 1.5f);
			}
			else {
				enemy->SetPosition(outPosition);
			}
		}
	}
}

void Player::CollisionProjectilesVsEnemies() {
	return;
	EnemyManager& enemyManager = EnemyManager::Instance();

	//int projectileCount = projectileManager.GetProjectileCount();
	//int enemyCount = enemyManager.GetEnemyCount();
	//
	//for (int p = 0; p < projectileCount; p++) {
	//	Projectile* projectile = projectileManager.GetProjectile(p);
	//	for (int e = 0; e < enemyCount; e++) {
	//		Enemy* enemy = enemyManager.GetEnemy(e);
	//
	//		DirectX::XMFLOAT3 outPosition;
	//		if (Collision::IntersectSphereVsCylinder(
	//			projectile->GetPosition(),
	//			projectile->GetRadius(),
	//			enemy->GetPosition(),
	//			enemy->GetRadius(),
	//			enemy->GetHeight(),
	//			&outPosition
	//		)) { 
	//			if (enemy->ApplyDamage(1, 0.5f)) { 
	//				DirectX::XMFLOAT3 impulse;
	//				DirectX::XMFLOAT3 ppos = projectile->GetPosition();
	//				DirectX::XMFLOAT3 epos = enemy->GetPosition();
	//				impulse.x = (epos.x - ppos.x) * 5.0f;
	//				impulse.y = 5.0f;
	//				impulse.z = (epos.z - ppos.z) * 5.0f;
	//				enemy->AddImpulse(impulse);
	//
	//				DirectX::XMFLOAT3 e = enemy->GetPosition();
	//				e.y += enemy->GetHeight() * 0.5f;
	//				hitEffect->Play(e);
	//				hitSE->Play(false);
	//
	//				projectile->Destroy(); 
	//			}
	//		}
	//	}
	//}
}