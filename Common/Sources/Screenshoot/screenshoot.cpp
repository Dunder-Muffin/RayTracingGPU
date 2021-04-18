#include "screenshoot.h"
#include "Application/application.h"
#include "stbi/stb_image_write.h"
#include "stbi/stb_image.h"
void take_screenshoot(const std::string &filename)
{
  int width = Application::get_context().get_width();
  int height = Application::get_context().get_height();
  vector<u8> pixels(3 * width * height);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
  stbi_flip_vertically_on_write(1);
  stbi_write_png(filename.c_str(), width, height, 3, pixels.data(), width*3);
}