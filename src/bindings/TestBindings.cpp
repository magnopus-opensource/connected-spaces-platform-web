/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */

#include "containers/Array.h"
#include "containers/List.h"
#include "containers/Map.h"
#include "string/String.h"
#include "utils/JSDisposable.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <string>
#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"

/*
 * A nonsense, instrumentable C++ object that we can bind and test mechanisms with.
 */
class BindingsTestType
{
public:
    BindingsTestType() {
        ++AliveCount;
    }
    BindingsTestType(int value, std::string name) : m_value(value), m_name(std::move(name)) {++AliveCount;}

    BindingsTestType(const BindingsTestType& other) : m_value(other.m_value), m_name(other.m_name) { ++AliveCount; }
    BindingsTestType(BindingsTestType&& other) noexcept : m_value(other.m_value), m_name(std::move(other.m_name)) { ++AliveCount; }

    /* No Constructor/Destructor dance when doing copy/move assignment, so AliveCount remains stable*/
    BindingsTestType& operator=(const BindingsTestType&) = default;
    BindingsTestType& operator=(BindingsTestType&&) noexcept = default;

    ~BindingsTestType(){--AliveCount;}

    int GetValue() const { return m_value; }
    void SetValue(int value) { m_value = value; }

    const std::string& GetName() const { return m_name; }
    void SetName(std::string name) { m_name = std::move(name); }

    bool operator==(const BindingsTestType& other) const { return m_value == other.m_value && m_name == other.m_name; }

    // Test instrumentation, keep track of how many instances of this object exist
    static inline int AliveCount = 0;

private:
    int m_value = 0;
    std::string m_name;
};

/*
 * A class to provide some fundamental patterns as interfaces to test binding mechanisms.
 * Testing real interfaces may be preferable, but we need to bootstrap.
 * This does not indicate any sort of support set, it's more just a utility type such that
 * we can test binding expressions freely.
 *
 * Some of the interfaces here might not make sense in an isolated context. Returning
 * a member variable by value, for example, is almost always wrong for non-basic types
 * (even with move semantics, it's strictly worse than const ref in my opinion).
 * However, we're just trying to prove interface expressions.
 */
class BindingMechanismsTestType
{
public:

    // Array<int>
    csp::common::Array<int> GetArrayBasicTypeByValue() const { return m_arrayBasicType; }
    const csp::common::Array<int>& GetArrayBasicTypeByConstRef() const { return m_arrayBasicType; }
    void SetArrayBasicTypeByValue(csp::common::Array<int> value) { m_arrayBasicType = std::move(value); }
    void SetArrayBasicTypeByConstRef(const csp::common::Array<int>& value) { m_arrayBasicType = value; }

    // Array<BindingsTestType>
    csp::common::Array<BindingsTestType> GetArrayFullTypeByValue() const { return m_arrayFullType; }
    const csp::common::Array<BindingsTestType>& GetArrayFullTypeByConstRef() const { return m_arrayFullType; }
    void SetArrayFullTypeByValue(csp::common::Array<BindingsTestType> value) { m_arrayFullType = std::move(value); }
    void SetArrayFullTypeByConstRef(const csp::common::Array<BindingsTestType>& value) { m_arrayFullType = value; }

    // Array<String>
    csp::common::Array<csp::common::String> GetArrayStringByValue() const { return m_arrayString; }
    const csp::common::Array<csp::common::String>& GetArrayStringByConstRef() const { return m_arrayString; }
    void SetArrayStringByValue(csp::common::Array<csp::common::String> value) { m_arrayString = std::move(value); }
    void SetArrayStringByConstRef(const csp::common::Array<csp::common::String>& value) { m_arrayString = value; }

    // List<int>
    csp::common::List<int> GetListBasicTypeByValue() const { return m_listBasicType; }
    const csp::common::List<int>& GetListBasicTypeByConstRef() const { return m_listBasicType; }
    void SetListBasicTypeByValue(csp::common::List<int> value) { m_listBasicType = std::move(value); }
    void SetListBasicTypeByConstRef(const csp::common::List<int>& value) { m_listBasicType = value; }

    // List<BindingsTestType>
    csp::common::List<BindingsTestType> GetListFullTypeByValue() const { return m_listFullType; }
    const csp::common::List<BindingsTestType>& GetListFullTypeByConstRef() const { return m_listFullType; }
    void SetListFullTypeByValue(csp::common::List<BindingsTestType> value) { m_listFullType = std::move(value); }
    void SetListFullTypeByConstRef(const csp::common::List<BindingsTestType>& value) { m_listFullType = value; }

