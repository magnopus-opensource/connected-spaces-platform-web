#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/LightSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPLightSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::LightType>("LightType", emscripten::enum_value_type::number)
        .value("Directional", csp::multiplayer::LightType::Directional)
        .value("Point", csp::multiplayer::LightType::Point)
        .value("Spot", csp::multiplayer::LightType::Spot)
        .value("Num", csp::multiplayer::LightType::Num);

    emscripten::enum_<csp::multiplayer::LightShadowType>("LightShadowType", emscripten::enum_value_type::number)
        .value("None", csp::multiplayer::LightShadowType::None)
        .value("Static", csp::multiplayer::LightShadowType::Static)
        .value("Realtime", csp::multiplayer::LightShadowType::Realtime)
        .value("Num", csp::multiplayer::LightShadowType::Num);

    emscripten::enum_<csp::multiplayer::LightCookieType>("LightCookieType", emscripten::enum_value_type::number)
        .value("ImageCookie", csp::multiplayer::LightCookieType::ImageCookie)
        .value("VideoCookie", csp::multiplayer::LightCookieType::VideoCookie)
        .value("NoCookie", csp::multiplayer::LightCookieType::NoCookie);

    emscripten::enum_<csp::multiplayer::LightPropertyKeys>("LightPropertyKeys", emscripten::enum_value_type::number)
        .value("LightType", csp::multiplayer::LightPropertyKeys::LightType)
        .value("Color", csp::multiplayer::LightPropertyKeys::Color)
        .value("Intensity", csp::multiplayer::LightPropertyKeys::Intensity)
        .value("Range", csp::multiplayer::LightPropertyKeys::Range)
        .value("InnerConeAngle", csp::multiplayer::LightPropertyKeys::InnerConeAngle)
        .value("OuterConeAngle", csp::multiplayer::LightPropertyKeys::OuterConeAngle)
        .value("Position", csp::multiplayer::LightPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::LightPropertyKeys::Rotation)
        .value("IsVisible", csp::multiplayer::LightPropertyKeys::IsVisible)
        .value("LightCookieAssetId", csp::multiplayer::LightPropertyKeys::LightCookieAssetId)
        .value("LightCookieAssetCollectionId", csp::multiplayer::LightPropertyKeys::LightCookieAssetCollectionId)
        .value("LightCookieType", csp::multiplayer::LightPropertyKeys::LightCookieType)
        .value("IsARVisible", csp::multiplayer::LightPropertyKeys::IsARVisible)
        .value("ThirdPartyComponentRef", csp::multiplayer::LightPropertyKeys::ThirdPartyComponentRef)
        .value("LightShadowType", csp::multiplayer::LightPropertyKeys::LightShadowType)
        .value("IsVirtualVisible", csp::multiplayer::LightPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::LightPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::LightSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("LightSpaceComponent")
        .property("lightType", &csp::multiplayer::LightSpaceComponent::GetLightType, &csp::multiplayer::LightSpaceComponent::SetLightType)
        .property("lightShadowType", &csp::multiplayer::LightSpaceComponent::GetLightShadowType, &csp::multiplayer::LightSpaceComponent::SetLightShadowType)
        .property("color", &csp::multiplayer::LightSpaceComponent::GetColor, &csp::multiplayer::LightSpaceComponent::SetColor)
        .property("intensity", &csp::multiplayer::LightSpaceComponent::GetIntensity, &csp::multiplayer::LightSpaceComponent::SetIntensity)
        .property("range", &csp::multiplayer::LightSpaceComponent::GetRange, &csp::multiplayer::LightSpaceComponent::SetRange)
        .property("innerConeAngle", &csp::multiplayer::LightSpaceComponent::GetInnerConeAngle, &csp::multiplayer::LightSpaceComponent::SetInnerConeAngle)
        .property("outerConeAngle", &csp::multiplayer::LightSpaceComponent::GetOuterConeAngle, &csp::multiplayer::LightSpaceComponent::SetOuterConeAngle)
        .property("position", &csp::multiplayer::LightSpaceComponent::GetPosition, &csp::multiplayer::LightSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::LightSpaceComponent::GetRotation, &csp::multiplayer::LightSpaceComponent::SetRotation)
        .property("lightCookieAssetId", &csp::multiplayer::LightSpaceComponent::GetLightCookieAssetId, &csp::multiplayer::LightSpaceComponent::SetLightCookieAssetId)
        .property("lightCookieAssetCollectionId", &csp::multiplayer::LightSpaceComponent::GetLightCookieAssetCollectionId,
            &csp::multiplayer::LightSpaceComponent::SetLightCookieAssetCollectionId)
        .property("lightCookieType", &csp::multiplayer::LightSpaceComponent::GetLightCookieType, &csp::multiplayer::LightSpaceComponent::SetLightCookieType)
        .property("isVisible", &csp::multiplayer::LightSpaceComponent::GetIsVisible, &csp::multiplayer::LightSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::LightSpaceComponent::GetIsARVisible, &csp::multiplayer::LightSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::LightSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::LightSpaceComponent::SetIsVirtualVisible)
        .property("thirdPartyComponentRef", &csp::multiplayer::LightSpaceComponent::GetThirdPartyComponentRef, &csp::multiplayer::LightSpaceComponent::SetThirdPartyComponentRef);
}
