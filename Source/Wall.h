#pragma once


#include "System/ModelRenderer.h"
#include "System/ShapeRenderer.h"

class Wall {
private:
	DirectX::XMFLOAT3   size		= { 0, 0, 0 };
	DirectX::XMFLOAT3	position	= { 0, 0, 0 };
	DirectX::XMFLOAT3	scale		= { 1, 1, 1 };
	DirectX::XMFLOAT3	angle		= { 0, 0, 0 };
	DirectX::XMFLOAT4X4 transform	= { 1, 0, 0, 0,
										0, 1, 0, 0,
										0, 0, 1, 0,
										0, 0, 0, 1 };


private:
	void UpdateTransform();
};