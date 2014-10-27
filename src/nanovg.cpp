#include "nanovg.hpp"
#include "SvgRenderer.hpp"

#include "cinder/gl/gl.h"

#if defined(CINDER_GL_ES_3)
#define NANOVG_GLES3_IMPLEMENTATION
#elif defined(CINDER_GL_ES_2)
#define NANOVG_GLES2_IMPLEMENTATION
#else
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "nanovg_gl.h"

namespace cinder { namespace nvg {

Context::Context(NVGcontext *ptr, Deleter deleter) : mPtr{ ptr, deleter } {
}

Context Context::create(bool antiAlias, bool stencilStrokes) {
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

// svg::Paint to NVGpaint conversion.
// Currently only works for 2-color linear gradients.
NVGpaint Context::convert(const svg::Paint &paint) {
  // NanoVG does not really support radial gradients in the same way as SVG.
  // Cinder seems to support unequal radius on x and y, and NVG has two radii.
  // Since I don't have a use for this i'm just going to punt for now.
  // There is probably also a better way to handle this..
  assert(paint.isLinearGradient());

  const auto& p0 = paint.getCoords0();
  const auto& p1 = paint.getCoords1();
  const auto& c0 = paint.getColor(0);
  const auto& c1 = paint.getColor(1);

  return linearGradient(p0, p1, c0, c1);
}

void Context::strokePaint(const svg::Paint &paint) {
  if (paint.isNone()) return;
  if (paint.isLinearGradient()) strokePaint(convert(paint));
  strokeColor(paint.getColor());
}

void Context::fillPaint(const svg::Paint &paint) {
  if (paint.isNone()) return;
  if (paint.isLinearGradient()) fillPaint(convert(paint));
  fillColor(paint.getColor());
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

void Context::draw(const svg::Doc &svg) {
  SvgRenderer renderer(*this);
  svg.render(renderer);
}

}} // cinder::nvg
