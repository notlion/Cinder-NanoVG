#include "nanovg.hpp"
#include "SvgRenderer.hpp"

#include "cinder/gl/gl.h"

#if defined(CINDER_GL_ES_2)
#define NANOVG_GLES2_IMPLEMENTATION
#else
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "nanovg_gl.h"

using std::string;
using std::function;

namespace cinder { namespace nvg {

Context Context::create(bool antiAlias, bool stencilStrokes) {
  int flags = (antiAlias      ? NVG_ANTIALIAS       : 0) |
              (stencilStrokes ? NVG_STENCIL_STROKES : 0);

#if defined(NANOVG_GLES2)
  return { Context::BackingCtx{ nvgCreateGLES2(flags), nvgDeleteGLES2 } };
#else
  return { Context::BackingCtx{ nvgCreateGL3(flags), nvgDeleteGL3 } };
#endif
}

Context::Context(BackingCtx&& ctx) : mCtx{ std::move(ctx) } {
}

void Context::polyLine(const PolyLine2f& polyline) {
  auto& pts = polyline.getPoints();
  if (pts.size() >= 2) {
    moveTo(pts[0]);
    for (auto i = begin(pts) + 1; i != end(pts); ++i) {
      lineTo(*i);
    }
    if (polyline.isClosed()) closePath();
  }
}

void Context::path2dSegment(Path2d::SegmentType type, const vec2 *p1,
                                                      const vec2 *p2,
                                                      const vec2 *p3) {
  switch (type) {
    case Path2d::MOVETO:  moveTo(*p3); break;
    case Path2d::QUADTO:  quadTo(*p2, *p3); break;
    case Path2d::CUBICTO: bezierTo(*p1, *p2, *p3); break;
    case Path2d::LINETO:
    case Path2d::CLOSE:
      lineTo(*p3); break;
  }
}

void Context::path2d(const cinder::Path2d &path) {
  const vec2* p1 = nullptr, *p2 = nullptr, *p3 = &path.getPoint(0);

  path2dSegment(Path2d::MOVETO, p1, p2, p3);

  size_t i = 1;
  for (auto& seg : path.getSegments()) {
    if(seg == Path2d::CLOSE) {
      p1 = p2;
      p2 = p3;
      p3 = &path.getPoint(0);
    }
    else {
      for (size_t ni = i + Path2d::sSegmentTypePointCounts[seg]; i < ni; ++i) {
        p1 = p2;
        p2 = p3;
        p3 = &path.getPoint(i);
      }
    }

    path2dSegment(seg, p1, p2, p3);
  }
}

void Context::shape2d(const Shape2d &shape) {
  for (auto& contour : shape.getContours()) {
    path2d(contour);
  }
}

void Context::drawSvg(const svg::Doc &svg) {
  SvgRenderer renderer(*this);
  svg.render(renderer);
}

}} // cinder::nvg
