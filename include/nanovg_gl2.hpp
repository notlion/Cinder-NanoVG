#pragma once

#include "nanovg.hpp"
#include "cinder/gl/gl.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"

namespace nvg {

Context createContextGL2(bool antiAlias = true, bool stencilStrokes = false) {
  int flags = (antiAlias      ? NVG_ANTIALIAS       : 0) |
              (stencilStrokes ? NVG_STENCIL_STROKES : 0);

  return { Context::BackingCtx{ nvgCreateGL2(flags), nvgDeleteGL2 } };
}
  
} // nvg
