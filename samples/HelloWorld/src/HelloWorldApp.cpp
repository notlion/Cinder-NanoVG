#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "nanovg_gl2.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class HelloWorldApp : public AppNative {
public:
  void prepareSettings(Settings* settings);
  void setup();
  void update();
  void draw();

private:
  nvg::ContextPtr mNanoVG;
  PolyLine2f mTriangle;
};

void HelloWorldApp::prepareSettings(Settings* settings) {
  settings->enableHighDensityDisplay();
}

void HelloWorldApp::setup() {
  mNanoVG = nvg::createContextGL2();

  // Load a font
  mNanoVG->createFont("roboto", getAssetPath("Roboto-Regular.ttf").string());

  // Create triangle PolyLine
  int numPts = 3;
  for (size_t i = 0; i < numPts; ++i) {
    float u = static_cast<float>(i) / static_cast<float>(numPts);
    float t = u * M_PI * 2.0f;
    mTriangle.push_back({cosf(t), sinf(t)});
  }
  mTriangle.setClosed();
}

void HelloWorldApp::update() {
}

void HelloWorldApp::draw() {
  gl::clear(Color(0, 0, 0));

  // Store a reference so we can use dot-notation.
  auto& vg = *mNanoVG;

  auto time = getElapsedSeconds();

  vg.beginFrame(getWindowSize(), getWindowContentScale());
  vg.translate(getWindowCenter());

  // Draw Hue Wheel
  vg.save();
  {
    float sc = 1.0f + sinf(time * 4.0f) * 0.5f;
    vg.scale(sc, sc);
    vg.rotate(time);
    for (int n = 64, i = 0; i < n; ++i) {
      float u1 = (float)i / (float)n;
      float u2 = (float)(i + 1) / (float)n;
      float t1 = u1 * M_PI * 2.0f;
      float t2 = u2 * M_PI * 2.0f;
      float r1 = 200.0f + sinf(u1 * M_PI * 12.0f + time * 16.0f) * 25.0f;
      float r2 = 200.0f + sinf(u2 * M_PI * 12.0f + time * 16.0f) * 25.0f;
      vg.beginPath();
      vg.moveTo(0, 0);
      vg.lineTo(cosf(t1) * r1, sinf(t1) * r1);
      vg.lineTo(cosf(t2) * r2, sinf(t2) * r2);
      vg.closePath();
      vg.fillColor(ColorAf{CM_HSV, u1, 1.0f, 1.0f});
      vg.fill();
    }
  }
  vg.restore();

  // Draw Triangle
  vg.save();
  {
    vg.rotate(-time);
    float r = 100.0f - sinf(time * 4.0f) * 40.0f;
    vg.scale(r, r);
    vg.beginPath();
    vg.polyLine(mTriangle);
    vg.strokeWidth(10 / r);
    vg.strokeColor(ColorAf{1.0f, 1.0f, 1.0f, 0.75f});
    vg.stroke();
  }
  vg.restore();

  // Draw Arc
  {
    float r = 200.0f;
    vg.beginPath();
    vg.arc(0, 0, r, -M_PI_2, fmod(time, M_PI * 2.0f) - M_PI_2, NVG_CW);
    vg.strokeColor(ColorAf{1.0f, 1.0f, 1.0f});
    vg.strokeWidth(2);
    vg.stroke();
  }

  // Draw Text
  vg.fontSize(50);
  vg.fontFace("roboto");
  vg.textAlign(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  vg.textLetterSpacing(-4.0);
  vg.fillColor(ColorAf{0.0f, 0.0f, 0.0f});
  vg.text(0, 0, "hello, world");

  vg.endFrame();
}

CINDER_APP_NATIVE(HelloWorldApp, RendererGl{RendererGl::AA_NONE})
