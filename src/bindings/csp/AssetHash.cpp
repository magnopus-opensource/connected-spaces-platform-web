#include "../containers/String.h"

#include "CSP/AssetHash.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAssetHash) { emscripten::function("generateAssetHash(assetId)", &csp::GenerateAssetHash); }
