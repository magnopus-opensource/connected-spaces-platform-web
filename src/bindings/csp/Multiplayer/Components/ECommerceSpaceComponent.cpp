#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ECommerceSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPECommerceSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ECommercePropertyKeys>("ECommercePropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::ECommercePropertyKeys::Position)
        .value("ProductId", csp::multiplayer::ECommercePropertyKeys::ProductId)
        .value("Num", csp::multiplayer::ECommercePropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ECommerceSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ECommerceSpaceComponent")
        .property("position", &csp::multiplayer::ECommerceSpaceComponent::GetPosition, &csp::multiplayer::ECommerceSpaceComponent::SetPosition)
        .property("productId", &csp::multiplayer::ECommerceSpaceComponent::GetProductId, &csp::multiplayer::ECommerceSpaceComponent::SetProductId);
}
