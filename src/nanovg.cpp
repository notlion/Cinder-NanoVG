#include "nanovg.hpp"

using namespace std;
using namespace ci;

namespace nvg {

void Context::beginFrame(int windowWidth, int windowHeight, float devicePixelRatio) {
  nvgBeginFrame(mCtx.get(), windowWidth, windowHeight, devicePixelRatio);
}

void Context::beginFrame(const Vec2i& windowSize, float devicePixelRatio) {
  beginFrame(windowSize.x, windowSize.y, devicePixelRatio);
}

void Context::endFrame() {
  nvgEndFrame(mCtx.get());
}


// State Handling

void Context::save() {
  nvgSave(mCtx.get());
}

void Context::restore() {
  nvgRestore(mCtx.get());
}

void Context::reset() {
  nvgReset(mCtx.get());
}


// Render Styles

void Context::strokeColor(const NVGcolor& color) {
  nvgStrokeColor(mCtx.get(), color);
}

void Context::strokeColor(const ColorAf& color) {
  strokeColor(reinterpret_cast<const NVGcolor&>(color));
}

void Context::fillColor(const NVGcolor& color) {
  nvgFillColor(mCtx.get(), color);
}

void Context::fillColor(const ColorAf& color) {
  fillColor(reinterpret_cast<const NVGcolor&>(color));
}

void Context::strokePaint(const NVGpaint& paint) {
  nvgStrokePaint(mCtx.get(), paint);
}

void Context::fillPaint(const NVGpaint& paint) {
  nvgFillPaint(mCtx.get(), paint);
}

void Context::miterLimit(float limit) {
  nvgMiterLimit(mCtx.get(), limit);
}

void Context::strokeWidth(float size) {
  nvgStrokeWidth(mCtx.get(), size);
}

void Context::lineCap(int cap) {
  nvgLineCap(mCtx.get(), cap);
}

void Context::lineJoin(int join) {
  nvgLineJoin(mCtx.get(), join);
}


// Transform

void Context::setTransform(const MatrixAffine2f& mtx) {
  resetTransform();
  transform(mtx);
}

void Context::resetTransform() {
  nvgResetTransform(mCtx.get());
}

void Context::transform(const MatrixAffine2f& mtx) {
  nvgTransform(mCtx.get(), mtx[0], mtx[1], mtx[2], mtx[3], mtx[4], mtx[5]);
}

void Context::translate(float x, float y) {
  nvgTranslate(mCtx.get(), x, y);
}

void Context::translate(const Vec2f& translation) {
  translate(translation.x, translation.y);
}

void Context::rotate(float angle) {
  nvgRotate(mCtx.get(), angle);
}

void Context::skewX(float angle) {
  nvgSkewX(mCtx.get(), angle);
}

void Context::skewY(float angle) {
  nvgSkewY(mCtx.get(), angle);
}

void Context::scale(float x, float y) {
  nvgScale(mCtx.get(), x, y);
}

void Context::scale(const Vec2f& s) {
  scale(s.x, s.y);
}

MatrixAffine2f Context::currentTransform() {
  MatrixAffine2f xform;
  nvgCurrentTransform(mCtx.get(), &xform[0]);
  return xform;
}


// Paint

NVGpaint Context::linearGradient(float sx, float sy, float ex, float ey,
                                 NVGcolor icol, NVGcolor ocol) {
  return nvgLinearGradient(mCtx.get(), sx, sy, ex, ey, icol, ocol);
}

NVGpaint Context::boxGradient(float x, float y, float w, float h, float r, float f,
                              NVGcolor icol, NVGcolor ocol) {
  return nvgBoxGradient(mCtx.get(), x, y, w, h, r, f, icol, ocol);
}

NVGpaint Context::radialGradient(float cx, float cy, float inr, float outr,
                                 NVGcolor icol, NVGcolor ocol) {
  return nvgRadialGradient(mCtx.get(), cx, cy, inr, outr, icol, ocol);
}

NVGpaint Context::imagePattern(float ox, float oy, float ex, float ey, float angle,
                               int image, int repeat, float alpha) {
  return nvgImagePattern(mCtx.get(), ox, oy, ex, ey, angle, image, repeat, alpha);
}


// Scissoring

void Context::scissor(float x, float y, float w, float h) {
  nvgScissor(mCtx.get(), x, y, w, h);
}

void Context::resetScissor() {
  nvgResetScissor(mCtx.get());
}


// Paths

void Context::beginPath() {
  nvgBeginPath(mCtx.get());
}

void Context::moveTo(float x, float y) {
  nvgMoveTo(mCtx.get(), x, y);
}

void Context::moveTo(const Vec2f& p) {
  moveTo(p.x, p.y);
}

void Context::lineTo(float x, float y) {
  nvgLineTo(mCtx.get(), x, y);
}

void Context::lineTo(const Vec2f& p) {
  lineTo(p.x, p.y);
}

void Context::quadTo(float cx, float cy, float x, float y) {
  nvgQuadTo(mCtx.get(), cx, cy, x, y);
}

void Context::quadTo(const Vec2f& p1, const ci::Vec2f& p2) {
  quadTo(p1.x, p1.y, p2.x, p2.y);
}

void Context::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
  nvgBezierTo(mCtx.get(), c1x, c1y, c2x, c2y, x, y);
}

