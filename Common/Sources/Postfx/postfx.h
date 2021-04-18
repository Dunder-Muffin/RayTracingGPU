#pragma once
#include "component.h"
#include "Mesh/vertex_array_object.h"
#include "Shader/shader.h"
class PostFX : public Component, public IRenderable
{
private:
  VertexArrayObject vao;
protected:
  Shader shader;
public:
  PostFX(Shader shader);
  void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) override;
  virtual void postfx_render(); 
  virtual void postfx_unbind(); 
};