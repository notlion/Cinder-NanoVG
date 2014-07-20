#pragma once

#include "nanovg.hpp"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"

namespace nvg {

std::unique_ptr<Context> createContextGLES3(bool antiAlias      = true,
                                            bool stencilStrokes = false) {
  int flags = antiAlias ? NVG_ANTIALIAS : 0;
  flags |= stencilStrokes ? NVG_STENCIL_STROKES : 0;

  auto ctx = nvgCreateGLES3(flags);
  auto ptr = std::unique_ptr<NVGcontext, RawContextDeleter>(ctx, [](NVGcontext* ctx) {
    nvgDeleteGLES3(ctx);
  });

  return std::unique_ptr<Context>(new Context(std::move(ptr)));
}
  
} // nvg
