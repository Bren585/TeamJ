#include "SceneLoading.h"

#include "SceneManager.h"
#include "System/Graphics.h"
#include "System/Input.h"

void SceneLoading::LoadingThread(SceneLoading* scene) {
	CoInitialize(nullptr);
	scene->nextScene->Initialize();
	CoUninitialize();
	scene->nextScene->SetReady();
}

void SceneLoading::Initialize() {
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	thread = new std::thread(LoadingThread, this);
}

void SceneLoading::Finalize() {
	if (sprite != nullptr) {
		delete sprite;
		sprite = nullptr;
	}

	if (thread != nullptr) {
		thread->join();
		delete thread;
		thread = nullptr;
	}
}

void SceneLoading::Update(float elapsedTime) {
	constexpr float speed = 180.0f;
	angle += speed * elapsedTime;

	if (nextScene->IsReady()) {
		SceneManager::Instance().ChangeScene(nextScene);
	}
}

void SceneLoading::Render() {
	Graphics& graphics	= Graphics::Instance();
	RenderContext rc;
	rc.deviceContext	= graphics.GetDeviceContext();
	rc.renderState		= graphics.GetRenderState();

	float screenWidth	= static_cast<float>(graphics.GetScreenWidth());
	float screenHeight	= static_cast<float>(graphics.GetScreenHeight());
	float spriteWidth	= 256.0f;
	float spriteHeight	= 256.0f;
	float positionX		= screenWidth - spriteWidth;
	float positionY		= screenHeight - spriteHeight;

	sprite->Render(
		rc,
		positionX, positionY, 0, spriteWidth, spriteHeight,
		angle,
		1, 1, 1, 1
	);
}

void SceneLoading::DrawGUI() {

}