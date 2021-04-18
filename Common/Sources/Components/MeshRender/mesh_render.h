
#pragma once
/*#include "component.h"
#include "Mesh/mesh.h"
#include "Shader/shader.h"
#include "Material/material.h"*/
#include "Camera/camera.h"
/*#include "Light/direction_light.h"
#include "Transform/transform.h"
class MeshRender : public Component, public IRenderable
{
private:
  MeshPtr mesh;
  MaterialPtr material;
  Shader shader;
public:
 MeshRender(MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader);
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false);
  void render(const Transform *transform, const Camera& mainCam, const DirectionLight& light, bool wire_frame = false);
  MaterialPtr get_material() const;
  const Shader& get_shader() const;
  Shader& get_shader();
};
shared_ptr<MeshRender> create_plane(bool create_uv = false);
shared_ptr<MeshRender> create_cube(bool create_uv = false);
shared_ptr<MeshRender> create_sphere(int detailed = 1, bool smooth = false, bool create_uv = false);
shared_ptr<MeshRender> create_postfx();*/