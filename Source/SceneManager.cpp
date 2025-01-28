#include "SceneManager.h"

void SceneManager::Update(float elapsedTime) {
	if (nextScene != nullptr) {
		if (currentScene != nullptr) { currentScene->Finalize(); delete currentScene; }

		currentScene = nextScene;
		nextScene = nullptr;

		if (!currentScene->IsReady()) currentScene->Initialize();
	}

	if (currentScene != nullptr) {
		currentScene->Update(elapsedTime);
	}
}

void SceneManager::Render() {
	if (currentScene != nullptr) {
		currentScene->Render();
	}
}

void SceneManager::DrawGUI() {
	if (currentScene != nullptr) {
		currentScene->DrawGUI();
	}
}

void SceneManager::Clear() {
	if (currentScene != nullptr) {
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
	bkg->Stop();
	delete bkg;
}

void SceneManager::ChangeScene(Scene* scene) {
	nextScene = scene;
}