#pragma once

#include "nanovg.hpp"
#include <memory>

namespace nvg {

std::unique_ptr<Context> createContextGLES2(bool edgeAA = true);

}