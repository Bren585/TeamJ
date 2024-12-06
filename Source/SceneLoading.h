#pragma once

#include "System/Sprite.h"
#include "Scene.h"

#include <thread>

class SceneLoading : public Scene {
private:
	Scene*			nextScene	= nullptr;
	std::thread*	thread		= nullptr;

	Sprite*			sprite		= nullptr;
	float			angle		= 0.0f;

	static void LoadingThread(SceneLoading* scene);
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {}

	void Initialize()				override;
	void Finalize()					override;
	void Update(float elapsedTime)	override;
	void Render()					override;
	void DrawGUI()					override;
};