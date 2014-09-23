#include "nanovg.hpp"
#include "SvgRenderer.hpp"

namespace cinder { namespace nvg {

SvgRenderer::SvgRenderer(Context &ctx) : mCtx{ ctx } {
  mMatrixStack.emplace_back(MatrixAffine2f::identity());
  mFillStack.push_back(svg::Paint(Color::black()));
  mStrokeStack.push_back(svg::Paint());
  mFillOpacityStack.push_back(1.0f);
  mStrokeOpacityStack.push_back(1.0f);
  mStrokeWidthStack.push_back(1.0f);

  pushLineCap(svg::LINE_CAP_BUTT);
  pushLineJoin(svg::LINE_JOIN_MITER);
}


bool SvgRenderer::prepareFill(const svg::Node &node) {
  const auto &paint = mFillStack.back();

  if (paint.isNone()) {
    return false;
  }
  else {
    float opacity = mFillOpacityStack.back();

    if (paint.isLinearGradient()) {
      // NanoVG currently only supports gradients with 2 colors.
      auto c0 = paint.getColor(0);
      auto c1 = paint.getColor(1);
      c0.a *= opacity;
      c1.a *= opacity;
      mCtx.fillPaint(mCtx.linearGradient(paint.getCoords0(), paint.getCoords1(), c0, c1));
    }
    else if (paint.isRadialGradient()) {
      // TODO(ryan): Implement this.
      return false;
    }
    else {
      ColorAf color = paint.getColor();
      color.a *= opacity;
      mCtx.fillColor(color);
    }

    return true;
  }
}

bool SvgRenderer::prepareStroke(const svg::Node &node) {
  const auto &paint = mStrokeStack.back();

  if (paint.isNone()) {
    return false;
  }
  else {
    ColorAf color = paint.getColor();
    color.a *= mStrokeOpacityStack.back();
    mCtx.strokeColor(color);
    return true;
  }
}

void SvgRenderer::fillAndStroke(const svg::Node &node) {
  if (prepareFill(node)) mCtx.fill();
  if (prepareStroke(node)) mCtx.stroke();
}


void SvgRenderer::drawPath(const svg::Path &path) {
  mCtx.beginPath();
  mCtx.shape2d(path.getShape2d());
  fillAndStroke(path);
}

void SvgRenderer::drawPolyline(const svg::Polyline &polyline) {
  mCtx.beginPath();
  mCtx.polyLine(polyline.getPolyLine());
  fillAndStroke(polyline);
}

void SvgRenderer::drawPolygon(const svg::Polygon &polygon) {
  mCtx.beginPath();
  mCtx.polyLine(polygon.getPolyLine());
  fillAndStroke(polygon);
}

void SvgRenderer::drawLine(const svg::Line &line) {
  mCtx.beginPath();
  mCtx.moveTo(line.getPoint1());
  mCtx.lineTo(line.getPoint2());
  fillAndStroke(line);
}

void SvgRenderer::drawRect(const svg::Rect &rect) {
  mCtx.beginPath();
  mCtx.rect(rect.getRect());
  fillAndStroke(rect);
}

void SvgRenderer::drawCircle(const svg::Circle &circle) {
  mCtx.beginPath();
  mCtx.circle(circle.getCenter(), circle.getRadius());
  fillAndStroke(circle);
}

void SvgRenderer::drawEllipse(const svg::Ellipse &ellipse) {
  mCtx.beginPath();
  mCtx.ellipse(ellipse.getCenter(), ellipse.getRadiusX(), ellipse.getRadiusY());
  fillAndStroke(ellipse);
}


void SvgRenderer::pushMatrix(const MatrixAffine2f &top) {
  MatrixAffine2f m = mMatrixStack.back() * top;
  mMatrixStack.emplace_back(m);
}
void SvgRenderer::popMatrix() {
  mMatrixStack.pop_back();
}

void SvgRenderer::pushFill(const class svg::Paint &paint) {
  mFillStack.emplace_back(paint);
}
void SvgRenderer::popFill() {
  mFillStack.pop_back();
}

void SvgRenderer::pushStroke(const class svg::Paint &paint) {
  mStrokeStack.emplace_back(paint);
}
void SvgRenderer::popStroke() {
  mStrokeStack.pop_back();
}

void SvgRenderer::pushFillOpacity(float opacity ) {
  mFillOpacityStack.emplace_back(opacity);
}
void SvgRenderer::popFillOpacity() {
  mFillOpacityStack.pop_back();
}

void SvgRenderer::pushStrokeOpacity(float opacity) {
  mStrokeOpacityStack.emplace_back(opacity);
}
void SvgRenderer::popStrokeOpacity() {
  mStrokeOpacityStack.pop_back();
}

void SvgRenderer::pushStrokeWidth(float width) {
  mStrokeWidthStack.emplace_back(width);
}
void SvgRenderer::popStrokeWidth() {
  mStrokeWidthStack.pop_back();
}

void SvgRenderer::pushFillRule(svg::FillRule rule) {
  // Fill rules other than even-odd are not supported by NanoVG.
}
void SvgRenderer::popFillRule() {
}

void SvgRenderer::pushLineCap(svg::LineCap lineCap) {
  int cap = lineCap == svg::LINE_CAP_ROUND  ? NVG_ROUND :
            lineCap == svg::LINE_CAP_SQUARE ? NVG_SQUARE : NVG_BUTT;
  mLineCapStack.emplace_back(cap);
  mCtx.lineCap(cap);
}
void SvgRenderer::popLineCap() {
  mLineCapStack.pop_back();
  mCtx.lineCap(mLineCapStack.back());
}

void SvgRenderer::pushLineJoin(svg::LineJoin lineJoin) {
  int join = lineJoin == svg::LINE_JOIN_ROUND ? NVG_ROUND :
             lineJoin == svg::LINE_JOIN_BEVEL ? NVG_BEVEL : NVG_MITER;
  mLineJoinStack.emplace_back(join);
  mCtx.lineJoin(join);
}
void SvgRenderer::popLineJoin() {
  mLineJoinStack.pop_back();
  mCtx.lineJoin(mLineJoinStack.back());
}

}} // cinder::nvg
