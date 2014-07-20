#pragma once

#include "nanovg.hpp"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

namespace nvg {

std::unique_ptr<Context> createContextGLES2(bool antiAlias      = true,
                                            bool stencilStrokes = false) {
  int flags = antiAlias ? NVG_ANTIALIAS : 0;
  flags |= stencilStrokes ? NVG_STENCIL_STROKES : 0;

  auto ctx = nvgCreateGLES2(flags);
  auto ptr = std::unique_ptr<NVGcontext, RawContextDeleter>(ctx, [](NVGcontext* ctx) {
    nvgDeleteGLES2(ctx);
  });

  return std::unique_ptr<Context>(new Context(std::move(ptr)));
}
  
} // nvg
