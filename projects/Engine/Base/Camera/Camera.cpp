#include "Camera.h"
#include "Engine/Base/WinApp/WinApp.h"

Camera::Camera()
    : transform_({
          {1.0f, 1.0f, 1.0f},
          {0.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 0.0f}
}),
      fovY_(0.45f), aspectRatio_(float(WinApp::kWindowWidth_) / float(WinApp::kWindowHeight_)), nearClip_(0.1f), farClip_(100.0f),
      worldMatrix(MyMath::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate)), viewMatrix(MyMath::Inverse4x4(worldMatrix)),
      projectionMatrix(MyMath::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_)), viewProjectionMatrix(MyMath::Multiply(viewMatrix, projectionMatrix)) {}

void Camera::Init() {}

void Camera::Update() {

	//
	worldMatrix = MyMath::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix = MyMath::Inverse4x4(worldMatrix);

	projectionMatrix = MyMath::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix = MyMath::Multiply(viewMatrix, projectionMatrix);
}