#pragma once

#include "cinder/Matrix.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "cinder/Shape2d.h"

#include "nanovg.h"

#include <memory>
#include <string>

namespace cinder {

namespace svg {
  class Paint;
  class Doc;
} // svg

namespace nvg {

class Image {
protected:
  NVGcontext *mCtx = nullptr;

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

public:
  int id;

  Image() = default;
  Image(Image &&image);
  Image(NVGcontext *ctx, int id);
  ~Image();

  Image &operator=(Image &&image);
};

class Context {
protected:
  using Deleter = void (*)(NVGcontext *);

  std::unique_ptr<NVGcontext, Deleter> mPtr;

  void path2dSegment(Path2d::SegmentType, const vec2 *p1, const vec2 *p2, const vec2 *p3);

public:
  Context(NVGcontext *ptr, Deleter deleter);
  Context(Context&& ctx) : mPtr{ std::move(ctx.mPtr) } {}

  // NOTE(ryan): Allow this class to be extended. Backends may want to bundle some associated data.
  // Subclasses will need to provide their own move constructor.
  virtual ~Context() = default;

  inline NVGcontext *get() { return mPtr.get(); }

  // Conversion //

  NVGpaint convert(const svg::Paint &paint);

  // Frame //

  void beginFrame(int windowWidth, int windowHeight, float devicePixelRatio);
  void beginFrame(const ivec2 &windowSize, float devicePixelRatio);
  void cancelFrame();
  void endFrame();

  // State Handling //

  void save();
  void restore();
  void reset();

  // Render Styles //

  void strokeColor(const NVGcolor &color);
  void strokeColor(const ColorAf &color);
  void strokePaint(const NVGpaint &paint);
  void fillColor(const NVGcolor &color);
  void fillColor(const ColorAf &color);
  void fillPaint(const NVGpaint &paint);

  void strokePaint(const svg::Paint &paint);
  void fillPaint(const svg::Paint &paint);

  void miterLimit(float limit);
  void strokeWidth(float size);
  void lineCap(int cap);
  void lineJoin(int join);

  // Transform //

  void resetTransform();
  void transform(const mat3 &mtx);
  void setTransform(const mat3 &mtx);
  void translate(float x, float y);
  void translate(const vec2 &translation);
  void rotate(float angle);
  void skewX(float angle);
  void skewY(float angle);
  void scale(float x, float y);
  void scale(const vec2 &s);

  mat3 currentTransform();

  // Paints //

  NVGpaint linearGradient(const vec2 &start, const vec2 &end, const ColorAf &startColor,
                          const ColorAf &endColor);
  NVGpaint boxGradient(const Rectf &bounds, float r, float f, const ColorAf &startColor,
                       const ColorAf &endColor);
  NVGpaint radialGradient(const vec2 &center, float innerRad, float outerRad,
                          const ColorAf &innerColor, const ColorAf &outerColor);

  NVGpaint imagePattern(float cx, float cy, float w, float h, float angle, const Image &image,
                        float alpha);

  // Scissoring //

  void scissor(float x, float y, float w, float h);
  void resetScissor();

  // Paths //

  void beginPath();
  void moveTo(float x, float y);
  void moveTo(const vec2 &p);
  void lineTo(float x, float y);
  void lineTo(const vec2 &p);
  void quadTo(float cx, float cy, float x, float y);
  void quadTo(const vec2 &p1, const vec2 &p2);
  void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
  void bezierTo(const vec2 &p1, const vec2 &p2, const vec2 &p3);
  void arcTo(float x1, float y1, float x2, float y2, float radius);
  void arcTo(const vec2 &p1, const vec2 &p2, float radius);
  void closePath();
  void pathWinding(int dir);
  void arc(float cx, float cy, float r, float a0, float a1, int dir);
  void arc(const vec2 &center, float r, float a0, float a1, int dir);
  void rect(float x, float y, float w, float h);
  void rect(const Rectf &r);
  void roundedRect(float x, float y, float w, float h, float r);
  void roundedRect(const Rectf &rect, float r);
  void ellipse(float cx, float cy, float rx, float ry);
  void ellipse(const vec2 &center, float rx, float ry);
  void circle(float cx, float cy, float r);
  void circle(const vec2 &center, float radius);

  void fill();
  void stroke();

  // Cinder Types //

  void polyLine(const PolyLine2f &polyline);
  void path2d(const Path2d &path);
  void shape2d(const Shape2d &shape);

  // Cinder SVG //

  void draw(const svg::Doc &svg);

  // Text //

  int createFont(const std::string &name, const std::string &filename);
  int findFont(const std::string &name);

  void fontSize(float size);
  void fontBlur(float blur);
  void fontFaceId(int font);
  void fontFace(const std::string &font);

  void textLetterSpacing(float spacing);
  void textLineHeight(float lineHeight);
  void textAlign(int align);

  float text(float x, float y, const std::string &str);
  float text(const vec2 &p, const std::string &str);

  void textBox(float x, float y, float breakRowWidth, const std::string &str);
  void textBox(const vec2 &p, float breakRowWidth, const std::string &str);
  void textBox(const Rectf &rect, const std::string &str);

  Rectf textBounds(float x, float y, const std::string &str);
  Rectf textBounds(const vec2 &p, const std::string &str);

  Rectf textBoxBounds(float x, float y, float breakRowWidth, const std::string &str);
  Rectf textBoxBounds(const vec2 &p, float breakRowWidth, const std::string &str);
  Rectf textBoxBounds(const Rectf &rect, const std::string &str);
};

}} // cinder::nvg