    // Map<int, int>
    csp::common::Map<int, int> GetMapBasicTypeByValue() const { return m_mapBasicType; }
    const csp::common::Map<int, int>& GetMapBasicTypeByConstRef() const { return m_mapBasicType; }
    void SetMapBasicTypeByValue(csp::common::Map<int, int> value) { m_mapBasicType = std::move(value); }
    void SetMapBasicTypeByConstRef(const csp::common::Map<int, int>& value) { m_mapBasicType = value; }

    // Map<int, BindingsTestType>
    csp::common::Map<int, BindingsTestType> GetMapFullTypeByValue() const { return m_mapFullType; }
    const csp::common::Map<int, BindingsTestType>& GetMapFullTypeByConstRef() const { return m_mapFullType; }
    void SetMapFullTypeByValue(csp::common::Map<int, BindingsTestType> value) { m_mapFullType = std::move(value); }
    void SetMapFullTypeByConstRef(const csp::common::Map<int, BindingsTestType>& value) { m_mapFullType = value; }

    // Map<string, int>
    csp::common::Map<csp::common::String, int> GetMapStringIntByValue() const { return m_mapStringInt; }
    const csp::common::Map<csp::common::String, int>& GetMapStringIntByConstRef() const { return m_mapStringInt; }
    void SetMapStringIntByValue(csp::common::Map<csp::common::String, int> value) { m_mapStringInt = std::move(value); }
    void SetMapStringIntByConstRef(const csp::common::Map<csp::common::String, int>& value) { m_mapStringInt = value; }

    // Map<string, string>
    csp::common::Map<csp::common::String, csp::common::String> GetMapStringStringByValue() const { return m_mapStringString; }
    const csp::common::Map<csp::common::String, csp::common::String>& GetMapStringStringByConstRef() const { return m_mapStringString; }
    void SetMapStringStringByValue(csp::common::Map<csp::common::String, csp::common::String> value) { m_mapStringString = std::move(value); }
    void SetMapStringStringByConstRef(const csp::common::Map<csp::common::String, csp::common::String>& value) { m_mapStringString = value; }

    // Optional<int>
    csp::common::Optional<int> GetOptionalBasicTypeByValue() const { return m_optionalBasicType; }
    const csp::common::Optional<int>& GetOptionalBasicTypeByConstRef() const { return m_optionalBasicType; }
    void SetOptionalBasicTypeByValue(csp::common::Optional<int> value) { m_optionalBasicType = std::move(value); }
    void SetOptionalBasicTypeByConstRef(const csp::common::Optional<int>& value) { m_optionalBasicType = value; }

    // Optional<BindingsTestType>
    csp::common::Optional<BindingsTestType> GetOptionalFullTypeByValue() const { return m_optionalFullType; }
    const csp::common::Optional<BindingsTestType>& GetOptionalFullTypeByConstRef() const { return m_optionalFullType; }
    void SetOptionalFullTypeByValue(csp::common::Optional<BindingsTestType> value) { m_optionalFullType = std::move(value); }
    void SetOptionalFullTypeByConstRef(const csp::common::Optional<BindingsTestType>& value) { m_optionalFullType = value; }

    // String
    csp::common::String GetCspStringByValue() const { return m_cspString; }
    const csp::common::String& GetCspStringByConstRef() const { return m_cspString; }
    void SetCspStringByValue(csp::common::String value) { m_cspString = std::move(value); }
    void SetCspStringByConstRef(const csp::common::String& value) { m_cspString = value; }

    /*
     * The pointer setters here would be a bit of a footgun in a real API expression and we should try to avoid it.
     * It's perfectly valid, it means that C++ holds reference to memory owned by JS, which we do want in some cases,
     * although given that JS is garbage collected, is a recipe for dangling references unless the JS implementor is careful.
     * The normal expression of pointer lists is one-directional, and should give JS access to objects owned in C++ by reference.
     * Hence, the alternate getter. (No real need to test const-ref of pointers, the mechanisms are unrelated)
     */

