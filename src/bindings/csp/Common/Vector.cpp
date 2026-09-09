#include "CSP/Common/Vector.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPVector)
{
    emscripten::value_object<csp::common::Vector2>("Vector2").field("x", &csp::common::Vector2::X).field("y", &csp::common::Vector2::Y);

    emscripten::value_object<csp::common::Vector3>("Vector3").field("x", &csp::common::Vector3::X).field("y", &csp::common::Vector3::Y).field("z", &csp::common::Vector3::Z);

    emscripten::value_object<csp::common::Vector4>("Vector4")
        .field("x", &csp::common::Vector4::X)
        .field("y", &csp::common::Vector4::Y)
        .field("z", &csp::common::Vector4::Z)
        .field("w", &csp::common::Vector4::W);
}
