
#include "scene_object.h"

Transform SceneObject::GetTransform() const
{
	return transform_;
}
Vector3 SceneObject::GetPosition() const
{
	return transform_.translation;
}
Rotator SceneObject::GetRotation() const
{
	return transform_.rotation;
}
Vector3 SceneObject::GetScale() const
{
	return transform_.scale;
}
void SceneObject::SetTransform(const Transform& transform)
{
	transform_ = transform;
}
void SceneObject::SetPosition(const Vector3& position)
{
	transform_.translation = position;
}
void SceneObject::SetRotation(const Rotator& rotation)
{
	transform_.rotation = rotation;
}
void SceneObject::SetScale(const Vector3& scale)
{
	transform_.scale = scale;
}