    // Array of pointers
    csp::common::Array<BindingsTestType*> GetArrayOfPointersByValue() const { return m_arrayOfPointers; }
    const csp::common::Array<BindingsTestType*>& GetArrayOfPointersByConstRef() const { return m_arrayOfPointers; }
    csp::common::Array<BindingsTestType*> GetArrayOfCppOwnedPointers() const { return m_arrayOfCppOwnedPointers; }
    void SetArrayOfPointersByValue(csp::common::Array<BindingsTestType*> value) { m_arrayOfPointers = std::move(value); }
    void SetArrayOfPointersByConstRef(const csp::common::Array<BindingsTestType*>& value) { m_arrayOfPointers = value; }

    // List of pointers
    csp::common::List<BindingsTestType*> GetListOfPointersByValue() const { return m_listOfPointers; }
    const csp::common::List<BindingsTestType*>& GetListOfPointersByConstRef() const { return m_listOfPointers; }
    csp::common::List<BindingsTestType*> GetListOfCppOwnedPointers() const { return m_listOfCppOwnedPointers; }
    void SetListOfPointersByValue(csp::common::List<BindingsTestType*> value) { m_listOfPointers = std::move(value); }
    void SetListOfPointersByConstRef(const csp::common::List<BindingsTestType*>& value) { m_listOfPointers = value; }

    // Map of pointers
    csp::common::Map<int, BindingsTestType*> GetMapOfPointersByValue() const { return m_mapOfPointers; }
    const csp::common::Map<int, BindingsTestType*>& GetMapOfPointersByConstRef() const { return m_mapOfPointers; }
    csp::common::Map<int, BindingsTestType*> GetMapOfCppOwnedPointers() const { return m_mapOfCppOwnedPointers; }
    void SetMapOfPointersByValue(csp::common::Map<int, BindingsTestType*> value) { m_mapOfPointers = std::move(value); }
    void SetMapOfPointersByConstRef(const csp::common::Map<int, BindingsTestType*>& value) { m_mapOfPointers = value; }

private:
    csp::common::Array<int> m_arrayBasicType;
    csp::common::Array<BindingsTestType> m_arrayFullType;
    csp::common::Array<csp::common::String> m_arrayString;
    csp::common::List<int> m_listBasicType;
    csp::common::List<BindingsTestType> m_listFullType;
    csp::common::Map<int, int> m_mapBasicType;
    csp::common::Map<int, BindingsTestType> m_mapFullType;
    csp::common::Map<csp::common::String, int> m_mapStringInt;
    csp::common::Map<csp::common::String, csp::common::String> m_mapStringString;
    csp::common::Optional<int> m_optionalBasicType;
    csp::common::Optional<BindingsTestType> m_optionalFullType;
    csp::common::String m_cspString;

    csp::common::Array<BindingsTestType*> m_arrayOfPointers;
    csp::common::Array<BindingsTestType*> m_arrayOfCppOwnedPointers; //Prefilled in constructor
    csp::common::List<BindingsTestType*> m_listOfPointers;
    csp::common::List<BindingsTestType*> m_listOfCppOwnedPointers; //Prefilled in constructor
    csp::common::Map<int, BindingsTestType*> m_mapOfPointers;
    csp::common::Map<int, BindingsTestType*> m_mapOfCppOwnedPointers; //Prefilled in constructor
    // Optional to pointer isn't a pattern we express (i think).

    // We may need to add List<T*>* for annoying reasons, I believe this is a pattern
    // expressed due to the legacy wrapper generator, which we may need to support
    // in a transitionary capacity.
public:
    BindingMechanismsTestType() : m_arrayOfCppOwnedPointers(2) {
        // Yes, these will technically leak, but who cares in test code.
        m_arrayOfCppOwnedPointers[0] = new BindingsTestType(1, "One");
        m_arrayOfCppOwnedPointers[1] = new BindingsTestType(2, "Two");

        m_listOfCppOwnedPointers.Append(new BindingsTestType(1, "One"));
        m_listOfCppOwnedPointers.Append(new BindingsTestType(2, "Two"));

        m_mapOfCppOwnedPointers[1] = new BindingsTestType(1, "One");
        m_mapOfCppOwnedPointers[2] = new BindingsTestType(2, "Two");
    }
};

