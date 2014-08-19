#pragma once

#include "nanovg.hpp"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

namespace nvg {

Context createContextGL2(bool antiAlias = true, bool stencilStrokes = false) {
  int flags = (antiAlias      ? NVG_ANTIALIAS       : 0) |
              (stencilStrokes ? NVG_STENCIL_STROKES : 0);

  return { Context::BackingCtx{ nvgCreateGLES2(flags), nvgDeleteGL2 } };
}

} // nvg
