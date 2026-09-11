#include "../../../containers/List.h"

#include "CSP/Common/List.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/SplineSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSplineSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::SplinePropertyKeys>("SplinePropertyKeys", emscripten::enum_value_type::number)
        .value("Waypoints", csp::multiplayer::SplinePropertyKeys::Waypoints)
        .value("Num", csp::multiplayer::SplinePropertyKeys::Num);

    emscripten::class_<csp::multiplayer::SplineSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("SplineSpaceComponent")
        .property("waypoints", &csp::multiplayer::SplineSpaceComponent::GetWaypoints, &csp::multiplayer::SplineSpaceComponent::SetWaypoints)
        .function("getLocationAlongSpline(normalisedDistance)", &csp::multiplayer::SplineSpaceComponent::GetLocationAlongSpline);
}
