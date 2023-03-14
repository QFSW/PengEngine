#include "draw_call_tree.h"

#include <profiling/scoped_event.h>
#include <utils/strtools.h>

#include "draw_call.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"

using namespace rendering;

DrawCallTree::DrawCallTree(const std::vector<DrawCall>& draw_calls)
{
	SCOPED_EVENT("Building DrawCallTree", strtools::catf_temp("%d draw calls", draw_calls.size()));

	for (const DrawCall& draw_call : draw_calls)
	{
		const peng::shared_ref<const Shader> shader = draw_call.material->shader();
		MeshDrawTree& mesh_draw = find_add_mesh_draw(shader, draw_call.mesh);
		mesh_draw.materials.push_back(draw_call.material);
	}
}

void DrawCallTree::execute() const
{
	SCOPED_EVENT("DrawCallTree - execute");

	for (const ShaderDrawTree& shader_draw : _shader_draws)
	{
		shader_draw.shader->use();

		for (const MeshDrawTree& mesh_draw : shader_draw.mesh_draws)
		{
			const peng::shared_ref<const Mesh> mesh = mesh_draw.mesh;
			mesh->bind();

			for (const peng::shared_ref<Material>& material : mesh_draw.materials)
			{
				material->apply_uniforms();
				mesh->draw();
			}
		}
	}

	glBindVertexArray(0);
}

ShaderDrawTree& DrawCallTree::find_add_shader_draw(const peng::shared_ref<const Shader>& shader)
{
	if (const auto it = _shader_draw_indices.find(shader); it != _shader_draw_indices.end())
	{
		return _shader_draws[it->second];
	}

	const ShaderDrawTree shader_draw = {
		.index = _shader_draws.size(),
		.shader = shader
	};

	_shader_draw_indices[shader] = shader_draw.index;
	_shader_draws.push_back(shader_draw);

	return _shader_draws[shader_draw.index];
}

MeshDrawTree& DrawCallTree::find_add_mesh_draw(const peng::shared_ref<const Shader>& shader,
	const peng::shared_ref<const Mesh>& mesh)
{
	const auto key = std::make_tuple(shader, mesh);
	if (const auto it = _mesh_draw_indices.find(key); it != _mesh_draw_indices.end())
	{
		const std::tuple<size_t, size_t> index_pair = it->second;
		return _shader_draws[std::get<0>(index_pair)].mesh_draws[std::get<1>(index_pair)];
	}

	ShaderDrawTree& shader_draw = find_add_shader_draw(shader);
	std::vector<MeshDrawTree>& mesh_draws = shader_draw.mesh_draws;

	const MeshDrawTree mesh_draw = {
		.index = mesh_draws.size(),
		.mesh = mesh
	};

	_mesh_draw_indices[key] = std::make_tuple(shader_draw.index, mesh_draw.index);
	mesh_draws.push_back(mesh_draw);

	return mesh_draws[mesh_draw.index];
}