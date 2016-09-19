#pragma once

#include "cinder/gl/platform.h"
#include "ci_nanovg.hpp"

namespace cinder { namespace nvg {

class ContextGL : public Context {
public:
  ContextGL(NVGcontext *ptr, Deleter deleter) : Context(ptr, deleter) {}

  Image createImageFromHandle(GLuint textureId, int w, int h, int imageFlags);
};

ContextGL createContextGL(bool antiAlias = true, bool stencilStrokes = false);

}} // cinder::nvg
