#pragma once

#include "nanovg.hpp"
#include <memory>

namespace nvg {

std::unique_ptr<Context> createContextGLES3(bool edgeAA = true);

}