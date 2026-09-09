#include "CallbackDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Systems/Log/LogLevels.h"
#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"
#include <cstdint>
#include <functional>

/*
 * Define all the callbacks used in the CSP api. Pairs with declarations in CallbackDeclarations.h
 *
 * The TypeScript signature given to each `DEFINE_CALLBACK` is what appears in the generated .d.ts
 * The Embind name, (ie, BooleanCallback), never appears in generated output, it's just for embind to know about.
 */

DEFINE_CALLBACK(std::function<void()>, VoidCallback, "() => void")

/* Common */
DEFINE_CALLBACK(csp::common::LogSystem::EventCallbackHandler, StringCallback, "(value: string) => void")
DEFINE_CALLBACK(csp::common::EntityFetchCompleteCallback, EntityFetchCompleteCallback, "(entityCount: number) => void")
DEFINE_CALLBACK(csp::common::LogSystem::LogCallbackHandler, LogCallback, "(level: LogLevel, message: string) => void")

/* Multiplayer */
DEFINE_CALLBACK(csp::multiplayer::EntityCreatedCallback, EntityCreatedCallback, "(entity: SpaceEntity | null) => void")
DEFINE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback, "(success: boolean) => void")