void Context::bezierTo(const Vec2f& p1, const Vec2f& p2, const Vec2f& p3) {
  bezierTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void Context::arcTo(float x1, float y1, float x2, float y2, float radius) {
  nvgArcTo(mCtx.get(), x1, y1, x2, y2, radius);
}

void Context::closePath() {
  nvgClosePath(mCtx.get());
}

void Context::pathWinding(int dir) {
  nvgPathWinding(mCtx.get(), dir);
}

void Context::arc(float cx, float cy, float r, float a0, float a1, int dir) {
  nvgArc(mCtx.get(), cx, cy, r, a0, a1, dir);
}

void Context::rect(float x, float y, float w, float h) {
  nvgRect(mCtx.get(), x, y, w, h);
}

void Context::rect(const Rectf& r) {
  rect(r.getX1(), r.getY1(), r.getWidth(), r.getHeight());
}

void Context::roundedRect(float x, float y, float w, float h, float r) {
  nvgRoundedRect(mCtx.get(), x, y, w, h, r);
}

void Context::roundedRect(const Rectf& rect, float r) {
  nvgRoundedRect(mCtx.get(), rect.getX1(), rect.getY1(), rect.getWidth(), rect.getHeight(), r);
}

void Context::ellipse(float cx, float cy, float rx, float ry) {
  nvgEllipse(mCtx.get(), cx, cy, rx, ry);
}

void Context::circle(float cx, float cy, float r) {
  nvgCircle(mCtx.get(), cx, cy, r);
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

using Path2dVisitor = function<void(Path2d::SegmentType, const Vec2f*, const Vec2f*, const Vec2f*)>;

static void iterateShape2d(const Shape2d& shape, Path2dVisitor visit) {
  for (auto& contour : shape.getContours()) {
    const Vec2f* p1 = nullptr, *p2 = nullptr, *p3 = &contour.getPoint(0);

    visit(Path2d::MOVETO, p1, p2, p3);

    size_t i = 1;
    for (auto& seg : contour.getSegments()) {
      if(seg == Path2d::CLOSE) {
        p1 = p2;
        p2 = p3;
        p3 = &contour.getPoint(0);
      }
      else {
        for (size_t ni = i + Path2d::sSegmentTypePointCounts[seg]; i < ni; ++i) {
          p1 = p2;
          p2 = p3;
          p3 = &contour.getPoint(i);
        }
      }

      visit(seg, p1, p2, p3);
    }
  }
}

void Context::shape2d(const Shape2d& shape) {
  iterateShape2d(shape, [this](Path2d::SegmentType type, const Vec2f* p1,
                                                         const Vec2f* p2,
                                                         const Vec2f* p3) {
    switch (type) {
      case Path2d::MOVETO:  moveTo(*p3); break;
      case Path2d::QUADTO:  quadTo(*p2, *p3); break;
      case Path2d::CUBICTO: bezierTo(*p1, *p2, *p3); break;
      case Path2d::LINETO:
      case Path2d::CLOSE:
        lineTo(*p3); break;
    }
  });
}

void Context::fill() {
  nvgFill(mCtx.get());
}

void Context::stroke() {
  nvgStroke(mCtx.get());
}


// Text

int Context::createFont(const std::string& name, const std::string& filename) {
  return nvgCreateFont(mCtx.get(), name.c_str(), filename.c_str());
}

int Context::findFont(const std::string& name) {
  return nvgFindFont(mCtx.get(), name.c_str());
}

void Context::fontSize(float size) {
  nvgFontSize(mCtx.get(), size);
}

void Context::fontBlur(float blur) {
  nvgFontBlur(mCtx.get(), blur);
}

void Context::textLetterSpacing(float spacing) {
  nvgTextLetterSpacing(mCtx.get(), spacing);
}

void Context::textLineHeight(float lineHeight) {
  nvgTextLineHeight(mCtx.get(), lineHeight);
}

void Context::textAlign(int align) {
  nvgTextAlign(mCtx.get(), align);
}

void Context::fontFaceId(int font) {
  nvgFontFaceId(mCtx.get(), font);
}

void Context::fontFace(const std::string& font) {
  nvgFontFace(mCtx.get(), font.c_str());
}

float Context::text(float x, float y, const std::string& string) {
  return nvgText(mCtx.get(), x, y, string.c_str(), NULL);
}

void Context::textBox(float x, float y, float breakRowWidth, const std::string& string) {
  nvgTextBox(mCtx.get(), x, y, breakRowWidth, string.c_str(), NULL);
}

Rectf Context::textBounds(float x, float y, const std::string& string) {
  Rectf bounds;
  nvgTextBounds(mCtx.get(), x, y, string.c_str(), NULL, &bounds.x1);
  return bounds;
}

Rectf Context::textBoxBounds(float x, float y, float breakRowWidth, const std::string& string) {
  Rectf bounds;
  nvgTextBoxBounds(mCtx.get(), x, y, breakRowWidth, string.c_str(), NULL, &bounds.x1);
  return bounds;
}

} // nvg
