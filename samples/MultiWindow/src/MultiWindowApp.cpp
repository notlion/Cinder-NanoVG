#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/PolyLine.h"
#include "cinder/Rand.h"
#include "nanovg_gl2.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

struct WindowData {
  nvg::ContextPtr ctx;
  PolyLine2f polygon;
  uint32_t id;

  WindowData(uint32_t id)
  : ctx{ nvg::createContextGL2() }, id{ id }
  {
    // Load a font.
    ctx->createFont("roboto", getAssetPath("Roboto-Regular.ttf").string());

    // Create some geometry to draw.
    int numVerts = Rand::randInt(8);
    float scale = 100;
    for (int i = 0; i < numVerts; ++i) {
      float u = float(i) / float(numVerts);
      float t = u * M_PI * 2.0f;
      polygon.push_back({ cosf(t) * scale, sinf(t) * scale });
    }
    polygon.setClosed();
  }
};

class MultiWindowApp : public AppNative {
public:
  void prepareSettings(Settings* settings);
  void setup();
  void update();
  void draw();
  void spawnWindow();
};

void MultiWindowApp::prepareSettings(Settings* settings) {
  settings->enableHighDensityDisplay();
}

void MultiWindowApp::setup() {
  getWindow()->setUserData(new WindowData(getNumWindows()));
  spawnWindow();
}

void MultiWindowApp::spawnWindow() {
  auto newWindow = createWindow(Window::Format().size(getWindowSize()));
  newWindow->setUserData(new WindowData(getNumWindows()));
}

void MultiWindowApp::update() {
}

void MultiWindowApp::draw() {
  auto data = getWindow()->getUserData<WindowData>();
  auto& vg = *data->ctx;

  gl::clear(Color(0, 0, 0));

  vg.beginFrame(getWindowWidth(), getWindowHeight(), getWindowContentScale());

  vg.translate(getWindowCenter());

  vg.save();
  vg.rotate(getElapsedSeconds());
  vg.beginPath();
  vg.polyLine(data->polygon);
  vg.strokeWidth(10);
  vg.strokeColor(Colorf::white());
  vg.stroke();
  vg.restore();

  vg.fontFace("roboto");
  vg.fontSize(100);
  vg.textAlign(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  vg.text(0, 0, to_string(data->id));

  vg.endFrame();
}

CINDER_APP_NATIVE(MultiWindowApp, RendererGl{RendererGl::AA_NONE})
