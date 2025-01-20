#include "Collision.h"

bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3* outPositionB
) {
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec		= DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	float range = radiusA + radiusB;
	if (range < sqrtf(lengthSq)) return false; 
	if (!outPositionB) return true;

	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	Vec = DirectX::XMVectorAdd(Vec, PositionA);

	DirectX::XMStoreFloat3(outPositionB, Vec);
	return true;
}

bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3* outPositionB
) {
	if (
		positionA.y				> positionB.y + heightB ||
		positionA.y + heightA	< positionB.y
		) {
		return false;
	}

	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (range < distXZ) return false;
	if (!outPositionB) return true;

	vx /= distXZ;
	vz /= distXZ;

	outPositionB->x = positionA.x + vx * range;
	outPositionB->y = positionB.y;
	outPositionB->z = positionA.z + vz * range;
	
	return true;
}