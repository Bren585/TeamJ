#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

using Effekseer::Handle;
typedef DirectX::XMFLOAT3 float3;

class Effect {
public:
	Effect(const char* filename);
	~Effect() {}

	Handle Play(const float3& position, float scale = 1.0f);

	void Stop(Handle handle);

	void SetPosition(Handle handle, const float3& position);

	void SetScale(Handle handle, const float3& scale);

private:
	Effekseer::EffectRef effekseerEffect;
};