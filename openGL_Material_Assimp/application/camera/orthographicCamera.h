#pragma once

#include "camera.h"

class OrthographicCamera :public Camera {
public:
	OrthographicCamera(float l, float r, float t, float b, float n, float f);
	~OrthographicCamera();

	glm::mat4 getProjectionMatrix()override;
	
	void scale(float deltaScale)override;

private://一次性赋值之后，不需要再更改了
	float mLeft = 0.0f;
	float mRight = 0.0f;
	float mTop = 0.0f;
	float mBottom = 0.0f;
	float mNear = 0.0f;
	float mFar = 0.0f;

	float mScale{ 0.0f };
};