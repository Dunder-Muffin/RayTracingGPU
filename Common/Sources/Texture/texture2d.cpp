#include "texture2d.h"
#include "stbi/stb_image.h"

Texture2D::Texture2D(string texture_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat,
  TextureWrappFormat wrapping, 
  bool generate_mips)
  {
    string fullpath = texture_path_from_textures_folder;
    int w, h, ch;
    auto image = stbi_load(fullpath.c_str(), &w, &h, &ch, 0);
		
		if (image)
		{
      textureType = TextureType::Texture2DType;
      colorFormat = color_format;
      textureFormat = texture_format;
      textureName = texture_path_from_textures_folder;
  
      glGenTextures(1, &textureObject);
      glBindTexture(textureType, textureObject);
			textureWidth = w;
			textureHeight = h;
      
			glTexImage2D(textureType, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, image);

      glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
      glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
      if (generate_mips)
      {
        glGenerateMipmap(textureType);
        GLenum mipmapMinPixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
        GLenum mipmapMagPixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
        
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, mipmapMinPixelFormat);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, mipmapMagPixelFormat);
      }
      else
      {
        GLenum minMagixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minMagixelFormat);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, minMagixelFormat);
      }
      glBindTexture(textureType, 0); 
		}
		else
		{
			debug_error("Can't load texture %s!",texture_path_from_textures_folder.c_str());
			return;
		}
  }
TexturePtr make_texture2d()
{
  return make_shared<Texture2D>();
}
TexturePtr make_texture2d(const string& texture_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat,
  TextureWrappFormat wrapping, 
  bool generate_mips)
{
  return make_shared<Texture2D>(texture_path_from_textures_folder, color_format, texture_format, pixelFormat, wrapping, generate_mips);
}
TexturePtr make_texture2d(const string& texture_path_from_textures_folder,
  TexturePixelFormat pixelFormat,
  TextureWrappFormat wrapping, 
  bool generate_mips,
  TextureColorFormat color_format, 
  TextureFormat texture_format)
{
  return make_shared<Texture2D>(texture_path_from_textures_folder, color_format, texture_format, pixelFormat, wrapping, generate_mips);
}