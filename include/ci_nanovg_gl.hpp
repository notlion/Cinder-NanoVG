#pragma once

#include "ci_nanovg.hpp"

namespace cinder { namespace nvg {

Context createContextGL(bool antiAlias = true, bool stencilStrokes = false);

}} // cinder::nvg
