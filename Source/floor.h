#pragma once
#pragma once

#include "System/ModelRenderer.h"
#include "System/ShapeRenderer.h"
#include <vector>

using std::vector;

class Floor {
protected:
	Model* model = nullptr;

	float				radius		= 100.0f;
	float				height		= 1.20f;
	DirectX::XMFLOAT3	position	= { 0, -50, 0 };
	DirectX::XMFLOAT3	scale		= { 1, 0.1f, 1 };
	DirectX::XMFLOAT3	angle		= { 0, 0, 0 };
	DirectX::XMFLOAT4X4 transform	= { 1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1 };

	bool active = true;

public:
	virtual ~Floor() { if (model) delete model; }

	void Initialize() {
		model = new Model("Data/Model/Grass.mdl");
	}

	void Activate() { active = true; }
	void Deactivate() { active = false; }

	virtual int CollisionVsSphere(const DirectX::XMFLOAT3& positionS, float radiusS) {
		if (!active) return false;
		if (fabsf(position.y - positionS.y) > radiusS + height / 2.0f) return false;
		DirectX::XMFLOAT2 dist = { position.x - positionS.x, position.z - positionS.z };
		float hyp = dist.x * dist.x + dist.y * dist.y;

		if (hyp < (radius + radiusS) * (radius + radiusS)) {
			return (position.y > positionS.y) ? -1 : 1;
		}
		return false;
	}

	virtual void Update() { UpdateTransform(); }

	void UpdateTransform() {
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMStoreFloat4x4(&transform, W);
	}

	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) {
		renderer->Render(rc, transform, model, ShaderId::Lambert);
	}

	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
		renderer->RenderCylinder(rc, position, radius, height / 2.0f, DirectX::XMFLOAT4(0, 1, 0, 1));
	}

	float getHeight() { return height; }
	DirectX::XMFLOAT3 getPosition() { return position; }

	void setY(float y) { position.y = y; }
	void addY(float y) { position.y += y; }
};

class HolyFloor : public Floor {
private:
	class Hole {
	private:
		Model*				model;
		DirectX::XMFLOAT2	pos;
		float				radius;
		HolyFloor*			parent;
	public:
		Hole(DirectX::XMFLOAT2 p, float r, HolyFloor* parent) : pos(p), radius(r), parent(parent), model(nullptr) {}
		~Hole() { if (model) delete model; }
		bool CollisionVsCircle(DirectX::XMFLOAT2 p, float r) {
			return ((pos.x - p.x) * (pos.x - p.x) + (pos.y - p.y) * (pos.y - p.y)) < (radius - r) * (radius - r);
		}
		void Render(const RenderContext& rc, ModelRenderer* renderer) {

			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(parent->scale.x * radius / parent->radius, parent->scale.y, parent->scale.z * radius / parent->radius);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(parent->angle.x, parent->angle.y, parent->angle.z);
			DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(parent->position.x + pos.x, parent->position.y + 1.0f, parent->position.z + pos.y);
			DirectX::XMMATRIX W = S * R * T;
			DirectX::XMFLOAT4X4 HoleTransform;
			DirectX::XMStoreFloat4x4(&HoleTransform, W);

			if (!model) model = new Model("Data/Model/Hole.mdl");
			renderer->Render(rc, HoleTransform, model, ShaderId::Lambert);
		}
		void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
			renderer->RenderCylinder(rc, { parent->position.x + pos.x, parent->position.y + 1.0f, parent->position.z + pos.y }, radius, parent->height, DirectX::XMFLOAT4(1, 1, 1, 1));
		}
	};
	friend class Hole;

	vector<Hole> holes;

public:

	void addHole(DirectX::XMFLOAT2 pos, float r) { holes.push_back(Hole(pos, r, this)); }

	int CollisionVsSphere(const DirectX::XMFLOAT3& positionS, float radiusS) override {
		int super = Floor::CollisionVsSphere(positionS, radiusS);
		if (!super) return false;

		for (Hole& hole : holes) {
			if (hole.CollisionVsCircle({ positionS.x, positionS.z }, radiusS)) { Deactivate(); return false; }
		}

		return super;
	}

	void Render(const RenderContext& rc, ModelRenderer* renderer) override {
		Floor::Render(rc, renderer);
		for (auto& hole : holes) hole.Render(rc, renderer);
	}

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) {
		Floor::RenderDebugPrimitive(rc, renderer);
		for (auto& hole : holes) hole.RenderDebugPrimitive(rc, renderer);
	}

};