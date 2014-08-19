#pragma once

#include "nanovg.h"

#include "cinder/MatrixAffine2.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "cinder/Shape2d.h"

#include <memory>
#include <string>

namespace nvg {

class Context {
public:
  using BackingCtx = std::unique_ptr<NVGcontext, std::function<void(NVGcontext*)>>;

  Context(BackingCtx&& ctx) : mCtx{std::move(ctx)} {
  }

  void beginFrame(int windowWidth, int windowHeight, float devicePixelRatio);
  void beginFrame(const ci::Vec2i& windowSize, float devicePixelRatio);
  void endFrame();

  // State Handling
  void save();
  void restore();
  void reset();

  // Render Styles
  void strokeColor(const NVGcolor& color);
  void strokeColor(const ci::ColorAf& color);
  void strokePaint(const NVGpaint& paint);
  void fillColor(const NVGcolor& color);
  void fillColor(const ci::ColorAf& color);
  void fillPaint(const NVGpaint& paint);
  void miterLimit(float limit);
  void strokeWidth(float size);
  void lineCap(int cap);
  void lineJoin(int join);

  // Transforms
  void setTransform(const ci::MatrixAffine2f& mtx);
  void resetTransform();
  void transform(const ci::MatrixAffine2f& mtx);
  void translate(float x, float y);
  void translate(const ci::Vec2f& translation);
  void rotate(float angle);
  void skewX(float angle);
  void skewY(float angle);
  void scale(float x, float y);
  void scale(const ci::Vec2f& s);
  ci::MatrixAffine2f currentTransform();

  // Paints
  NVGpaint linearGradient(float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocol);
  NVGpaint boxGradient(float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol);
  NVGpaint radialGradient(float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol);
  NVGpaint imagePattern(float ox, float oy, float ex, float ey, float angle, int image, int repeat, float alpha);

  // Scissoring
  void scissor(float x, float y, float w, float h);
  void resetScissor();

  // Paths
  void beginPath();
  void moveTo(float x, float y);
  void moveTo(const ci::Vec2f& p);
  void lineTo(float x, float y);
  void lineTo(const ci::Vec2f& p);
  void quadTo(float cx, float cy, float x, float y);
  void quadTo(const ci::Vec2f& p1, const ci::Vec2f& p2);
  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
  void bezierTo(const ci::Vec2f& p1, const ci::Vec2f& p2, const ci::Vec2f& p3);
  void arcTo(float x1, float y1, float x2, float y2, float radius);
  void closePath();
  void pathWinding(int dir);
  void arc(float cx, float cy, float r, float a0, float a1, int dir);
  void rect(float x, float y, float w, float h);
  void rect(const ci::Rectf& r);
  void roundedRect(float x, float y, float w, float h, float r);
  void roundedRect(const ci::Rectf& rect, float r);
  void ellipse(float cx, float cy, float rx, float ry);
  void circle(float cx, float cy, float r);
  void polyLine(const ci::PolyLine2f& polyline);
  void shape2d(const ci::Shape2d& shape);
  void fill();
  void stroke();

  // Text
  int createFont(const std::string& name, const std::string& filename);
  int findFont(const std::string& name);
  void fontSize(float size);
  void fontBlur(float blur);
  void textLetterSpacing(float spacing);
  void textLineHeight(float lineHeight);
  void textAlign(int align);
  void fontFaceId(int font);
  void fontFace(const std::string& font);
  float text(float x, float y, const std::string& string);
  void textBox(float x, float y, float breakRowWidth, const std::string& string);
  ci::Rectf textBounds(float x, float y, const std::string& string);
  ci::Rectf textBoxBounds(float x, float y, float breakRowWidth, const std::string& string);

private:
  BackingCtx mCtx;
};

} // nvg
