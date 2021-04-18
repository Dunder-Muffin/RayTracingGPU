#include "texture.h"
static const int maxActiveTexturesCount = 10;
static bool activeTextures[maxActiveTexturesCount] = {false};

void Texture::bind(const Shader &shader, const char * textureName)
{
  int offset = 0;
  for (; offset < maxActiveTexturesCount; offset++)
  {
    if (!activeTextures[offset])
    {
      activationUnit = offset;
      break;
    }
  }
  if (offset == maxActiveTexturesCount)
  {
    debug_error("Need more texture unit for %s", textureName);
    return;
  }
  
  activeTextures[activationUnit] = true;
  glActiveTexture(GL_TEXTURE0 + activationUnit);
  glBindTexture(textureType, textureObject);
  glUniform1i(glGetUniformLocation(shader.get_shader_program(), textureName), activationUnit);
}
const string &Texture::get_name() const
{
  return textureName;
}
void Texture::unbind() const
{
  activeTextures[activationUnit] = false;
  glBindTexture(textureType, 0);
}
void Texture::dispose()
{
  glDeleteTextures(1, &textureObject);
  textureName = "Bad texture";
}
int Texture::width() const
{
  return textureWidth;
}
int Texture::height() const
{
  return textureHeight;
}
int Texture::depth() const
{
  return textureDepth;
}
GLenum Texture::get_internal_format(TextureColorFormat colorFormat, TextureFormat textureFormat)
{
 switch (colorFormat)
 {
  case R: switch (textureFormat)
  {
    case UnsignedByte:  return GL_R8UI; break;
    case Byte:          return GL_R8I; break;
    case UnsignedShort: return GL_R16UI; break;
    case Short:         return GL_R16I; break;
    case UnsignedInt:   return GL_R32UI; break;
    case Int:           return GL_R32I; break;
    case HalfFloat:     return GL_R16F; break;
    case Float:         return GL_R32F; break;
  }break;
  case RG: switch (textureFormat)
  {
    case UnsignedByte:  return GL_RG8UI; break;
    case Byte:          return GL_RG8I; break;
    case UnsignedShort: return GL_RG16UI; break;
    case Short:         return GL_RG16I; break;
    case UnsignedInt:   return GL_RG32UI; break;
    case Int:           return GL_RG32I; break;
    case HalfFloat:     return GL_RG16F; break;
    case Float:         return GL_RG32F; break;
  }break;
  case RGB: switch (textureFormat)
  {
    case UnsignedByte:  return GL_RGB8UI; break;
    case Byte:          return GL_RGB8I; break;
    case UnsignedShort: return GL_RGB16UI; break;
    case Short:         return GL_RGB16I; break;
    case UnsignedInt:   return GL_RGB32UI; break;
    case Int:           return GL_RGB32I; break;
    case HalfFloat:     return GL_RGB16F; break;
    case Float:         return GL_RGB32F; break;
  }break;
   case RGBA: switch (textureFormat)
  {
    case UnsignedByte:  return GL_RGBA8UI; break;
    case Byte:          return GL_RGBA8I; break;
    case UnsignedShort: return GL_RGBA16UI; break;
    case Short:         return GL_RGBA16I; break;
    case UnsignedInt:   return GL_RGBA32UI; break;
    case Int:           return GL_RGBA32I; break;
    case HalfFloat:     return GL_RGBA16F; break;
    case Float:         return GL_RGBA32F; break;
  }break;
  default:return colorFormat; break;
 }
 return colorFormat; 
}