#include "Effect.h"
#include "EffectManager.h"
#include "System/Graphics.h"

Effect::Effect(const char* filename) {
	std::lock_guard<std::mutex> lock(Graphics::Instance().GetMutex());
	char16_t utf16Filename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

	effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}

Handle Effect::Play(const float3& position, float scale) {
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y, position.z);
	effekseerManager->SetScale(handle, scale, scale, scale);
	return handle;
}

void Effect::Stop(Handle handle) {
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->StopEffect(handle);
}

void Effect::SetPosition(Handle handle, const float3& position) {
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

void Effect::SetScale(Handle handle, const float3& scale) {
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}
