#include "System/Graphics.h"
#include "SceneGame.h"
#include "player.h"
#include "EffectManager.h"

// ������
void SceneGame::Initialize() {
	stage				= new Stage();
	cameraController	= new CameraController();

	Player::Instance().Initialize();

	// Camera Settings
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10), // Eye
		DirectX::XMFLOAT3(0, 0, 0), // Focus
		DirectX::XMFLOAT3(0, 1, 0)  // up
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45), // FovY
		graphics.GetScreenWidth() / graphics.GetScreenHeight(), // Aspect
		0.1f,	// NearZ
		1000.0f // FarZ
	);

	// Monster Setup
	EnemyManager& em = EnemyManager::Instance();
	for (int i = 0; i < 2; i++) {
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0.0f, 5.0f));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		em.Register(slime);
	}
}

// �I����
void SceneGame::Finalize() {
	if (stage != nullptr) {
		delete stage;
		stage = nullptr;
	}

	Player::Instance().Finalize();

	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}

	EnemyManager::Instance().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime) {
	stage->Update(elapsedTime);

	Player::Instance().Update(elapsedTime);

	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// �`�揀��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ���C�g�����i�������j
	rc.renderState = graphics.GetRenderState();

	// Camera Setup
	Camera& camera	= Camera::Instance();
	rc.view			= camera.GetView();
	rc.projection	= camera.GetProjection();


	// 3D���f���`��
	{
		stage->Render(rc, modelRenderer);
		Player::Instance().Render(rc, modelRenderer);
		EnemyManager::Instance().Render(rc, modelRenderer);
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2D�X�v���C�g�`��
	{

	}
}

// GUI�`��
void SceneGame::DrawGUI()
{
	Player::Instance().DrawDebugGUI();
}
