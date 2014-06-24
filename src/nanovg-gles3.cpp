#include "nanovg-gles3.hpp"

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"

using namespace std;

namespace nvg {

unique_ptr<Context> createContextGLES3(bool edgeAA) {
  auto ctx = nvgCreateGLES3(edgeAA ? NVG_ANTIALIAS : 0);
  auto ptr = unique_ptr<NVGcontext, RawContextDeleter>(ctx, [](NVGcontext* ctx) {
    nvgDeleteGLES3(ctx);
  });
  return unique_ptr<Context>(new Context(move(ptr)));
}

}