#pragma once

#include <DirectXMath.h>
#include "Character.h"
#include "floor.h"

class Collision {
public:
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3* outPositionB
	);

	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3* outPositionB
	);

	static bool IntersectFloorVsSphere(
		Floor* floor,
		Character& character
	);

	inline static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3* outCylinderPosition
	) {
		DirectX::XMFLOAT3 spherePositionFloor = spherePosition;
		spherePositionFloor.y -= sphereRadius;
		return IntersectCylinderVsCylinder(
			spherePositionFloor,
			sphereRadius, 
			2 * sphereRadius, 
			cylinderPosition, 
			cylinderHeight, 
			cylinderRadius, 
			outCylinderPosition
		);
	}

	inline static bool IntersectCharacterVsCharacter(
		const Character& characterA,
		const Character& characterB,
		DirectX::XMFLOAT3* outPositionB
	) {
		return IntersectCylinderVsCylinder(
			characterA.GetPosition(),
			characterA.GetRadius(),
			characterA.GetHeight(),
			characterB.GetPosition(),
			characterB.GetRadius(),
			characterB.GetHeight(),
			outPositionB
		);
	}
};