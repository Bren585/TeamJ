#pragma once

#include "Scene.h"
#include "System/Audio.h"

class SceneManager {
private:
	SceneManager() { bkg = Audio::Instance().LoadAudioSource("data/Sound/bkg_ost.wav"); bkg->Play(true); }
	~SceneManager() { }

	Scene* currentScene = nullptr;
	Scene* nextScene	= nullptr;

	AudioSource* bkg;

public:
	static SceneManager& Instance() {
		static SceneManager instance;
		return instance;
	}

	void Update(float elapsedTime); 
	void Render();
	void DrawGUI();
	void Clear();
	void ChangeScene(Scene* scene);
};