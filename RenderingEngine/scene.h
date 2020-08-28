#pragma once
#include <list>
#include <set>
#include <string>
#include <future>
#include <functional>

#include "camera.h"
#include "mesh_instance.h"

class Scene;

class SceneMeshLoader
{
public:
	using LoadCallback = std::function<std::vector<MeshInstance>(std::shared_ptr<Mesh>)>;

	void Tick();
	virtual void LoadMeshAsync(std::string file_path, LoadCallback callback) = 0;

protected:
	friend class Scene;
	SceneMeshLoader(Scene& scene) : scene_(scene) {}

	struct LoadTask
	{
		std::future<std::shared_ptr<Mesh>> future;
		LoadCallback callback;
	};

	std::list<LoadTask> pending_tasks_;
	Scene& scene_;
};

class ObjSceneMeshLoader : public SceneMeshLoader
{
public:
	void LoadMeshAsync(std::string file_path, LoadCallback callback) override;

protected:
	friend class Scene;
	ObjSceneMeshLoader(Scene& scene) : SceneMeshLoader(scene) {}

private:
	static std::shared_ptr<Mesh> LoadMesh(const std::string& file_path);
};

class Scene
{
public:
	const std::list<MeshInstance>& GetInstances() const;
	const Camera& GetCamera() const;
	Camera& GetCamera();
	ColorFloat GetBackgroundColor() const;
	void Tick();

	template<class T, typename = std::enable_if_t<std::is_base_of_v<SceneMeshLoader, T>>>
	T* SetMeshLoader()
	{
		object_loader_ = std::unique_ptr<SceneMeshLoader>(new T(*this));
		return static_cast<T*>(object_loader_.get());
	}

private:
	friend class SceneMeshLoader;

	std::unique_ptr<SceneMeshLoader> object_loader_;

	std::list<MeshInstance> instances_;
	std::set<std::shared_ptr<Mesh>> meshes_;

	Camera camera_;
	ColorFloat background_color_ = ColorFloat(0.5f, 0.5f, 0.5f, 1.f);
};