#pragma once

#include <vector>
#include "Texture.h"

class TextureCubeMap : public Texture
{
  protected:
    bool InnerInit(const std::vector<const char *> &faces);

    GLenum GetTextureTarget() const override;

  public:
    /*
	   * faces:[right, left, top, bottom, front, back]
	  */
    TextureCubeMap(const std::vector<const char *> &faces);
    ~TextureCubeMap() override;
};