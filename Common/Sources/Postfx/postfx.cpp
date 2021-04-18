#include "postfx.h"

PostFX::PostFX(Shader shader):
shader(shader)
{
  static VertexArrayObject postFXvao;
  if (!postFXvao.is_valid())
  {
    vector<uint> indices = {0,1,2,0,2,3};
    vector<vec2> corners =  {vec2(-1, -1), vec2(1,-1), vec2(1,1),vec2(-1,1)};
    postFXvao = VertexArrayObject(indices, corners);
  }
  vao = postFXvao;
}
void PostFX::render(const Camera&, const DirectionLight&, bool)
{
  shader.use();
  postfx_render();
  vao.render(false);
  postfx_unbind();
}
void PostFX::postfx_render()
{

}
void PostFX::postfx_unbind()
{

}