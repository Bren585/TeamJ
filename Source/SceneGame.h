#pragma once

#include "Scene.h"
#include "Stage.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "floor.h"
#include "System/Sprite.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

	// GUI�`��
	void DrawGUI() override;
private:
	//Stage* stage = nullptr;
	CameraController* cameraController = nullptr;
	Floor* floor[2] = { nullptr, nullptr };
	bool using_floor = false;

	Sprite* numbers[11] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	Sprite* shots = nullptr;

	Sprite* game_over = nullptr;
	Sprite* star[2] = {nullptr, nullptr};
	bool end = false;

	int score = 0;
	float timer = 0;
}; 
