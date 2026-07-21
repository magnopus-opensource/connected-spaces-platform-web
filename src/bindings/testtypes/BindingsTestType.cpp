/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * The EMSCRIPTEN_BINDINGS registration for BindingsTestType (and the container
 * register_type calls keyed on it) live here, in exactly one translation unit.
 * The class definition stays in the header so multiple TUs can use the type,
 * but the registration must compile once or embind throws "Cannot register
 * type 'BindingsTestType' twice" at module init.
 */

#include "BindingsTestType.h"
#include "../containers/Array.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/String.h"
#include "../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <iostream>

constexpr bool PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION = false;
inline int UniqueIDCounter = 0; //Mutable static counter just to give every type a unique ID.

BindingsTestType::BindingsTestType() : m_uniqueID(++UniqueIDCounter)
{
    ++AliveCount;
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "DEFAULT CONSTRUCTOR: " << m_uniqueID << ";" << std::endl;
    }
}
BindingsTestType::BindingsTestType(int value, std::string name) : m_value(value), m_name(std::move(name)), m_uniqueID(++UniqueIDCounter)
{
    ++AliveCount;
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "REGULAR CONSTRUCTOR: " << m_uniqueID << ";" << std::endl;
    }
}

BindingsTestType::BindingsTestType(const BindingsTestType& other) : m_value(other.m_value), m_name(other.m_name), m_uniqueID(++UniqueIDCounter)
{
    ++AliveCount;
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "COPY CONSTRUCTOR: " << m_uniqueID << ";" << std::endl;
    }
}
BindingsTestType::BindingsTestType(BindingsTestType&& other) noexcept : m_value(other.m_value), m_name(std::move(other.m_name)), m_uniqueID(++UniqueIDCounter)
{
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "MOVE CONSTRUCTOR: " << m_uniqueID << ";" << std::endl;
    }
    /* Move does not change alivecount, just transfering data */
    other.m_value = -1;
    other.m_name = "";
    other.m_movedFrom = true;
}

/* No Constructor/Destructor dance when doing copy/move assignment, so AliveCount remains stable*/
BindingsTestType& BindingsTestType::operator=(const BindingsTestType& other)
{
    if (this != &other) {
        m_uniqueID = ++UniqueIDCounter;
        m_value = other.m_value;
        m_name = other.m_name;
    }
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "COPY ASSIGN OPERATOR: " << m_uniqueID << ";" << std::endl;
    }
    return *this;
}
BindingsTestType& BindingsTestType::operator=(BindingsTestType&& other) noexcept
{
    if (this != &other) {
        m_uniqueID = ++UniqueIDCounter;
        m_value = other.m_value;
        m_name = std::move(other.m_name);

        other.m_value = -1;
        other.m_name = "";
        other.m_movedFrom = true;
    }
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "MOVE ASSIGN OPERATOR: " << m_uniqueID << ";" << std::endl;
    }
    return *this;
}

BindingsTestType::~BindingsTestType()
{
    if (!m_movedFrom) {
        --AliveCount;
    }
    if (PRINT_BINDINGSTESTTYPE_OPERATOR_INSTRUMENTATION) {
        std::cout << "DESTRUCTOR: " << m_uniqueID << ";" << std::endl;
    }
}

int BindingsTestType::GetValue() const { return m_value; }
void BindingsTestType::SetValue(int value) { m_value = value; }

const std::string& BindingsTestType::GetName() const { return m_name; }
void BindingsTestType::SetName(std::string name) { m_name = std::move(name); }

bool BindingsTestType::operator==(const BindingsTestType& other) const { return m_value == other.m_value && m_name == other.m_name; }

