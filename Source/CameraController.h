#pragma once

#include <DirectXMath.h>
#include "System/Input.h"
#include "CameraController.h"
#include "Camera.h"

class CameraController {
public:
	void Update(float elapsedTime);
	void SetTarget(const DirectX::XMFLOAT3 target);

private:
	DirectX::XMFLOAT3	target		= { 0, 0, 0 };
	DirectX::XMFLOAT3	angle		= { 0, 0, 0 };
	float				rollSpeed	= DirectX::XMConvertToRadians(90);
	float				range		= 10.0f;
	float				maxAngleX	= DirectX::XMConvertToRadians(45);
	float				minAngleX	= DirectX::XMConvertToRadians(-45);
};