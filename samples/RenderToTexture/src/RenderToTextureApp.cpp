#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Shader.h"

#include "cinder/Rand.h"
#include "cinder/BSpline.h"

#include "ci_nanovg_gl.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

static Colorf randColor() {
  return { Rand::randFloat(), Rand::randFloat(), Rand::randFloat() };
}

class Shape {
  Path2d mPath;
  Colorf mColor;
  vec2 mPosition;

public:
  Shape(Path2d path, vec2 position) : mPath{ move(path) },
    mPosition{ position },
    mColor{ randColor() } {
  }

  void draw(nvg::Context& vg) const {
    vg.beginPath();
    vg.path2d(mPath);
    vg.fillColor(mColor);
    vg.fill();
  }

  vec2 getPosition() {
    return mPosition;
  }

  Rectf getBounds() const {
    return mPath.calcPreciseBoundingBox();
  }
};

class ShapeProxy {
  gl::TextureRef mTexture;
  Rectf mBounds;
  shared_ptr<Shape> mShape;

public:
  ShapeProxy(nvg::Context& vg, shared_ptr<Shape> shape) : mShape{ shape } {
    mBounds = mShape->getBounds();

    // Grow the bounds a little to preserve nice anti-aliasing at the edges.
    mBounds.inflate(vec2(2));

    auto fboSize = mBounds.getSize() * getWindow()->getContentScale();
    auto fbo = gl::Fbo::create(fboSize.x, fboSize.y, gl::Fbo::Format().stencilBuffer());
    gl::ScopedFramebuffer fboScp(fbo);

    gl::viewport(fboSize);
    gl::clear(ColorAf::zero());
    gl::clear(GL_STENCIL_BUFFER_BIT);

    vg.beginFrame(mBounds.getSize(), getWindow()->getContentScale());
    vg.translate(-mBounds.getUpperLeft());
    mShape->draw(vg);
    vg.endFrame();

    mTexture = fbo->getColorTexture();
  }

  void draw() {
    gl::pushModelMatrix();
    gl::draw(mTexture, mBounds);
    gl::popModelMatrix();
  }

  vec2 getPosition() const {
    return mShape->getPosition();
  }
};

class RenderToTextureApp : public App {
  shared_ptr<nvg::Context> mCtx;
  vector<shared_ptr<Shape>> mShapes;
  vector<shared_ptr<ShapeProxy>> mProxies;

  bool mRenderProxies = false;

public:
  void generateShapes();
  void generateShapeTextures();
  void setup() override;
  void update() override;
  void draw() override;
};

void RenderToTextureApp::generateShapes() {
  auto generatePath = [](float radius, int numPoints) {
    // Generate a some random points.
    vector<vec2> points(numPoints);
    generate(points.begin(), points.end(), [&] {
      static mat4 rotation;
      vec2 point = vec2(vec4(randFloat(radius * 0.5f, radius), 0, 0, 1) * rotation);
      rotation *= rotate((float)M_PI * 2.0f / numPoints, vec3(0, 0, 1));
      return point;
    });

    // Create a path from a closed looped BSpline.
    return Path2d{BSpline2f{points, 3, true, false}};
  };

  // Fill our array with shapes.
  mShapes.resize(32);
  generate(mShapes.begin(), mShapes.end(), [&] {
    float radius = randFloat(100, 300);
    int numPoints = randInt(4, 12);

    auto path = generatePath(radius, numPoints);
    auto position = vec2{randFloat(), randFloat()} * vec2{getWindowSize()};

    return make_shared<Shape>(path, position);
  });

  // Generate texture proxies for each shape.
  mProxies.resize(mShapes.size());
  transform(mShapes.begin(), mShapes.end(), mProxies.begin(), [&](shared_ptr<Shape> shape) {
    return make_shared<ShapeProxy>(*mCtx, shape);
  });
}

void RenderToTextureApp::setup() {
  Rand::randomize();
  mCtx = make_shared<nvg::Context>(nvg::createContextGL());
  mCtx->createFont("roboto", getAssetPath("Roboto-Regular.ttf").string());

  getWindow()->getSignalTouchesBegan().connect([&](const TouchEvent event) {
    generateShapes();
  });
  getWindow()->getSignalMouseDown().connect([&](const MouseEvent event) {
    generateShapes();
  });

  generateShapes();
}

void RenderToTextureApp::update() {
}

void RenderToTextureApp::draw() {
  ivec2 windowPixelSize = vec2(getWindowSize()) * getWindowContentScale();

  gl::viewport(windowPixelSize);
  gl::clear(Colorf{0, 0, 0});
  gl::clear(GL_STENCIL_BUFFER_BIT);

  auto& vg = *mCtx;

  auto time = getElapsedSeconds();
  bool showProxies = fmod(getElapsedSeconds(), 10.0) < 5.0;

  if (showProxies) { // Switch every 3 seconds.
    gl::setMatricesWindow(getWindowSize());
    gl::ScopedGlslProg shaderScp(gl::getStockShader(gl::ShaderDef().color()));
    for (auto& proxy : mProxies) {
      gl::ScopedModelMatrix modelScp;
      gl::translate(proxy->getPosition());
      gl::rotate(time, vec3(0, 0, 1));
      proxy->draw();
    }
  }
  else {
    vg.beginFrame(getWindowSize(), getWindowContentScale());
    for (auto& shape : mShapes) {
      vg.save();
      vg.translate(shape->getPosition());
      vg.rotate(time);
      shape->draw(vg);
      vg.restore();
    }
    vg.endFrame();
  }

  string mode = showProxies ? "Texture Proxies" : "NanoVG";
  vg.beginFrame(getWindowSize(), getWindowContentScale());
  vg.translate(10, getWindowHeight() - 10);
  vg.fontFace("roboto");
  vg.fontSize(24);
  vg.fillColor(Colorf::white());
  vg.text(vec2(), "Fps: " + to_string(getAverageFps()));
  vg.text(vec2(0, -20), "Mode: " + mode);
  vg.endFrame();
}

// NanoVG requires a stencil buffer in the main framebuffer and performs it's
// own anti-aliasing by default. We disable opengl's AA and enable stencil here
// to allow for this.
CINDER_APP(RenderToTextureApp, RendererGl(RendererGl::Options().stencil().msaa(0)),
           [](App::Settings *settings) {
  settings->setHighDensityDisplayEnabled();
  settings->setMultiTouchEnabled();
})
