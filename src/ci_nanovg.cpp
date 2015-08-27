#include "ci_nanovg.hpp"
#include "SvgRenderer.hpp"

namespace cinder {
namespace nvg {

Image::Image(Image &&image) : mCtx{ image.mCtx }, id{ image.id } {
  image.mCtx = nullptr;
}

Image &Image::operator=(Image &&image) {
  mCtx = image.mCtx;
  id = image.id;
  image.mCtx = nullptr;
  return *this;
}

Image::Image(NVGcontext *ctx, int id) : mCtx{ ctx }, id{ id } {}

Image::~Image() {
  if (mCtx) nvgDeleteImage(mCtx, id);
}

Context::Context(NVGcontext *ptr, Deleter deleter) : mPtr{ ptr, deleter } {
}

// svg::Paint to NVGpaint conversion.
// Currently only works for 2-color linear gradients.
NVGpaint Context::convert(const svg::Paint &paint) {
  // NanoVG does not really support radial gradients in the same way as SVG.
  // Cinder seems to support unequal radius on x and y, and NVG has two radii.
  // Since I don't have a use for this i'm just going to punt for now.
  // There is probably also a better way to handle this..
  assert(paint.isLinearGradient());

  const auto &p0 = paint.getCoords0();
  const auto &p1 = paint.getCoords1();
  const auto &c0 = paint.getColor(0);
  const auto &c1 = paint.getColor(1);

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

void Context::polyLine(const PolyLine2f &polyline) {
  auto &pts = polyline.getPoints();
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
  const vec2 *p1 = nullptr, *p2 = nullptr, *p3 = &path.getPoint(0);

  path2dSegment(Path2d::MOVETO, p1, p2, p3);

  size_t i = 1;
  for (auto &seg : path.getSegments()) {
    if (seg == Path2d::CLOSE) {
      p1 = p2;
      p2 = p3;
      p3 = &path.getPoint(0);
    } else {
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
  for (auto &contour : shape.getContours()) {
    path2d(contour);
  }
}

void Context::draw(const svg::Doc &svg) {
  SvgRenderer renderer(*this);
  svg.render(renderer);
}

// Frame //

void Context::beginFrame(int windowWidth, int windowHeight, float devicePixelRatio) {
  nvgBeginFrame(get(), windowWidth, windowHeight, devicePixelRatio);
}
void Context::beginFrame(const ivec2 &windowSize, float devicePixelRatio) {
  beginFrame(windowSize.x, windowSize.y, devicePixelRatio);
}
void Context::cancelFrame() {
  nvgCancelFrame(get());
}
void Context::endFrame() {
  nvgEndFrame(get());
}

// State Handling //

void Context::save() {
  nvgSave(get());
}
void Context::restore() {
  nvgRestore(get());
}
void Context::reset() {
  nvgReset(get());
}

// Render Styles //

void Context::strokeColor(const NVGcolor &color) {
  nvgStrokeColor(get(), color);
}
void Context::strokeColor(const ColorAf &color) {
  strokeColor(reinterpret_cast<const NVGcolor &>(color));
}
void Context::strokePaint(const NVGpaint &paint) {
  nvgStrokePaint(get(), paint);
}
void Context::fillColor(const NVGcolor &color) {
  nvgFillColor(get(), color);
}
void Context::fillColor(const ColorAf &color) {
  fillColor(reinterpret_cast<const NVGcolor &>(color));
}
void Context::fillPaint(const NVGpaint &paint) {
  nvgFillPaint(get(), paint);
}

void Context::miterLimit(float limit) {
  nvgMiterLimit(get(), limit);
}
void Context::strokeWidth(float size) {
  nvgStrokeWidth(get(), size);
}
void Context::lineCap(int cap) {
  nvgLineCap(get(), cap);
}
void Context::lineJoin(int join) {
  nvgLineJoin(get(), join);
}

// Transform //

void Context::resetTransform() {
  nvgResetTransform(get());
}
void Context::transform(const mat3 &mtx) {
  nvgTransform(get(), mtx[0][0], mtx[0][1], mtx[1][0], mtx[1][1], mtx[2][0], mtx[2][1]);
}
void Context::setTransform(const mat3 &mtx) {
  resetTransform();
  transform(mtx);
}
void Context::translate(float x, float y) {
  nvgTranslate(get(), x, y);
}
void Context::translate(const vec2 &translation) {
  translate(translation.x, translation.y);
}
void Context::rotate(float angle) {
  nvgRotate(get(), angle);
}
void Context::skewX(float angle) {
  nvgSkewX(get(), angle);
}
void Context::skewY(float angle) {
  nvgSkewY(get(), angle);
}
void Context::scale(float x, float y) {
  nvgScale(get(), x, y);
}
void Context::scale(const vec2 &s) {
  scale(s.x, s.y);
}

mat3 Context::currentTransform() {
  mat3 xform;
  nvgCurrentTransform(get(), &xform[0][0]);
  return xform;
}

// Paints //

NVGpaint Context::linearGradient(const vec2 &start, const vec2 &end,
                                 const ColorAf &startColor, const ColorAf &endColor) {
  return nvgLinearGradient(get(), start.x, start.y, end.x, end.y,
                           reinterpret_cast<const NVGcolor &>(startColor),
                           reinterpret_cast<const NVGcolor &>(endColor));
}
NVGpaint Context::boxGradient(const Rectf &bounds, float r, float f,
                              const ColorAf &startColor, const ColorAf &endColor) {
  return nvgBoxGradient(get(), bounds.getX1(), bounds.getY1(),
                        bounds.getWidth(), bounds.getWidth(),
                        r, f,
                        reinterpret_cast<const NVGcolor&>(startColor),
                        reinterpret_cast<const NVGcolor&>(endColor));
}
NVGpaint Context::radialGradient(const vec2 &center, float innerRad, float outerRad,
                                 const ColorAf &innerColor, const ColorAf &outerColor) {
  return nvgRadialGradient(get(), center.x, center.y, innerRad, outerRad,
                           reinterpret_cast<const NVGcolor&>(innerColor),
                           reinterpret_cast<const NVGcolor&>(outerColor));
}

NVGpaint Context::imagePattern(float cx, float cy, float w, float h, float angle,
                               const Image &image, float alpha) {
  return nvgImagePattern(get(), cx, cy, w, h, angle, image.id, alpha);
}

// Scissoring //

void Context::scissor(float x, float y, float w, float h) {
  nvgScissor(get(), x, y, w, h);
}
void Context::resetScissor() {
  nvgResetScissor(get());
}

// Paths //

void Context::beginPath() {
  nvgBeginPath(get());
}
void Context::moveTo(float x, float y) {
  nvgMoveTo(get(), x, y);
}
void Context::moveTo(const vec2 &p) {
  moveTo(p.x, p.y);
}
void Context::lineTo(float x, float y) {
  nvgLineTo(get(), x, y);
}
void Context::lineTo(const vec2 &p) {
  lineTo(p.x, p.y);
}
void Context::quadTo(float cx, float cy, float x, float y) {
  nvgQuadTo(get(), cx, cy, x, y);
}
void Context::quadTo(const vec2 &p1, const vec2 &p2) {
  quadTo(p1.x, p1.y, p2.x, p2.y);
}
void Context::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
  nvgBezierTo(get(), c1x, c1y, c2x, c2y, x, y);
}
void Context::bezierTo(const vec2 &p1, const vec2 &p2, const vec2 &p3) {
  bezierTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}
void Context::arcTo(float x1, float y1, float x2, float y2, float radius) {
  nvgArcTo(get(), x1, y1, x2, y2, radius);
}
void Context::arcTo(const vec2 &p1, const vec2 &p2, float radius) {
  arcTo(p1.x, p1.y, p2.x, p2.y, radius);
}
void Context::closePath() {
  nvgClosePath(get());
}
void Context::pathWinding(int dir) {
  nvgPathWinding(get(), dir);
}
void Context::arc(float cx, float cy, float r, float a0, float a1, int dir) {
  nvgArc(get(), cx, cy, r, a0, a1, dir);
}
void Context::arc(const vec2 &center, float r, float a0, float a1, int dir) {
  arc(center.x, center.y, r, a0, a1, dir);
}
void Context::rect(float x, float y, float w, float h) {
  nvgRect(get(), x, y, w, h);
}
void Context::rect(const Rectf &r) {
  rect(r.getX1(), r.getY1(), r.getWidth(), r.getHeight());
}
void Context::roundedRect(float x, float y, float w, float h, float r) {
  nvgRoundedRect(get(), x, y, w, h, r);
}
void Context::roundedRect(const Rectf &rect, float r) {
  roundedRect(rect.getX1(), rect.getY1(), rect.getWidth(), rect.getHeight(), r);
}
void Context::ellipse(float cx, float cy, float rx, float ry) {
  nvgEllipse(get(), cx, cy, rx, ry);
}
void Context::ellipse(const vec2 &center, float rx, float ry) {
  ellipse(center.x, center.y, rx, ry);
}
void Context::circle(float cx, float cy, float r) {
  nvgCircle(get(), cx, cy, r);
}
void Context::circle(const vec2 &center, float radius) {
  circle(center.x, center.y, radius);
}

void Context::fill() {
  nvgFill(get());
}
void Context::stroke() {
  nvgStroke(get());
}

// Text //

int Context::createFont(const std::string &name, const std::string &filename) {
  return nvgCreateFont(get(), name.c_str(), filename.c_str());
}
int Context::findFont(const std::string &name) {
  return nvgFindFont(get(), name.c_str());
}

void Context::fontSize(float size) {
  nvgFontSize(get(), size);
}
void Context::fontBlur(float blur) {
  nvgFontBlur(get(), blur);
}
void Context::fontFaceId(int font) {
  nvgFontFaceId(get(), font);
}
void Context::fontFace(const std::string &font) {
  nvgFontFace(get(), font.c_str());
}

void Context::textLetterSpacing(float spacing) {
  nvgTextLetterSpacing(get(), spacing);
}
void Context::textLineHeight(float lineHeight) {
  nvgTextLineHeight(get(), lineHeight);
}
void Context::textAlign(int align) {
  nvgTextAlign(get(), align);
}

float Context::text(float x, float y, const std::string &str) {
  return nvgText(get(), x, y, str.c_str(), NULL);
}
float Context::text(const vec2 &p, const std::string &str) {
  return text(p.x, p.y, str);
}

void Context::textBox(float x, float y, float breakRowWidth, const std::string &str) {
  nvgTextBox(get(), x, y, breakRowWidth, str.c_str(), NULL);
}
void Context::textBox(const vec2 &p, float breakRowWidth, const std::string &str) {
  textBox(p.x, p.y, breakRowWidth, str);
}
void Context::textBox(const Rectf &rect, const std::string &str) {
  textBox(rect.getX1(), rect.getY1(), rect.getWidth(), str);
}

Rectf Context::textBounds(float x, float y, const std::string &str) {
  Rectf bounds;
  nvgTextBounds(get(), x, y, str.c_str(), NULL, &bounds.x1);
  return bounds;
}
Rectf Context::textBounds(const vec2 &p, const std::string &str) {
  return textBounds(p.x, p.y, str);
}

Rectf Context::textBoxBounds(float x, float y, float breakRowWidth, const std::string &str) {
  Rectf bounds;
  nvgTextBoxBounds(get(), x, y, breakRowWidth, str.c_str(), NULL, &bounds.x1);
  return bounds;
}
Rectf Context::textBoxBounds(const vec2 &p, float breakRowWidth, const std::string &str) {
  return textBoxBounds(p.x, p.y, breakRowWidth, str);
}
Rectf Context::textBoxBounds(const Rectf &rect, const std::string &str) {
  return textBoxBounds(rect.getX1(), rect.getY1(), rect.getWidth(), str);
}

}} // cinder::nvg
