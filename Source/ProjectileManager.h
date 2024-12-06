#pragma once

#include <vector>
#include <set>
#include "Projectile.h"

class ProjectileManager {
private:
	std::vector<Projectile*> projectiles;
	std::set<Projectile*> removes;

public:
	ProjectileManager();
	~ProjectileManager();

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	void Register(Projectile* projectile);
	void Clear();
	void Remove(Projectile* projectile);

	int			GetProjectileCount	()			const	{ return static_cast<int>(projectiles.size()); }
	Projectile* GetProjectile		(int index)			{ return projectiles.at(index); }
};