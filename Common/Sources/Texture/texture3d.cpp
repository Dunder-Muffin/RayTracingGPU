#include "texture3d.h"
#include "stbi/stb_image.h"

Texture3D::Texture3D(int size_x, int size_y, int size_z,
  const string & name,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat,
  TextureWrappFormat wrapping)
{
  textureType = TextureType::Texture3DType;
  colorFormat = color_format;
  textureFormat = texture_format;
  textureName = name;

  glGenTextures(1, &textureObject);
  glBindTexture(textureType, textureObject);
  textureWidth = size_x;
  textureHeight = size_y;
  textureDepth = size_z;
  
  glTexImage3D(textureType, 0, get_internal_format(color_format, texture_format), textureWidth, textureHeight, textureDepth, 0, colorFormat, textureFormat, nullptr);

  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapping);

  GLenum minMagixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minMagixelFormat);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, minMagixelFormat);
  glBindTexture(textureType, 0); 
}
TexturePtr make_texture3d(int size_x, int size_y, int size_z, const string & name)
{
  return make_shared<Texture3D>(size_x, size_y, size_z, name);
}
TexturePtr make_texture3d(int size_x, int size_y, int size_z, const string & name,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat,
  TextureWrappFormat wrapping)
{
  return make_shared<Texture3D>(size_x, size_y, size_z, name, color_format, texture_format, pixelFormat, wrapping);
}

void Texture3D::execute_dispatch(int work_group_size)
{
    glBindImageTexture(0, textureObject, 0, GL_FALSE, 0, GL_WRITE_ONLY, get_internal_format(colorFormat, textureFormat));

    glDispatchCompute(textureWidth / work_group_size, textureHeight / work_group_size, textureDepth / work_group_size); 
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}