#include "nanovg.hpp"
#include "cinder/gl/gl.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"

namespace nvg {

std::unique_ptr<Context> createContextGL2(bool antiAlias      = true,
                                          bool stencilStrokes = false) {
  int flags = antiAlias ? NVG_ANTIALIAS : 0;
  flags |= stencilStrokes ? NVG_STENCIL_STROKES : 0;

  auto ctx = nvgCreateGL2(flags);
  auto ptr = std::unique_ptr<NVGcontext, RawContextDeleter>(ctx, [](NVGcontext* ctx) {
    nvgDeleteGL2(ctx);
  });

  return std::unique_ptr<Context>(new Context(std::move(ptr)));
}
  
} // nvg
