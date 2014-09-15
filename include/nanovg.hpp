#pragma once

#include "nanovg.h"

#include "cinder/MatrixAffine2.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "cinder/Shape2d.h"
#include "cinder/svg/Svg.h"

#include <memory>
#include <string>

namespace cinder { namespace nvg {

class Context {
public:
  using BackingCtx = std::unique_ptr<NVGcontext, std::function<void(NVGcontext*)>>;

  Context(BackingCtx&& ctx);

  static Context create(bool antiAlias = true, bool stencilStrokes = false);

  inline NVGcontext* get() { return mCtx.get(); }

  // Frame //

  inline void beginFrame(int windowWidth, int windowHeight, float devicePixelRatio) {
    nvgBeginFrame(get(), windowWidth, windowHeight, devicePixelRatio);
  }
  inline void beginFrame(const ivec2& windowSize, float devicePixelRatio) {
    beginFrame(windowSize.x, windowSize.y, devicePixelRatio);
  }
  inline void endFrame() {
    nvgEndFrame(get());
  }

  // State Handling //

  inline void save() { nvgSave(get()); }
  inline void restore() { nvgRestore(get()); }
  inline void reset() { nvgReset(get()); }

  // Render Styles //

  inline void strokeColor(const NVGcolor& color) {
    nvgStrokeColor(get(), color);
  }
  inline void strokeColor(const ColorAf& color) {
    strokeColor(reinterpret_cast<const NVGcolor&>(color));
  }
  inline void strokePaint(const NVGpaint& paint) {
    nvgStrokePaint(get(), paint);
  }
  inline void fillColor(const NVGcolor& color) {
    nvgFillColor(get(), color);
  }
  inline void fillColor(const ColorAf& color) {
    fillColor(reinterpret_cast<const NVGcolor&>(color));
  }
  inline void fillPaint(const NVGpaint& paint) {
    nvgFillPaint(get(), paint);
  }

  inline void miterLimit(float limit) { nvgMiterLimit(get(), limit); }
  inline void strokeWidth(float size) { nvgStrokeWidth(get(), size); }
  inline void lineCap(int cap) { nvgLineCap(get(), cap); }
  inline void lineJoin(int join) { nvgLineJoin(get(), join); }

  // Transform //

  inline void resetTransform() {
    nvgResetTransform(get());
  }
  inline void transform(const MatrixAffine2f& mtx) {
    nvgTransform(get(), mtx[0], mtx[1], mtx[2], mtx[3], mtx[4], mtx[5]);
  }
  inline void setTransform(const MatrixAffine2f& mtx) {
    resetTransform();
    transform(mtx);
  }
  inline void translate(float x, float y) {
    nvgTranslate(get(), x, y);
  }
  inline void translate(const vec2& translation) {
    translate(translation.x, translation.y);
  }
  inline void rotate(float angle) { nvgRotate(get(), angle); }
  inline void skewX(float angle) { nvgSkewX(get(), angle); }
  inline void skewY(float angle) { nvgSkewY(get(), angle); }
  inline void scale(float x, float y) { nvgScale(get(), x, y); }
  inline void scale(const vec2& s) { scale(s.x, s.y); }

  MatrixAffine2f currentTransform() {
    MatrixAffine2f xform;
    nvgCurrentTransform(mCtx.get(), &xform[0]);
    return xform;
  }

  // Paints //

  inline NVGpaint linearGradient(float sx, float sy, float ex, float ey,
                                 NVGcolor icol, NVGcolor ocol) {
    return nvgLinearGradient(get(), sx, sy, ex, ey, icol, ocol);
  }
  inline NVGpaint boxGradient(float x, float y, float w, float h, float r, float f,
                              NVGcolor icol, NVGcolor ocol) {
    return nvgBoxGradient(get(), x, y, w, h, r, f, icol, ocol);
  }
  inline NVGpaint radialGradient(float cx, float cy, float inr, float outr,
                                 NVGcolor icol, NVGcolor ocol) {
    return nvgRadialGradient(get(), cx, cy, inr, outr, icol, ocol);
  }
  
  // Scissoring //

  inline void scissor(float x, float y, float w, float h) {
    nvgScissor(get(), x, y, w, h);
  }
  inline void resetScissor() {
    nvgResetScissor(get());
  }

  // Paths //

