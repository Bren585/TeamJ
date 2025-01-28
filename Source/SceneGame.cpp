#include "System/Graphics.h"
#include "SceneGame.h"
#include "player.h"
#include "EffectManager.h"
#include "Collision.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include <sstream>

// 初期化
void SceneGame::Initialize() {
	//stage				= new Stage();
	cameraController	= new CameraController();
	floor[0]			= new HolyFloor();
	floor[0]->Initialize();
	((HolyFloor*)floor[0])->addHole({0, 20}, 4.0f);

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

	//// Monster Setup
	//EnemyManager& em = EnemyManager::Instance();
	//for (int i = 0; i < 2; i++) {
	//	EnemySlime* slime = new EnemySlime();
	//	slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0.0f, 5.0f));
	//	slime->SetTerritory(slime->GetPosition(), 10.0f);
	//	em.Register(slime);
	//}

	// Sprites
	{
		std::stringstream ss;
		for (int i = 0; i < 10; i++) { 
			ss << "data/Sprite/numbers/" << i << ".png";
			numbers[i] = new Sprite(ss.str().c_str());
			ss.str("");
		}
		numbers[10] = new Sprite("data/Sprite/numbers/slash.png");

		shots = new Sprite("data/Sprite/Shots.png");
		game_over = new Sprite("data/Sprite/Score.png");
		star[true] = new Sprite("data/Sprite/filled_star.png");
		star[false] = new Sprite("data/Sprite/empty_star.png");

	}
}

// 終了化
void SceneGame::Finalize() {
	//if (stage != nullptr) {
	//	delete stage;
	//	stage = nullptr;
	//}

	Player::Instance().Finalize();

	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}

	if (floor[0]) { delete floor[0]; floor[0] = nullptr; }
	if (floor[1]) { delete floor[1]; floor[1] = nullptr; }

	for (int i = 0; i < 11; i++) {
		if (numbers[i]) {
			delete numbers[i];
			numbers[i] = nullptr;
		}
	}
	
	if (shots) {
		delete shots;
		shots = nullptr;
	}
	
	if (game_over) {
		delete game_over;
		game_over = nullptr;
	}

	if (star[0]) { delete star[0]; star[0] = nullptr; }
	if (star[1]) { delete star[1]; star[1] = nullptr; }

	EnemyManager::Instance().Clear();
}

// 更新処理
void SceneGame::Update(float elapsedTime) {
	//stage->Update(elapsedTime);

	if (!end) {

		Player::Instance().Update(elapsedTime);

		if (floor[0]) floor[0]->Update();
		if (floor[1]) floor[1]->Update();

		Collision::IntersectFloorVsSphere(floor[using_floor], Player::Instance());

		if (Player::Instance().GetPosition().y < floor[using_floor]->getPosition().y) {
			using_floor = !using_floor;
			if (floor[using_floor]) delete floor[using_floor];
			floor[using_floor] = new HolyFloor();
			floor[using_floor]->Initialize();
			((HolyFloor*)floor[using_floor])->addHole({ (float)(rand() % 50), (float)(rand() % 50) }, 4.0f);
			floor[using_floor]->setY(floor[!using_floor]->getPosition().y - 50.0f);
			score++;
		}

		if (Player::Instance().finished()) {
			end = true;
		}
	}
	else {
		timer += elapsedTime;
		if (timer > 5) {
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}
	}

	DirectX::XMFLOAT3 target = Player::Instance().GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//EnemyManager::Instance().Update(elapsedTime);
	EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	// Camera Setup
	Camera& camera	= Camera::Instance();
	rc.view			= camera.GetView();
	rc.projection	= camera.GetProjection();


	// 3Dモデル描画
	{
		//stage->Render(rc, modelRenderer);
		Player::Instance().Render(rc, modelRenderer);
		//EnemyManager::Instance().Render(rc, modelRenderer);
		EffectManager::Instance().Render(rc.view, rc.projection);
		if (floor[0]) floor[0]->Render(rc, modelRenderer);
		if (floor[1]) floor[1]->Render(rc, modelRenderer);
	}

	// 3Dデバッグ描画
	if (false) {
		Player::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		if (floor[0]) floor[0]->RenderDebugPrimitive(rc, shapeRenderer);
		if (floor[1]) floor[1]->RenderDebugPrimitive(rc, shapeRenderer);
		//EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{
		float screenWidth  = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		shots->Render(rc, screenWidth / 2 - 128, screenHeight - 128 - 16, 0, 256, 128, 0, 1, 1, 1, 1);

		// Shots Display
		{
			numbers[10]->Render(rc, screenWidth / 2 + -16, screenHeight - 128 - 16 + 60, 0, 32, 64, 0, 1, 1, 1, 1);

			int shots = Player::Instance().getLimit();

			int num = shots;
			int digits[3];
			int i = 0;
			while (i < 3 && num > 0) {
				digits[i++] = num % 10;
				num /= 10;
			}

			int j = 0;
			while (i > 0) {
				numbers[digits[--i]]->Render(rc, screenWidth / 2 + 16 + 32 * j++, screenHeight - 128 - 16 + 60, 0, 32, 64, 0, 1, 1, 1, 1);
				num /= 10;
			}

			num = shots - Player::Instance().getHits();
			i = 0;
			do {
				digits[i++] = num % 10;
				num /= 10;
			} while (i < 3 && num > 0);

			j = i;
			while (i > 0) {
				numbers[digits[--i]]->Render(rc, screenWidth / 2 - 16 - 32 * j--, screenHeight - 128 - 16 + 60, 0, 32, 64, 0, 1, 1, 1, 1);
				num /= 10;
			}
		}
		
		if (end) {
			game_over->Render(rc, screenWidth / 2 - 128, screenHeight / 2 - 128, 0, 256, 256, 0, 1, 1, 1, 1);

			int num = score;
			int digits[2];
			int i = 0;
			do {
				digits[i++] = num % 10;
				num /= 10;
			} while (i < 2 && num > 0);

			int j = i;
			while (i > 0) {
				numbers[digits[--i]]->Render(rc, screenWidth / 2 - (score > 10 ? 32 : 16) - 32 * --j, screenHeight / 2 - 32, 0, 32, 64, 0, 1, 1, 1, 1);
				num /= 10;
			}

			star[score > 1]->Render(rc, screenWidth / 2 - 112, screenHeight / 2 + 48, 0, 64, 64, 0, 1, 1, 1, 1);
			star[score > 4]->Render(rc, screenWidth / 2 -  32, screenHeight / 2 + 48, 0, 64, 64, 0, 1, 1, 1, 1);
			star[score > 8]->Render(rc, screenWidth / 2 +  48, screenHeight / 2 + 48, 0, 64, 64, 0, 1, 1, 1, 1);

		}

	}
}

// GUI描画
void SceneGame::DrawGUI()
{
	//Player::Instance().DrawDebugGUI();
}
