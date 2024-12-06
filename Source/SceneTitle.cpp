#include "SceneTitle.h"

#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"

#include "System/Graphics.h"
#include "System/Input.h"


void SceneTitle::Initialize() {
	sprite = new Sprite("Data/Sprite/Title.png");
}

void SceneTitle::Finalize() {
	if (sprite != nullptr) {
		delete sprite;
		sprite = nullptr;
	}
}

void SceneTitle::Update(float elapsedTime) {
	GamePad& gamePad = Input::Instance().GetGamePad();
	const GamePadButton anyButton =
		GamePad::BTN_A |
		GamePad::BTN_B |
		GamePad::BTN_X |
		GamePad::BTN_Y ;

	if (gamePad.GetButtonDown() & anyButton) SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
}

void SceneTitle::Render() {
	Graphics&		graphics	= Graphics::Instance();
	RenderContext	rc;
	rc.deviceContext			= graphics.GetDeviceContext();
	rc.renderState				= graphics.GetRenderState();

	float screenWidth	= static_cast<float>(graphics.GetScreenWidth());
	float screenHeight	= static_cast<float>(graphics.GetScreenHeight());
	sprite->Render(rc, 0, 0, 0, screenWidth, screenHeight, 0, 1, 1, 1, 1);
}

void SceneTitle::DrawGUI() {}