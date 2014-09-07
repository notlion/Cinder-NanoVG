#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"

#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"

#include "nanovg.hpp"

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
  mNanoVG = make_shared<nvg::Context>(nvg::createContext());
  mDoc = svg::Doc::create(getAssetPath("tiger.svg"));
}

void HelloSvgApp::update() {
}

void HelloSvgApp::draw() {
  gl::clear(Color{0, 0, 0});
  gl::clear(GL_STENCIL_BUFFER_BIT);

  auto time = getElapsedSeconds();

  mNanoVG->beginFrame(getWindowSize(), getWindowContentScale());
  mNanoVG->translate(getWindowCenter());
  mNanoVG->scale(vec2{1} * lmap<float>(sinf(time * 2), -1, 1, 0.5, 2.0f));
  mNanoVG->rotate(time * -0.5);
  mNanoVG->translate(vec2{1} * -100.0f);
  mNanoVG->drawSvg(*mDoc);
  mNanoVG->endFrame();
}

// NanoVG requires a stencil buffer in the main framebuffer and performs it's
// own anti-aliasing by default. We disable opengl's AA and enable stencil here
// to allow for this.
CINDER_APP_NATIVE(HelloSvgApp, RendererGl{
  RendererGl::Options()
    .antiAliasing(RendererGl::AA_NONE)
    .stencil()
})
