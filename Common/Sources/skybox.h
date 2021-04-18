#pragma once
#include "common.h"
#include "component.h"
#include "Shader/shader.h"
#include "Texture/textures.h"
#include "Camera/camera.h"
#include "Mesh/vertex_array_object.h"
class SkyBox : public Component, public IRenderable
{
public:
  Shader skyboxShader;
  VertexArrayObject skyboxVAO;
  CubeMap skybox;
  SkyBox(string path);
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;
};