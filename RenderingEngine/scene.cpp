#include <unordered_map>

#include "scene.h"
#include "3rd_party/tiny_obj_loader.h"

void SceneMeshLoader::Tick()
{
	for (auto itr = pending_tasks_.begin(); itr != pending_tasks_.end();)
	{
		auto& task = *itr;
		auto status = task.future.wait_for(std::chrono::seconds(0));
		if (status == std::future_status::ready)
		{
			auto instances = task.callback(task.future.get());
			scene_.instances_.insert(scene_.instances_.end(), instances.begin(), instances.end());
			itr = pending_tasks_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void ObjSceneMeshLoader::LoadMeshAsync(std::string file_path, LoadCallback callback)
{
	std::promise<std::shared_ptr<Mesh>> promise;

	LoadTask task;
	task.callback = std::move(callback);
	task.future = promise.get_future();

	std::thread work_thread([](std::promise<std::shared_ptr<Mesh>> promise, std::string file_path)
	{
		promise.set_value(LoadMesh(file_path));
	}, std::move(promise), std::move(file_path));

	pending_tasks_.push_back(std::move(task));
	work_thread.detach();
}
std::shared_ptr<Mesh> ObjSceneMeshLoader::LoadMesh(const std::string& file_path)
{
	std::string material_dir;
	auto dir_end = file_path.rfind('\\');
	if (dir_end != std::string::npos)
	{
		material_dir = std::string(file_path.begin(), file_path.begin() + dir_end);
	}
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str(), material_dir.empty() ? nullptr : material_dir.c_str(), true);

	if (!ret)
	{
		/*if (!warn.empty()) {
			std::cout << "WARN " << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << "ERR " << err << std::endl;
		}*/
		return nullptr;
	}

	std::vector<Vertex> converted_vertices;
	std::vector<Index> converted_indices;
	std::unordered_map<Vertex, int> unique_vertices;

	unique_vertices.reserve(attrib.vertices.size() / 3);
	converted_vertices.reserve(attrib.vertices.size() / 3);
	for (auto& shape : shapes)
	{
		converted_indices.reserve(converted_indices.size() + shape.mesh.indices.size());

		int i = 0;
		for (auto& idx : shape.mesh.indices)
		{
			Vertex vertex;
			// axis are inverted due to different coordinate system
			vertex.position.y = attrib.vertices[3 * idx.vertex_index + 0];
			vertex.position.z = attrib.vertices[3 * idx.vertex_index + 1];
			vertex.position.x = attrib.vertices[3 * idx.vertex_index + 2];
			vertex.normal.y = attrib.normals[3 * idx.normal_index + 0];
			vertex.normal.z = attrib.normals[3 * idx.normal_index + 1];
			vertex.normal.x = attrib.normals[3 * idx.normal_index + 2];

			vertex.ambient = Vector3(1.f, 1.f, 1.f);
			vertex.diffuse = Vector3(1.f, 1.f, 1.f);
			vertex.specular = Vector3(0.f, 0.f, 0.f);
			vertex.shininess = 0.f;

			auto face_id = i / 3;
			auto material_id = shape.mesh.material_ids[face_id];
			if (material_id < materials.size())
			{
				auto& material = materials[material_id];
				vertex.ambient.x = material.ambient[0];
				vertex.ambient.y = material.ambient[1];
				vertex.ambient.z = material.ambient[2];

				vertex.diffuse.x = material.diffuse[0];
				vertex.diffuse.y = material.diffuse[1];
				vertex.diffuse.z = material.diffuse[2];

				vertex.specular.x = material.specular[0];
				vertex.specular.y = material.specular[1];
				vertex.specular.z = material.specular[2];

				vertex.shininess = material.shininess;
			}
			else
			{
				vertex.diffuse.x = attrib.colors[3 * idx.vertex_index + 0];
				vertex.diffuse.y = attrib.colors[3 * idx.vertex_index + 1];
				vertex.diffuse.z = attrib.colors[3 * idx.vertex_index + 2];
			}

			auto found_itr = unique_vertices.find(vertex);
			if (found_itr != unique_vertices.end())
			{
				converted_indices.push_back(found_itr->second);
			}
			else
			{
				unique_vertices[vertex] = converted_vertices.size();
				converted_indices.push_back(converted_vertices.size());
				converted_vertices.push_back(vertex);
			}
			++i;
		}
	}

	return std::make_shared<Mesh>(std::move(converted_vertices), std::move(converted_indices));
}

const std::list<MeshInstance>& Scene::GetInstances() const
{
	return instances_;
}
const Camera& Scene::GetCamera() const
{
	return camera_;
}
Camera& Scene::GetCamera()
{
	return camera_;
}
ColorFloat Scene::GetBackgroundColor() const
{
	return background_color_;
}
void Scene::Tick()
{
	if (object_loader_)
	{
		object_loader_->Tick();
	}
}