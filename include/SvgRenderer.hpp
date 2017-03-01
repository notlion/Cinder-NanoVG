#pragma once

#include "cinder/svg/Svg.h"
#include <vector>

namespace cinder { namespace nvg {

class Context;

class SvgRenderer : public svg::Renderer {
  Context &mCtx; // TODO(ryan): This should probably be a managed pointer..

  std::vector<mat3> mMatrixStack;

  std::vector<svg::Paint> mFillStack, mStrokeStack;
  std::vector<float> mFillOpacityStack, mStrokeOpacityStack;
  std::vector<float> mStrokeWidthStack;
  std::vector<int> mLineJoinStack, mLineCapStack;

  std::vector<vec2> mTextPenStack;
  std::vector<float> mTextRotationStack;

public:
  SvgRenderer(Context &ctx);

  bool prepareFill();
  bool prepareStroke();
  void fillAndStroke();

  void pushGroup(const svg::Group &group, float opacity) override {};
  void popGroup() override {};

  void drawPath(const svg::Path &path) override;
  void drawPolyline(const svg::Polyline &polyline) override;
  void drawPolygon(const svg::Polygon &polygon) override;
  void drawLine(const svg::Line &line) override;
  void drawRect(const svg::Rect &rect) override;
  void drawCircle(const svg::Circle &circle) override;
  void drawEllipse(const svg::Ellipse &ellipse) override;
  void drawImage(const svg::Image &image) override {}
  void drawTextSpan(const svg::TextSpan &span) override;

  void pushMatrix(const mat3 &m) override;
  void popMatrix() override;
  void pushStyle(const svg::Style &style) override {};
  void popStyle() override {};
  void pushFill(const class svg::Paint &paint) override;
  void popFill() override;
  void pushStroke(const class svg::Paint &paint) override;
  void popStroke() override;
  void pushFillOpacity(float opacity ) override;
  void popFillOpacity() override;
  void pushStrokeOpacity(float opacity) override;
  void popStrokeOpacity() override;
  void pushStrokeWidth(float width) override;
  void popStrokeWidth() override;
  void pushFillRule(svg::FillRule rule) override;
  void popFillRule() override;
  void pushLineCap(svg::LineCap lineCap) override;
  void popLineCap() override;
  void pushLineJoin(svg::LineJoin lineJoin) override;
  void popLineJoin() override;

  void pushTextPen(const vec2 &penPos) override;
  void popTextPen() override;
  void pushTextRotation(float rotation) override;
  void popTextRotation() override;
};

}} // cinder::nvg
