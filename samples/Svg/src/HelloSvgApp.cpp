#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "nanovg_gl2.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class HelloSvgApp : public AppNative {
  shared_ptr<nvg::Context> mNanoVG;
  svg::DocRef mDoc;

public:
  void prepareSettings(Settings* settings);
  void setup();
  void update();
  void draw();
};

void HelloSvgApp::prepareSettings(Settings* settings) {
  settings->enableHighDensityDisplay();
}

void HelloSvgApp::setup() {
  mNanoVG = make_shared<nvg::Context>(nvg::createContextGL2());
  mDoc = svg::Doc::create(getAssetPath("tiger.svg"));
}

void HelloSvgApp::update() {
}

void HelloSvgApp::draw() {
  gl::clear(Color(0, 0, 0));
  glClear(GL_STENCIL_BUFFER_BIT);

  auto time = getElapsedSeconds();

  mNanoVG->beginFrame(getWindowSize(), getWindowContentScale());
  mNanoVG->translate(getWindowCenter());
  mNanoVG->rotate(time);
  mNanoVG->scale(Vec2f::one() * lmap<float>(sinf(time), -1, 1, 0.5, 5.0f));
  mNanoVG->drawSvg(*mDoc);
  mNanoVG->endFrame();
}

CINDER_APP_NATIVE(HelloSvgApp, RendererGl{RendererGl::AA_NONE})
