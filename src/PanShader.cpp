#include "PanShader.hpp"

PanShader::PanShader()
{
}

PanShader::~PanShader()
{
}

bool PanShader::build()
{
  bool ret;

  _build();

  _bindAttribLocation(0, "vPosition");
  _bindAttribLocation(1, "camPos");
  _bindAttribLocation(2, "lightVec");
  _bindAttribLocation(3, "ambientLight");
  ret = _linkProgram();
  if (ret)
    {
      _bindTextureUnit(0, "tColor");
      _bindTextureUnit(1, "tPosition");
      _bindTextureUnit(2, "tNormals");
    }
  return ret;
}
