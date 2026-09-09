#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ReflectionSpaceComponent.h"

#include "emscripten/bind.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

EMSCRIPTEN_BINDINGS(CSPReflectionSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ReflectionPropertyKeys>("ReflectionPropertyKeys", emscripten::enum_value_type::number)
        .value("Name_DEPRECATED", csp::multiplayer::ReflectionPropertyKeys::Name_DEPRECATED)
        .value("ReflectionAssetId", csp::multiplayer::ReflectionPropertyKeys::ReflectionAssetId)
        .value("AssetCollectionId", csp::multiplayer::ReflectionPropertyKeys::AssetCollectionId)
        .value("Position", csp::multiplayer::ReflectionPropertyKeys::Position)
        .value("Rotation_NOT_USED", csp::multiplayer::ReflectionPropertyKeys::Rotation_NOT_USED)
        .value("Scale", csp::multiplayer::ReflectionPropertyKeys::Scale)
        .value("ReflectionShape", csp::multiplayer::ReflectionPropertyKeys::ReflectionShape)
        .value("ThirdPartyComponentRef", csp::multiplayer::ReflectionPropertyKeys::ThirdPartyComponentRef)
        .value("Num", csp::multiplayer::ReflectionPropertyKeys::Num);

    emscripten::enum_<csp::multiplayer::ReflectionShape>("ReflectionShape", emscripten::enum_value_type::number)
        .value("UnitSphere", csp::multiplayer::ReflectionShape::UnitSphere)
        .value("UnitBox", csp::multiplayer::ReflectionShape::UnitBox);

    emscripten::class_<csp::multiplayer::ReflectionSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ReflectionSpaceComponent")
        .property("name", &csp::multiplayer::ReflectionSpaceComponent::GetName, &csp::multiplayer::ReflectionSpaceComponent::SetName)
        .property("reflectionAssetId", &csp::multiplayer::ReflectionSpaceComponent::GetReflectionAssetId, &csp::multiplayer::ReflectionSpaceComponent::SetReflectionAssetId)
        .property("assetCollectionId", &csp::multiplayer::ReflectionSpaceComponent::GetAssetCollectionId, &csp::multiplayer::ReflectionSpaceComponent::SetAssetCollectionId)
        .property("position", &csp::multiplayer::ReflectionSpaceComponent::GetPosition, &csp::multiplayer::ReflectionSpaceComponent::SetPosition)
        .property("scale", &csp::multiplayer::ReflectionSpaceComponent::GetScale, &csp::multiplayer::ReflectionSpaceComponent::SetScale)
        .property("reflectionShape", &csp::multiplayer::ReflectionSpaceComponent::GetReflectionShape, &csp::multiplayer::ReflectionSpaceComponent::SetReflectionShape)
        .property("thirdPartyComponentRef", &csp::multiplayer::ReflectionSpaceComponent::GetThirdPartyComponentRef,
            &csp::multiplayer::ReflectionSpaceComponent::SetThirdPartyComponentRef);
}

#pragma clang diagnostic pop
