#include "EnemyManager.h"

void EnemyManager::Update(float elapsedTime) {
	for (Enemy* enemy : enemies) {
		enemy->Update(elapsedTime);
	}

	for (Enemy* enemy : removes) {
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end()) { enemies.erase(it); }
		delete enemy;
	}
	removes.clear();

	CollisionEnemyVsEnemies();
}

void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer) {
	for (Enemy* enemy : enemies) {
		enemy->Render(rc, renderer);
	}
}

void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
	for (Enemy* enemy : enemies) {
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

void EnemyManager::CollisionEnemyVsEnemies() {
	for (Enemy* enemyA : enemies) {
		for (Enemy* enemyB : enemies) {
			if (enemyA == enemyB) { continue; }
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectCylinderVsCylinder(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPosition
			)) {
				enemyB->SetPosition(outPosition);
			}
		}
	}
}

void EnemyManager::Clear() {
	for (Enemy* enemy : enemies) {
		if (enemy != nullptr) delete enemy;
	}
	enemies.clear();
}