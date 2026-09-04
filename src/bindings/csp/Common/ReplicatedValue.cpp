#include "../../containers/Map.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPReplicatedValue)
{
    emscripten::enum_<csp::common::ReplicatedValueType>("ReplicatedValueType", emscripten::enum_value_type::number)
        .value("InvalidType", csp::common::ReplicatedValueType::InvalidType)
        .value("Boolean", csp::common::ReplicatedValueType::Boolean)
        .value("Integer", csp::common::ReplicatedValueType::Integer)
        .value("Float", csp::common::ReplicatedValueType::Float)
        .value("String", csp::common::ReplicatedValueType::String)
        .value("Vector3", csp::common::ReplicatedValueType::Vector3)
        .value("Vector4", csp::common::ReplicatedValueType::Vector4)
        .value("Vector2", csp::common::ReplicatedValueType::Vector2)
        .value("StringMap", csp::common::ReplicatedValueType::StringMap);

    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>("Map<string, ReplicatedValue>");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>>("(Map<string, ReplicatedValue> & Disposable)");

    emscripten::class_<csp::common::ReplicatedValue>("ReplicatedValue")
        .class_function(
            "create", +[]() { return csp::common::ReplicatedValue(); })
        .class_function(
            "createBool(value)", +[](bool value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createFloat(value)", +[](float value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createInt(value)", +[](int64_t value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createString(value)", +[](const csp::common::String& value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createVector2(value)", +[](const csp::common::Vector2& value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createVector3(value)", +[](const csp::common::Vector3& value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createVector4(value)", +[](const csp::common::Vector4& value) { return csp::common::ReplicatedValue(value); })
        .class_function(
            "createStringMap(value)", +[](const csp::common::Map<csp::common::String, csp::common::ReplicatedValue>& value) { return csp::common::ReplicatedValue(value); })
        .property("replicatedValueType", &csp::common::ReplicatedValue::GetReplicatedValueType)
        .function("getBool", &csp::common::ReplicatedValue::GetBool)
        .function("setBool(value)", &csp::common::ReplicatedValue::SetBool)
        .function("getFloat", &csp::common::ReplicatedValue::GetFloat)
        .function("setFloat(value)", &csp::common::ReplicatedValue::SetFloat)
        .function("getInt", &csp::common::ReplicatedValue::GetInt)
        .function("setInt(value)", &csp::common::ReplicatedValue::SetInt)
        .function("getString", &csp::common::ReplicatedValue::GetString)
        .function("setString(value)", static_cast<void (csp::common::ReplicatedValue::*)(const csp::common::String&)>(&csp::common::ReplicatedValue::SetString))
        .function("getVector2", &csp::common::ReplicatedValue::GetVector2)
        .function("setVector2(value)", &csp::common::ReplicatedValue::SetVector2)
        .function("getVector3", &csp::common::ReplicatedValue::GetVector3)
        .function("setVector3(value)", &csp::common::ReplicatedValue::SetVector3)
        .function("getVector4", &csp::common::ReplicatedValue::GetVector4)
        .function("setVector4(value)", &csp::common::ReplicatedValue::SetVector4)
        .function(
            "getStringMap",
            +[](const csp::common::ReplicatedValue& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>> { self.GetStringMap() };
            })
        .function("setStringMap(value)", &csp::common::ReplicatedValue::SetStringMap)
        .function("equals(other)", &csp::common::ReplicatedValue::operator==);
}