  inline void beginPath() { nvgBeginPath(get()); }
  inline void moveTo(float x, float y) { nvgMoveTo(get(), x, y); }
  inline void moveTo(const vec2& p) { moveTo(p.x, p.y); }
  inline void lineTo(float x, float y) { nvgLineTo(get(), x, y); }
  inline void lineTo(const vec2& p) { lineTo(p.x, p.y); }
  inline void quadTo(float cx, float cy, float x, float y) {
    nvgQuadTo(get(), cx, cy, x, y);
  }
  inline void quadTo(const vec2& p1, const vec2& p2) {
    quadTo(p1.x, p1.y, p2.x, p2.y);
  }
  inline void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    nvgBezierTo(get(), c1x, c1y, c2x, c2y, x, y);
  }
  inline void bezierTo(const vec2& p1, const vec2& p2, const vec2& p3) {
    bezierTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
  }
  inline void arcTo(float x1, float y1, float x2, float y2, float radius) {
    nvgArcTo(get(), x1, y1, x2, y2, radius);
  }
  inline void arcTo(const vec2& p1, const vec2& p2, float radius) {
    arcTo(p1.x, p1.y, p2.x, p2.y, radius);
  }
  inline void closePath() { nvgClosePath(get()); }
  inline void pathWinding(int dir) { nvgPathWinding(get(), dir); }
  inline void arc(float cx, float cy, float r, float a0, float a1, int dir) {
    nvgArc(get(), cx, cy, r, a0, a1, dir);
  }
  inline void arc(const vec2 &center, float r, float a0, float a1, int dir) {
    arc(center.x, center.y, r, a0, a1, dir);
  }
  inline void rect(float x, float y, float w, float h) {
    nvgRect(get(), x, y, w, h);
  }
  inline void rect(const Rectf& r) {
    rect(r.getX1(), r.getY1(), r.getWidth(), r.getHeight());
  }
  inline void roundedRect(float x, float y, float w, float h, float r) {
    nvgRoundedRect(get(), x, y, w, h, r);
  }
  inline void roundedRect(const Rectf& rect, float r) {
    roundedRect(rect.getX1(), rect.getY1(), rect.getWidth(), rect.getHeight(), r);
  }
  inline void ellipse(float cx, float cy, float rx, float ry) {
    nvgEllipse(get(), cx, cy, rx, ry);
  }
  inline void ellipse(const vec2& center, float rx, float ry) {
    ellipse(center.x, center.y, rx, ry);
  }
  inline void circle(float cx, float cy, float r) {
    nvgCircle(get(), cx, cy, r);
  }
  inline void circle(const vec2& center, float radius) {
    circle(center.x, center.x, radius);
  }

  inline void fill() { nvgFill(get()); }
  inline void stroke() { nvgStroke(get()); }

  // Cinder Types //

  void polyLine(const PolyLine2f& polyline);
  void path2d(const Path2d& path);
  void shape2d(const Shape2d& shape);

  // Cinder SVG //

  void drawSvg(const svg::Doc &svg);

  // Text //

  inline int createFont(const std::string& name, const std::string& filename) {
    return nvgCreateFont(get(), name.c_str(), filename.c_str());
  }
  inline int findFont(const std::string& name) {
    return nvgFindFont(get(), name.c_str());
  }

  inline void fontSize(float size) { nvgFontSize(get(), size); }
  inline void fontBlur(float blur) { nvgFontBlur(get(), blur); }
  inline void fontFaceId(int font) { nvgFontFaceId(get(), font); }
  inline void fontFace(const std::string& font) { nvgFontFace(get(), font.c_str()); }

  inline void textLetterSpacing(float spacing) {
    nvgTextLetterSpacing(get(), spacing);
  }
  inline void textLineHeight(float lineHeight) {
    nvgTextLineHeight(get(), lineHeight);
  }
  inline void textAlign(int align) {
    nvgTextAlign(get(), align);
  }

  inline float text(float x, float y, const std::string& str) {
    return nvgText(get(), x, y, str.c_str(), NULL);
  }
  inline float text(const vec2 &p, const std::string& str) {
    return text(p.x, p.y, str);
  }

  inline void textBox(float x, float y, float breakRowWidth, const std::string& str) {
    nvgTextBox(get(), x, y, breakRowWidth, str.c_str(), NULL);
  }
  inline void textBox(const vec2 &p, float breakRowWidth, const std::string& str) {
    textBox(p.x, p.y, breakRowWidth, str);
  }
  inline void textBox(const Rectf &rect, const std::string& str) {
    textBox(rect.getX1(), rect.getY1(), rect.getWidth(), str);
  }

  inline Rectf textBounds(float x, float y, const std::string& str) {
    Rectf bounds;
    nvgTextBounds(get(), x, y, str.c_str(), NULL, &bounds.x1);
    return bounds;
  }
  inline Rectf textBounds(const vec2 &p, const std::string& str) {
    return textBounds(p.x, p.y, str);
  }

  inline Rectf textBoxBounds(float x, float y, float breakRowWidth, const std::string &str) {
    Rectf bounds;
    nvgTextBoxBounds(get(), x, y, breakRowWidth, str.c_str(), NULL, &bounds.x1);
    return bounds;
  }
  inline Rectf textBoxBounds(const vec2 &p, float breakRowWidth, const std::string &str) {
    return textBoxBounds(p.x, p.y, breakRowWidth, str);
  }
  inline Rectf textBoxBounds(const Rectf &rect, const std::string &str) {
    return textBoxBounds(rect.getX1(), rect.getY1(), rect.getWidth(), str);
  }

private:
  void path2dSegment(Path2d::SegmentType, const vec2 *p1, const vec2 *p2, const vec2 *p3);

  BackingCtx mCtx;
};

}} // cinder::nvg
