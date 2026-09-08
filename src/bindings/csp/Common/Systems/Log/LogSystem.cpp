#include "../../../../containers/String.h"
#include "../../../CallbackDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Systems/Log/LogLevels.h"
#include "CSP/Common/Systems/Log/LogSystem.h"

#include "emscripten/bind.h"
#include <functional>
#include <utility>

EMSCRIPTEN_BINDINGS(CSPLogSystem)
{
    emscripten::class_<csp::common::LogSystem>("LogSystem")
        //Const cast exists due to CSP having an inappropriate non-const method. They may remove, in which case, remove this.
        .property(
            "systemLevel", +[](const csp::common::LogSystem& self) { return const_cast<csp::common::LogSystem&>(self).GetSystemLevel(); },
            +[](csp::common::LogSystem& self, csp::common::LogLevel value) { self.SetSystemLevel(value); })
        .function(
            "setLogCallback(callback)", +[](csp::common::LogSystem& self, LogCallback callback) { self.SetLogCallback(ToNativeCallback(callback)); })
        .function(
            "setEventCallback(callback)", +[](csp::common::LogSystem& self, StringCallback callback) { self.SetEventCallback(ToNativeCallback(callback)); })
        .function(
            "setBeginMarkerCallback(callback)", +[](csp::common::LogSystem& self, StringCallback callback) { self.SetBeginMarkerCallback(ToNativeCallback(callback)); })
        .function(
            "setEndMarkerCallback(callback)",
            +[](csp::common::LogSystem& self, VoidCallback callback) {
                // Underlying CSP api is weird cause of legacy wrapper gen constraints. Redundant arg can be deleted once wrapper gen migration is over.
                std::function<void()> native = ToNativeCallback(callback);
                self.SetEndMarkerCallback([native = std::move(native)](void*) { native(); });
            })
        .function("loggingEnabled(level)", &csp::common::LogSystem::LoggingEnabled)
        .function("logMsg(level, message)", &csp::common::LogSystem::LogMsg)
        .function("logEvent(event)", &csp::common::LogSystem::LogEvent)
        .function("beginMarker(marker)", &csp::common::LogSystem::BeginMarker)
        .function("endMarker", &csp::common::LogSystem::EndMarker)
        .function("clearAllCallbacks", &csp::common::LogSystem::ClearAllCallbacks);
}