EMSCRIPTEN_BINDINGS(CSPTestBindings)
{

    emscripten::class_<BindingsTestType>("BindingsTestType")
        .class_function("create(value, name)", +[](int value, std::string name){ return BindingsTestType(value, std::move(name)); })
        .property("value", &BindingsTestType::GetValue, &BindingsTestType::SetValue)
        .property("name", &BindingsTestType::GetName, &BindingsTestType::SetName)
        .function("equals", &BindingsTestType::operator==)
        .class_property("aliveCount", &BindingsTestType::AliveCount);

    /*
     * We'll bind all these as functions because we're looking to test the raw mechanisms as they would apply to
     * any old method in the API. However, in a real case you'd probably want to bind getter/setter pairs like
     * this as properties
     *
     * Sidenote: pretty sure the value based returns here are going to involve 2 copies. One into the return
     * value of the method itself, and then once again as the data is copied across the JS/C++ runtime boundaries.
     * We have options here:
     *   - Just don't do value returns to owned data like this out of CSP. Tbh, the value return here is contrived anyhow, the const ref one makes more sense.
     *   - Have move operators on our container types, then the returns are RVO'd and it won't matter anyway.
     *   - Do different container element ownership across the interop boundary. Will look into this, although it seems counter to the theory of how embind wants you to do it.
     */

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
    emscripten::register_type<csp::common::Map<csp::common::String, int>>("Map<string, number>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");

    // Return types, allows embinds machinery to emit a different typescript signature for container returns, meaning we can use `using` in a type-checked manner.
    // You need to remember to convert to these types in the returning methods, but you don't need to worry about it for parameters.

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

    emscripten::class_<BindingMechanismsTestType>("BindingsMechanismsTestType")
        .class_function("create", +[](){ return BindingMechanismsTestType(); })
        .function("getArrayBasicTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<int>>{self.GetArrayBasicTypeByValue()};
        })
        .function("getArrayBasicTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<int>>{self.GetArrayBasicTypeByConstRef()};
        })
        .function("setArrayBasicTypeByValue(value)", &BindingMechanismsTestType::SetArrayBasicTypeByValue)
        .function("setArrayBasicTypeByConstRef(value)", &BindingMechanismsTestType::SetArrayBasicTypeByConstRef)
        .function("getArrayFullTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>>{self.GetArrayFullTypeByValue()};
        })
        .function("getArrayFullTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>>{self.GetArrayFullTypeByConstRef()};
        })
        .function("setArrayFullTypeByValue(value)", &BindingMechanismsTestType::SetArrayFullTypeByValue)
        .function("setArrayFullTypeByConstRef(value)", &BindingMechanismsTestType::SetArrayFullTypeByConstRef)
        .function("getArrayOfPointersByValue", &BindingMechanismsTestType::GetArrayOfPointersByValue)
        .function("getArrayOfPointersByConstRef", &BindingMechanismsTestType::GetArrayOfPointersByConstRef)
        .function("getArrayOfCppOwnedPointers", &BindingMechanismsTestType::GetArrayOfCppOwnedPointers)
        .function("setArrayOfPointersByValue(value)", &BindingMechanismsTestType::SetArrayOfPointersByValue)
        .function("setArrayOfPointersByConstRef(value)", &BindingMechanismsTestType::SetArrayOfPointersByConstRef)
        .function("getArrayStringByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<csp::common::String>>{self.GetArrayStringByValue()};
        })
        .function("getArrayStringByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Array<csp::common::String>>{self.GetArrayStringByConstRef()};
        })
        .function("setArrayStringByValue(value)", &BindingMechanismsTestType::SetArrayStringByValue)
        .function("setArrayStringByConstRef(value)", &BindingMechanismsTestType::SetArrayStringByConstRef)
        .function("getListBasicTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::List<int>>{self.GetListBasicTypeByValue()};
        })
        .function("getListBasicTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::List<int>>{self.GetListBasicTypeByConstRef()};
        })
        .function("setListBasicTypeByValue(value)", &BindingMechanismsTestType::SetListBasicTypeByValue)
        .function("setListBasicTypeByConstRef(value)", &BindingMechanismsTestType::SetListBasicTypeByConstRef)
        .function("getListFullTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::List<BindingsTestType>>{self.GetListFullTypeByValue()};
        })
        .function("getListFullTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::List<BindingsTestType>>{self.GetListFullTypeByConstRef()};
        })
        .function("setListFullTypeByValue(value)", &BindingMechanismsTestType::SetListFullTypeByValue)
        .function("setListFullTypeByConstRef(value)", &BindingMechanismsTestType::SetListFullTypeByConstRef)
        .function("getListOfPointersByValue", &BindingMechanismsTestType::GetListOfPointersByValue)
        .function("getListOfPointersByConstRef", &BindingMechanismsTestType::GetListOfPointersByConstRef)
        .function("getListOfCppOwnedPointers", &BindingMechanismsTestType::GetListOfCppOwnedPointers)
        .function("setListOfPointersByValue(value)", &BindingMechanismsTestType::SetListOfPointersByValue)
        .function("setListOfPointersByConstRef(value)", &BindingMechanismsTestType::SetListOfPointersByConstRef)
        .function("getMapBasicTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<int, int>>{self.GetMapBasicTypeByValue()};
        })
        .function("getMapBasicTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<int, int>>{self.GetMapBasicTypeByConstRef()};
        })
        .function("setMapBasicTypeByValue(value)", &BindingMechanismsTestType::SetMapBasicTypeByValue)
        .function("setMapBasicTypeByConstRef(value)", &BindingMechanismsTestType::SetMapBasicTypeByConstRef)
        .function("getMapFullTypeByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>>{self.GetMapFullTypeByValue()};
        })
        .function("getMapFullTypeByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>>{self.GetMapFullTypeByConstRef()};
        })
        .function("setMapFullTypeByValue(value)", &BindingMechanismsTestType::SetMapFullTypeByValue)
        .function("setMapFullTypeByConstRef(value)", &BindingMechanismsTestType::SetMapFullTypeByConstRef)
        .function("getMapOfPointersByValue", &BindingMechanismsTestType::GetMapOfPointersByValue)
        .function("getMapOfPointersByConstRef", &BindingMechanismsTestType::GetMapOfPointersByConstRef)
        .function("getMapOfCppOwnedPointers", &BindingMechanismsTestType::GetMapOfCppOwnedPointers)
        .function("setMapOfPointersByValue(value)", &BindingMechanismsTestType::SetMapOfPointersByValue)
        .function("setMapOfPointersByConstRef(value)", &BindingMechanismsTestType::SetMapOfPointersByConstRef)
        .function("getMapStringIntByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>>{self.GetMapStringIntByValue()};
        })
        .function("getMapStringIntByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>>{self.GetMapStringIntByConstRef()};
        })
        .function("setMapStringIntByValue(value)", &BindingMechanismsTestType::SetMapStringIntByValue)
        .function("setMapStringIntByConstRef(value)", &BindingMechanismsTestType::SetMapStringIntByConstRef)
        .function("getMapStringStringByValue", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>>{self.GetMapStringStringByValue()};
        })
        .function("getMapStringStringByConstRef", +[](const BindingMechanismsTestType& self) {
            return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>>{self.GetMapStringStringByConstRef()};
        })
        .function("setMapStringStringByValue(value)", &BindingMechanismsTestType::SetMapStringStringByValue)
        .function("setMapStringStringByConstRef(value)", &BindingMechanismsTestType::SetMapStringStringByConstRef)
        .function("getCspStringByValue", &BindingMechanismsTestType::GetCspStringByValue)
        .function("getCspStringByConstRef", &BindingMechanismsTestType::GetCspStringByConstRef)
        .function("setCspStringByValue", &BindingMechanismsTestType::SetCspStringByValue)
        .function("setCspStringByConstRef", &BindingMechanismsTestType::SetCspStringByConstRef);
        /*
        .function("getOptionalBasicTypeByValue", &BindingMechanismsTestType::GetOptionalBasicTypeByValue)
        .function("getOptionalBasicTypeByConstRef", &BindingMechanismsTestType::GetOptionalBasicTypeByConstRef)
        .function("setOptionalBasicTypeByValue", &BindingMechanismsTestType::SetOptionalBasicTypeByValue)
        .function("setOptionalBasicTypeByConstRef", &BindingMechanismsTestType::SetOptionalBasicTypeByConstRef)
        .function("getOptionalFullTypeByValue", &BindingMechanismsTestType::GetOptionalFullTypeByValue)
        .function("getOptionalFullTypeByConstRef", &BindingMechanismsTestType::GetOptionalFullTypeByConstRef)
        .function("setOptionalFullTypeByValue", &BindingMechanismsTestType::SetOptionalFullTypeByValue)
        .function("setOptionalFullTypeByConstRef", &BindingMechanismsTestType::SetOptionalFullTypeByConstRef)
        */
}
