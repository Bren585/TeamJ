#pragma once

#include "Scene.h"
#include "Stage.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "EnemySlime.h"

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
	Stage* stage = nullptr;
	CameraController* cameraController = nullptr;
};
