#pragma once

#include "Enemy.h"
#include <vector>
#include <set>
using std::vector;
using std::set;

class EnemyManager {
private:
	EnemyManager() {}
	~EnemyManager() {}

	vector<Enemy*> enemies;
	set<Enemy*> removes;

public:
	static EnemyManager& Instance() {
		static EnemyManager instance;
		return instance;
	}

	int		GetEnemyCount() const	{ return static_cast<int>(enemies.size()); }
	Enemy*	GetEnemy(int i)			{ return enemies.at(i); }

	void Update(float elapsedTime);
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	void CollisionEnemyVsEnemies();

	void Register(Enemy* enemy) { enemies.emplace_back(enemy); }
	void Remove(Enemy* enemy)	{ removes.insert(enemy); }
	void Clear();
};