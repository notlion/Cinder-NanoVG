#pragma once

#include "cinder/svg/Svg.h"
#include <vector>

namespace cinder { namespace nvg {

class Context;

class SvgRenderer : public svg::Renderer {
  Context &mCtx; // TODO(ryan): This should probably be a managed pointer..

  std::vector<MatrixAffine2f> mMatrixStack;
  std::vector<svg::Paint> mFillStack, mStrokeStack;
  std::vector<float> mFillOpacityStack, mStrokeOpacityStack;
  std::vector<float> mStrokeWidthStack;
  std::vector<int> mLineJoinStack, mLineCapStack;

public:
  SvgRenderer(Context &ctx);

  bool prepareFill(const svg::Node &node);
  bool prepareStroke(const svg::Node &node);
  void fillAndStroke(const svg::Node &node);

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
  void drawTextSpan(const svg::TextSpan &span) override {}

  void pushMatrix(const MatrixAffine2f &m) override;
  void popMatrix() override;
  void pushStyle(const svg::Style &style) override {};
  void popStyle(const svg::Style &style) override {};
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

  // TODO(ryan): Implement text rendering
  void pushTextPen(const Vec2f &penPos) override {}
  void popTextPen() override {}
  void pushTextRotation(float rotation) override {}
  void popTextRotation() override {}
};

}} // cinder::nvg
