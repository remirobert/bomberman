#include "TextureLoader.hpp"

#include <iostream>
#include <stdexcept>

Texture *TextureLoader::loadFromFile(const std::string& path) const
{
  Texture *ptr = new Texture(path);

  std::cout << path + " has been loaded" << std::endl;

  return ptr;
}
