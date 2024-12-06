#pragma once

#include "System/ModelRenderer.h"

// Stage
class Stage {
public:
	Stage();
	~Stage();

	void Update(float elapsedTime);

	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};