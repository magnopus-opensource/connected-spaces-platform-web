#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Sequence/Sequence.h"
#include "CSP/Systems/Sequence/SequenceSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::SequenceSystem>(csp::systems::SequenceSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPSequenceSystem)
{
    emscripten::class_<csp::systems::SequenceSystem, emscripten::base<csp::systems::SystemBase>>("SequenceSystem")
        .function(
            "createSequence(sequenceKey, referenceType, referenceId, items, metaData)",
            +[](csp::systems::SequenceSystem& self, const csp::common::String& sequenceKey, const csp::common::String& referenceType, const csp::common::String& referenceId,
                 const csp::common::Array<csp::common::String>& items, const csp::common::Map<csp::common::String, csp::common::String>& metaData) {
                return Promisify<PromiseOfSequenceResult>(
                    [&](emscripten::val cb) { self.CreateSequence(sequenceKey, referenceType, referenceId, items, metaData, ToNativeCallback(cb.as<SequenceResultCallback>())); });
            })
        .function(
            "updateSequence(sequenceKey, referenceType, referenceId, items, metaData)",
            +[](csp::systems::SequenceSystem& self, const csp::common::String& sequenceKey, const csp::common::String& referenceType, const csp::common::String& referenceId,
                 const csp::common::Array<csp::common::String>& items, const csp::common::Map<csp::common::String, csp::common::String>& metaData) {
                return Promisify<PromiseOfSequenceResult>(
                    [&](emscripten::val cb) { self.UpdateSequence(sequenceKey, referenceType, referenceId, items, metaData, ToNativeCallback(cb.as<SequenceResultCallback>())); });
            })
        .function(
            "renameSequence(oldSequenceKey, newSequenceKey)",
            +[](csp::systems::SequenceSystem& self, const csp::common::String& oldSequenceKey, const csp::common::String& newSequenceKey) {
                return Promisify<PromiseOfSequenceResult>(
                    [&](emscripten::val cb) { self.RenameSequence(oldSequenceKey, newSequenceKey, ToNativeCallback(cb.as<SequenceResultCallback>())); });
            })
        .function(
            "getSequencesByCriteria(sequenceKeys, keyRegex, referenceType, referenceIds, metaData)",
            +[](csp::systems::SequenceSystem& self, const csp::common::Array<csp::common::String>& sequenceKeys, const csp::common::Optional<csp::common::String>& keyRegex,
                 const csp::common::Optional<csp::common::String>& referenceType, const csp::common::Array<csp::common::String>& referenceIds,
                 const csp::common::Map<csp::common::String, csp::common::String>& metaData) {
                return Promisify<PromiseOfSequencesResult>([&](emscripten::val cb) {
                    self.GetSequencesByCriteria(sequenceKeys, keyRegex, referenceType, referenceIds, metaData, ToNativeCallback(cb.as<SequencesResultCallback>()));
                });
            })
        .function(
            "getAllSequencesContainingItems(items, referenceType, referenceIds)",
            +[](csp::systems::SequenceSystem& self, const csp::common::Array<csp::common::String>& items, const csp::common::Optional<csp::common::String>& referenceType,
                 const csp::common::Array<csp::common::String>& referenceIds) {
                return Promisify<PromiseOfSequencesResult>(
                    [&](emscripten::val cb) { self.GetAllSequencesContainingItems(items, referenceType, referenceIds, ToNativeCallback(cb.as<SequencesResultCallback>())); });
            })
        .function(
            "getSequence(sequenceKey)",
            +[](csp::systems::SequenceSystem& self, const csp::common::String& sequenceKey) {
                return Promisify<PromiseOfSequenceResult>([&](emscripten::val cb) { self.GetSequence(sequenceKey, ToNativeCallback(cb.as<SequenceResultCallback>())); });
            })
        .function(
            "deleteSequences(sequenceKeys)",
            +[](csp::systems::SequenceSystem& self, const csp::common::Array<csp::common::String>& sequenceKeys) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteSequences(sequenceKeys, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "setSequenceChangedCallback(callback)",
            +[](csp::systems::SequenceSystem& self, SequenceChangedEventCallback callback) { self.SetSequenceChangedCallback(ToNativeCallback(callback)); });
}
