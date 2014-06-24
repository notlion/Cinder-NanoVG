#include "nanovg-gles2.hpp"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"

using namespace std;

namespace nvg {

unique_ptr<Context> createContextGLES2(bool edgeAA) {
  auto ctx = nvgCreateGLES2(edgeAA ? NVG_ANTIALIAS : 0);
  auto ptr = unique_ptr<NVGcontext, RawContextDeleter>(ctx, [](NVGcontext* ctx) {
    nvgDeleteGLES2(ctx);
  });
  return unique_ptr<Context>(new Context(move(ptr)));
}

}