#include "../../../async/Promises.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/ExternalServices/ExternalServiceProxySystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::ExternalServiceProxySystem>(csp::systems::ExternalServiceProxySystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPExternalServiceProxySystem)
{
    emscripten::class_<csp::systems::ExternalServicesOperationParams>("ExternalServicesOperationParams")
        .class_function(
            "create", +[]() { return csp::systems::ExternalServicesOperationParams(); })
        .property("serviceName", &csp::systems::ExternalServicesOperationParams::ServiceName)
        .property("operationName", &csp::systems::ExternalServicesOperationParams::OperationName)
        .property("setHelp", &csp::systems::ExternalServicesOperationParams::SetHelp)
        .property("parameters", &csp::systems::ExternalServicesOperationParams::Parameters);

    emscripten::class_<csp::systems::AgoraUserTokenParams>("AgoraUserTokenParams")
        .class_function(
            "create", +[]() { return csp::systems::AgoraUserTokenParams(); })
        .property("agoraUserId", &csp::systems::AgoraUserTokenParams::AgoraUserId)
        .property("channelName", &csp::systems::AgoraUserTokenParams::ChannelName)
        .property("referenceId", &csp::systems::AgoraUserTokenParams::ReferenceId)
        .property("lifespan", &csp::systems::AgoraUserTokenParams::Lifespan)
        .property("readOnly", &csp::systems::AgoraUserTokenParams::ReadOnly)
        .property("shareAudio", &csp::systems::AgoraUserTokenParams::ShareAudio)
        .property("shareVideo", &csp::systems::AgoraUserTokenParams::ShareVideo)
        .property("shareScreen", &csp::systems::AgoraUserTokenParams::ShareScreen);

    emscripten::class_<csp::systems::ExternalServiceProxySystem, emscripten::base<csp::systems::SystemBase>>("ExternalServiceProxySystem")
        .function(
            "invokeOperation(params)",
            +[](csp::systems::ExternalServiceProxySystem& self, const csp::systems::ExternalServicesOperationParams& params) {
                return Promisify<PromiseOfStringResult>([&](emscripten::val cb) { self.InvokeOperation(params, ToNativeCallback(cb.as<StringResultCallback>())); });
            })
        .function(
            "getAgoraUserToken(params)", +[](csp::systems::ExternalServiceProxySystem& self, const csp::systems::AgoraUserTokenParams& params) {
                return Promisify<PromiseOfStringResult>([&](emscripten::val cb) { self.GetAgoraUserToken(params, ToNativeCallback(cb.as<StringResultCallback>())); });
            });
}
