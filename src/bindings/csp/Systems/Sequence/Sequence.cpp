#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Sequence/Sequence.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSequence)
{
    emscripten::class_<csp::systems::Sequence>("Sequence")
        .class_function(
            "create", +[]() { return csp::systems::Sequence(); })
        .property("key", &csp::systems::Sequence::Key)
        .property("referenceType", &csp::systems::Sequence::ReferenceType)
        .property("referenceId", &csp::systems::Sequence::ReferenceId)
        .property("items", &csp::systems::Sequence::Items)
        .property("metaData", &csp::systems::Sequence::MetaData)
        .function("equals(other)", &csp::systems::Sequence::operator==);

    emscripten::class_<csp::systems::SequenceResult, emscripten::base<csp::systems::ResultBase>>("SequenceResult")
        .function(
            "getSequence", +[](const csp::systems::SequenceResult& self) { return self.GetSequence(); });

    emscripten::class_<csp::systems::SequencesResult, emscripten::base<csp::systems::ResultBase>>("SequencesResult")
        .function(
            "getSequences",
            +[](const csp::systems::SequencesResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Sequence>> { self.GetSequences() }; });
}
