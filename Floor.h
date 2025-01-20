#pragma once

#include "System/ModelRenderer.h"
#include "System/ShapeRenderer.h"
#include <vector>

using std::vector;

class Floor {
private:

	//DirectX::XMFLOAT3   size		= { 0, 0, 0 };
	float				radius		= 0;
	float				height		= 0;
	DirectX::XMFLOAT3	position	= { 0, 0, 0 };
	DirectX::XMFLOAT3	scale		= { 1, 1, 1 };
	DirectX::XMFLOAT3	angle		= { 0, 0, 0 };
	DirectX::XMFLOAT4X4 transform	= { 1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1 };

public:

	virtual int CollisionVsSphere(const DirectX::XMFLOAT3& positionS, float radiusS) {
		if (fabsf(position.y - positionS.y) > radiusS + height / 2.0f) return false;
		DirectX::XMFLOAT2 dist = { position.x - positionS.x, position.z - positionS.z };
		float hyp = dist.x * dist.x + dist.y * dist.y;

		if (hyp < (radius + radiusS) * (radius + radiusS)) {
			return (position.y > positionS.y) ? -1 : 1;
		}
		return false;
	}

	void UpdateTransform() {
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(radius, height / 2.0f, radius);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		DirectX::XMMATRIX W = S * R * T;
		DirectX::XMStoreFloat4x4(&transform, W);
	}
};

class HolyFloor : public Floor {
private:
	class Hole {
	private:
		DirectX::XMFLOAT2 pos;
		float radius;
		HolyFloor* parent;
	public:
		Hole(DirectX::XMFLOAT2 p, float r, HolyFloor* parent) : pos(p), radius(r), parent(parent) {}
		bool CollisionVsCircle(DirectX::XMFLOAT2 p, float r) {
			return ((pos.x - p.x) * (pos.x - p.x) + (pos.y - p.y) * (pos.y - p.y)) < (radius - r) * (radius - r);
		}
	};
	
	vector<Hole> holes;

public:

	void addHole(DirectX::XMFLOAT2 pos, float r) { holes.push_back(Hole(pos, r, this)); }

	int CollisionVsSphere(const DirectX::XMFLOAT3& positionS, float radiusS) override {
		int super = Floor::CollisionVsSphere(positionS, radiusS);
		if (!super) return false;

		for (Hole hole : holes) {
			if (hole.CollisionVsCircle({ positionS.x, positionS.z }, radiusS)) { return false; }
		}

		return super;
	}

};