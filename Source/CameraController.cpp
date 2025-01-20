#include "CameraController.h"

void CameraController::Update(float elapsedTime) {
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	float speed = rollSpeed * elapsedTime;

	angle.y += ax * speed;
	angle.x += ay * speed;

	if (angle.x > maxAngleX) { angle.x = maxAngleX; }
	if (angle.x < minAngleX) { angle.x = minAngleX; }

	if (angle.y < -DirectX::XM_PI) { angle.y += DirectX::XM_2PI; }
	if (angle.y >  DirectX::XM_PI) { angle.y -= DirectX::XM_2PI; }

	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::SetTarget(const DirectX::XMFLOAT3 target) {
	this->target = target;
}

void CameraController::SetAngle(const DirectX::XMFLOAT3 angle) {
	this->angle = angle;
}