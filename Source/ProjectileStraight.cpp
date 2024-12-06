#include "ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager) {
	model = new Model("Data/Model/Sword/Sword.mdl");
	scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight() { 
	delete model; 
}

void ProjectileStraight::Update(float elapsedTime) {
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0) { Destroy(); return; }

	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
	UpdateTransform();
	model->UpdateTransform();
}

void ProjectileStraight::Render(const RenderContext& rc, ModelRenderer* renderer) {
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position) {
	this->direction = direction;
	this->position	= position;
}