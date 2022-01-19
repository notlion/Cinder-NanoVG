#include <memory>


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Color.h"
#include "cinder/PolyLine.h"

#include "ci_nanovg_gl.hpp"

using namespace ci;
using namespace ci::app;

class SimpleApp : public App {
public:
  void setup() override;
  void draw() override;

private:
  std::shared_ptr<nvg::Context> mNanoVG;
};

void SimpleApp::setup() {
  mNanoVG = std::make_shared<nvg::Context>(nvg::createContextGL());
}


void SimpleApp::draw() {
  gl::clear(Color(0, 0, 0));

  mNanoVG->beginFrame(getWindowSize(), getWindowContentScale());
  mNanoVG->translate(getWindowCenter());

  // Draw Arc
  {
    float r = 200.0f;
    mNanoVG->beginPath();
    mNanoVG->arc(0, 0, r, -M_PI * 0.5f, (M_PI * 2.0f) - M_PI * 0.5f, NVG_CW);
    mNanoVG->strokeColor(ColorAf{1.0f, 1.0f, 1.0f});
    mNanoVG->strokeWidth(2);
    mNanoVG->stroke();
  }

  mNanoVG->endFrame();
}

// NanoVG requires a stencil buffer in the main framebuffer and performs it's
// own anti-aliasing by default. We disable opengl's AA and enable stencil here
// to allow for this.
CINDER_APP(SimpleApp, RendererGl(RendererGl::Options().stencil().msaa(0)),
           [](App::Settings *settings) {
               settings->setHighDensityDisplayEnabled();
           })
