#include "CSP/CSPFoundation.h"

#include "emscripten/bind.h"
#include <string>

using namespace emscripten;

std::string GetVersionWrapper() { return std::string { csp::CSPFoundation::GetVersion().c_str() }; }

EMSCRIPTEN_BINDINGS(CSPLibModule) { function("GetVersion", &GetVersionWrapper); }