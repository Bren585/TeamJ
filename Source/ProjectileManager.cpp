#include "ProjectileManager.h"

ProjectileManager::ProjectileManager() {}

ProjectileManager::~ProjectileManager() { Clear(); }

void ProjectileManager::Update(float elapsedTime) {
	for (Projectile* projectile : projectiles) {
		projectile->Update(elapsedTime);
	}

	for (Projectile* projectile : removes) {
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end()) { projectiles.erase(it); }
		delete projectile;
	}
	removes.clear();
}

void ProjectileManager::Render(const RenderContext& rc, ModelRenderer* renderer) {
	for (Projectile* projectile : projectiles) {
		projectile->Render(rc, renderer);
	}
}

void ProjectileManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
	for (Projectile* projectile : projectiles) {
		projectile->RenderDebugPrimitive(rc, renderer);
	}
}

void ProjectileManager::Register(Projectile* projectile) { projectiles.push_back(projectile); }

void ProjectileManager::Clear() {
	for (Projectile* projectile : projectiles) {
		delete projectile;
	}
	projectiles.clear();
}

void ProjectileManager::Remove(Projectile* projectile) {
	removes.insert(projectile);
}