#pragma once

#include "texture.h"

class CubeMap : public Texture, public std::enable_shared_from_this<CubeMap>
{
public:

  CubeMap(){}
  CubeMap(string cubemap_path_from_textures_folder,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear);
};