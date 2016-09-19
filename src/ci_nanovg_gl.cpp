#include "ci_nanovg_gl.hpp"

#if defined(CINDER_GL_ES_3)
#define NANOVG_GLES3_IMPLEMENTATION
#elif defined(CINDER_GL_ES_2)
#define NANOVG_GLES2_IMPLEMENTATION
#else
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "nanovg_gl.h"

namespace cinder { namespace nvg {

ContextGL createContextGL(bool antiAlias, bool stencilStrokes) {
  int flags = (antiAlias      ? NVG_ANTIALIAS       : 0) |
              (stencilStrokes ? NVG_STENCIL_STROKES : 0);

#if defined(NANOVG_GLES3)
  return { nvgCreateGLES3(flags), nvgDeleteGLES3 };
#elif defined(NANOVG_GLES2)
  return { nvgCreateGLES2(flags), nvgDeleteGLES2 };
#else
  return { nvgCreateGL3(flags), nvgDeleteGL3 };
#endif
}

Image ContextGL::createImageFromHandle(GLuint textureId, int w, int h, int imageFlags) {
#if defined(NANOVG_GL3)
  int id = nvglCreateImageFromHandleGL3(get(), textureId, w, h, imageFlags);
#elif defined(NANOVG_GLES2)
  int id = nvglCreateImageFromHandleGLES2(get(), textureId, w, h, imageFlags);
#elif defined(NANOVG_GLES3)
  int id = nvglCreateImageFromHandleGLES3(get(), textureId, w, h, imageFlags);
#endif

  return { get(), id };
}

}} // cinder::nvg