EMSCRIPTEN_BINDINGS(CSPTestTypeBindings)
{
    emscripten::class_<BindingsTestType>("BindingsTestType")
        .class_function(
            "create(value, name)", +[](int value, std::string name) { return BindingsTestType(value, std::move(name)); })
        .property("value", &BindingsTestType::GetValue, &BindingsTestType::SetValue)
        .property("name", &BindingsTestType::GetName, &BindingsTestType::SetName)
        .function("equals", &BindingsTestType::operator==)
        .class_property("aliveCount", &BindingsTestType::AliveCount);

    //Register the named version of the non-owning pointer type, as we use it as a raw `val` via NonOwningVal.
    emscripten::register_type<BindingsTestTypePointer>("BindingsTestType | null");

    // Array
    emscripten::register_type<csp::common::Array<int>>("number[]");
    emscripten::register_type<csp::common::Array<BindingsTestType>>("BindingsTestType[]");
    emscripten::register_type<csp::common::Array<BindingsTestType*>>("(BindingsTestType | null)[]");
    emscripten::register_type<csp::common::Array<csp::common::String>>("string[]");

    // List
    emscripten::register_type<csp::common::List<int>>("number[]");
    emscripten::register_type<csp::common::List<BindingsTestType>>("BindingsTestType[]");
    emscripten::register_type<csp::common::List<BindingsTestType*>>("(BindingsTestType | null)[]");

    // Map
    emscripten::register_type<csp::common::Map<int, int>>("Map<number, number>");
    emscripten::register_type<csp::common::Map<int, BindingsTestType>>("Map<number, BindingsTestType>");
    emscripten::register_type<csp::common::Map<int, BindingsTestType*>>("Map<number, (BindingsTestType | null)>");
    emscripten::register_type<csp::common::Map<int, csp::common::Array<BindingsTestType>>>("Map<number, BindingsTestType[]>");
    emscripten::register_type<csp::common::Map<int, csp::common::Array<BindingsTestType*>>>("Map<number, (BindingsTestType[] | null)>");
    emscripten::register_type<csp::common::Map<csp::common::String, int>>("Map<string, number>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");

    /* Return types, allows embinds machinery to emit a different typescript signature for container returns, meaning we can use `using` in a
     * type-checked manner. You need to remember to convert to these types in the returning methods, but you don't need to worry about it for
     * parameters.
     * As pointers across the API boundary are never ownership-transfer, you don't need to list pointer types here
     */

    // Array
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<int>>>("(number[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>>>("(BindingsTestType[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::String>>>("(string[] & Disposable)");

    // List
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<int>>>("(number[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<BindingsTestType>>>("(BindingsTestType[] & Disposable)");

    // Map
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<int, int>>>("(Map<number, number> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>>>("(Map<number, BindingsTestType> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>>>("(Map<string, number> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>>>("(Map<string, string> & Disposable)");

    // Optional
    emscripten::register_optional<int>();
    emscripten::register_optional<BindingsTestType>();
    emscripten::register_optional<BindingsTestType*>();
    emscripten::register_optional<csp::common::String>();
    // Optional of List<BindingsTestType> (note the register_type instead of register_optional with JSDisposable to ensure the container has Disposable)
    emscripten::register_optional<csp::common::List<BindingsTestType>>();
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<BindingsTestType>>>>("(BindingsTestType[] & Disposable) | undefined");
    // Optional of Array<BindingsTestType> (note the register_type instead of register_optional with JSDisposable to ensure the container has Disposable)
    emscripten::register_optional<csp::common::Array<BindingsTestType>>();
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Optional<csp::common::Array<BindingsTestType>>>>("(BindingsTestType[] & Disposable) | undefined");
    // List of Optional<BindingsTestType>
    emscripten::register_type<csp::common::List<csp::common::Optional<BindingsTestType>>>("(BindingsTestType | undefined)[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::Optional<BindingsTestType>>>>("((BindingsTestType | undefined)[] & Disposable)");
    // Array of Optional<BindingsTestType>
    emscripten::register_type<csp::common::Array<csp::common::Optional<BindingsTestType>>>("(BindingsTestType | undefined)[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::Optional<BindingsTestType>>>>("((BindingsTestType | undefined)[] & Disposable)");
}
