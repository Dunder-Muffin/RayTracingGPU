#include "cube_map_texture.h"
#include "stbi/stb_image.h"

CubeMap::CubeMap(string cubemap_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat)
{
  
  string folderPath = cubemap_path_from_textures_folder;
  stbi_uc * images[6];
  int size = -1;
  for (int i = 0; i < 6; i++)
  {
    string fullpath =  folderPath + "/" + to_string(i) + ".jpg";
    int w, h, ch;
    images[i] = stbi_load(fullpath.c_str(), &w, &h, &ch, 0);
    if (!images[i])
    {
      debug_error("Can't load face[%d] %s for cubemap!", i, fullpath.c_str());
      continue;
    }
    if (h != w)
    {
      debug_error("Cubmap face[%d] %s must have similar width and height (%d != %d) !", i, cubemap_path_from_textures_folder.c_str(), w, h);
      return;
    }
    if (size < 0)
    {
      size = h;
    } else
    {
      if (size != h)
      {
        debug_error("Cubmap face[%d] %s must have similar sizes (%d != %d) !", i, cubemap_path_from_textures_folder.c_str(), size, h);
        return;
      }
    }    
  }
  textureType = TextureType::CubeMapType;
  colorFormat = color_format;
  textureFormat = texture_format;
  textureName = cubemap_path_from_textures_folder;

  textureWidth = textureHeight = size;
  glGenTextures(1, &textureObject);
  glBindTexture(textureType, textureObject);
  for (int i = 0; i < 6; i++)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, images[i]);
    stbi_image_free(images[i]);
  }
  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  GLenum minMagixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minMagixelFormat);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, minMagixelFormat);
  
  glBindTexture(textureType, 0); 
  
}