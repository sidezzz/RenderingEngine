#pragma once

#include "primitives.h"

class SceneObject
{
public:
	Transform GetTransform() const;
	Vector3 GetPosition() const;
	Rotator GetRotation() const;
	Vector3 GetScale() const;
	void SetTransform(const Transform& transform);
	void SetPosition(const Vector3& position);
	void SetRotation(const Rotator& rotation);
	void SetScale(const Vector3& scale);

protected:
	Transform transform_;
};