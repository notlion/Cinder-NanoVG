#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"

#include "cinder/svg/Svg.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"

#include "nanovg_gl.hpp"

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
  // Create a NanoVG context without anti-aliasing.
  mNanoVG = make_shared<nvg::Context>(nvg::createContextGL(false));
  mDoc = svg::Doc::create(loadAsset("rainbow_dash.svg"));
}

void HelloSvgApp::update() {
}

void HelloSvgApp::draw() {
  gl::clear(Color{0, 0, 0});
  gl::clear(GL_STENCIL_BUFFER_BIT);

  auto time = getElapsedSeconds();

  mNanoVG->beginFrame(getWindowSize(), getWindowContentScale());
  mNanoVG->translate(getWindowCenter());
  mNanoVG->scale(vec2(lmap<float>(sinf(time * 2), -1, 1, 1, 2)));
  mNanoVG->rotate(time * 0.5);
  mNanoVG->translate(-mDoc->getBounds().getCenter());
  mNanoVG->draw(*mDoc);
  mNanoVG->endFrame();
}

// Since we are disabling anti-aliasing in NanoVG, we enable MSAA here.
CINDER_APP_NATIVE(HelloSvgApp, RendererGl(
  RendererGl::Options().msaa(4).stencil()))
