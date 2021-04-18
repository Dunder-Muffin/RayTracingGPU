#pragma once
#include "common.h"
#include "Shader/shader.h"
enum TextureType
{
  Texture2DType = GL_TEXTURE_2D,
  Texture3DType = GL_TEXTURE_3D,
  CubeMapType = GL_TEXTURE_CUBE_MAP
};
enum TextureColorFormat
{
  R = GL_RED,
  RG = GL_RG,
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  Depth = GL_DEPTH_COMPONENT,
  DepthStencil = GL_DEPTH_STENCIL
};
enum TextureFormat
{
  UnsignedByte =  GL_UNSIGNED_BYTE,
  Byte =  GL_BYTE,
  UnsignedShort = GL_UNSIGNED_SHORT,
  Short = GL_SHORT,
  UnsignedInt = GL_UNSIGNED_INT,
  Int = GL_INT,
  HalfFloat = GL_HALF_FLOAT,
  Float = GL_FLOAT
};
enum TextureWrappFormat
{
  Repeat = GL_REPEAT,
  ClampToEdge = GL_CLAMP_TO_EDGE,
  ClampToBorder = GL_CLAMP_TO_BORDER
};
enum TexturePixelFormat
{
  Pixel,
  Linear
};
class Texture
{
  
protected:
  GLuint textureObject;
  TextureType textureType;
  TextureColorFormat colorFormat;
  TextureFormat textureFormat;
  int textureWidth, textureHeight, textureDepth;
  string textureName;
  GLenum get_internal_format(TextureColorFormat colorFormat, TextureFormat textureFormat);
private:
  GLuint activationUnit;
public:
  Texture():textureObject(0), textureDepth(1){}
  void bind(const Shader &shader, const char * textureName);
  const string &get_name() const;
  void unbind() const;
  void dispose();
  int width() const;
  int height() const;
  int depth() const;
  
};
using TexturePtr = shared_ptr<Texture>;