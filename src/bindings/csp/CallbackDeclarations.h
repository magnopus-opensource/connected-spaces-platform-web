#pragma once

#include "../async/Callbacks.h"

#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/Systems/Log/LogSystem.h"

#include <functional>

/*
 * Declare all the callbacks used in the CSP api. Pairs with definitions in CallbackDeclarations.cpp
 * Often times the Embind name given matches the CSP name, so fully qualify the CSP name declarations here to be safe.
 */

DECLARE_CALLBACK(std::function<void()>, VoidCallback)

/* Common */
DECLARE_CALLBACK(csp::common::LogSystem::EventCallbackHandler, StringCallback)
DECLARE_CALLBACK(csp::common::EntityFetchCompleteCallback, EntityFetchCompleteCallback)
DECLARE_CALLBACK(csp::common::LogSystem::LogCallbackHandler, LogCallback)

/* Multiplayer */
DECLARE_CALLBACK(csp::multiplayer::EntityCreatedCallback, EntityCreatedCallback)
DECLARE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback)
