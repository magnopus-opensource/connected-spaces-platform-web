/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * This one is for testing interop/container types
 */

#include "../containers/Array.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/Optional.h"
#include "../containers/String.h"
#include "../utils/Handles.h"
#include "../utils/JSDisposable.h"
#include "BindingsTestType.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <string>

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
class ContainerBindingMechanismsTestType {
public:
    // Individual Elements
    BindingsTestType GetSingleFullTypeByValue() const { return m_singleValueType; }
    const BindingsTestType& GetSingleFullTypeAsConstRef() const { return m_singleValueType; }
    BindingsTestType* GetSingleFullTypeAsPointer() const { return m_singlePointerType; }

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

    // Map<String, int>
    csp::common::Map<csp::common::String, int> GetMapStringIntByValue() const { return m_mapStringInt; }
    const csp::common::Map<csp::common::String, int>& GetMapStringIntByConstRef() const { return m_mapStringInt; }
    void SetMapStringIntByValue(csp::common::Map<csp::common::String, int> value) { m_mapStringInt = std::move(value); }
    void SetMapStringIntByConstRef(const csp::common::Map<csp::common::String, int>& value) { m_mapStringInt = value; }

    // Map<String, String>
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

    // Optional<String>
    csp::common::Optional<csp::common::String> GetOptionalStringByValue() const { return m_optionalString; }
    const csp::common::Optional<csp::common::String>& GetOptionalStringByConstRef() const { return m_optionalString; }
    void SetOptionalStringByValue(csp::common::Optional<csp::common::String> value) { m_optionalString = std::move(value); }
    void SetOptionalStringByConstRef(const csp::common::Optional<csp::common::String>& value) { m_optionalString = value; }

    // Optional<List<BindingsTestType>>
    csp::common::Optional<csp::common::List<BindingsTestType>> GetOptionalListFullTypeByValue() const { return m_optionalListFullType; }
    const csp::common::Optional<csp::common::List<BindingsTestType>>& GetOptionalListFullTypeByConstRef() const { return m_optionalListFullType; }
    void SetOptionalListFullTypeByValue(csp::common::Optional<csp::common::List<BindingsTestType>> value) { m_optionalListFullType = std::move(value); }
    void SetOptionalListFullTypeByConstRef(const csp::common::Optional<csp::common::List<BindingsTestType>>& value) { m_optionalListFullType = value; }

    // List<Optional<BindingsTestType>>
    csp::common::List<csp::common::Optional<BindingsTestType>> GetListOfOptionalFullTypeByValue() const { return m_listOfOptionalFullType; }
    const csp::common::List<csp::common::Optional<BindingsTestType>>& GetListOfOptionalFullTypeByConstRef() const { return m_listOfOptionalFullType; }
    void SetListOfOptionalFullTypeByValue(csp::common::List<csp::common::Optional<BindingsTestType>> value) { m_listOfOptionalFullType = std::move(value); }
    void SetListOfOptionalFullTypeByConstRef(const csp::common::List<csp::common::Optional<BindingsTestType>>& value) { m_listOfOptionalFullType = value; }

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
    BindingsTestType m_singleValueType = BindingsTestType(1, "One");
    BindingsTestType* m_singlePointerType;

    csp::common::Array<int> m_arrayBasicType;
    csp::common::Array<BindingsTestType> m_arrayFullType;
    csp::common::Array<csp::common::String> m_arrayString;
    csp::common::List<int> m_listBasicType;
    csp::common::List<BindingsTestType> m_listFullType;
    csp::common::List<csp::common::Optional<BindingsTestType>> m_listOfOptionalFullType;
    csp::common::Map<int, int> m_mapBasicType;
    csp::common::Map<int, BindingsTestType> m_mapFullType;
    csp::common::Map<csp::common::String, int> m_mapStringInt;
    csp::common::Map<csp::common::String, csp::common::String> m_mapStringString;
    csp::common::Optional<int> m_optionalBasicType;
    csp::common::Optional<BindingsTestType> m_optionalFullType;
    csp::common::Optional<csp::common::String> m_optionalString;
    csp::common::Optional<csp::common::List<BindingsTestType>> m_optionalListFullType;
    csp::common::String m_cspString;

    csp::common::Array<BindingsTestType*> m_arrayOfPointers;
    csp::common::Array<BindingsTestType*> m_arrayOfCppOwnedPointers; // Prefilled in constructor
    csp::common::List<BindingsTestType*> m_listOfPointers;
    csp::common::List<BindingsTestType*> m_listOfCppOwnedPointers; // Prefilled in constructor
    csp::common::Map<int, BindingsTestType*> m_mapOfPointers;
    csp::common::Map<int, BindingsTestType*> m_mapOfCppOwnedPointers; // Prefilled in constructor
    // Optional to pointer isn't a pattern we express (I think).

    // We may need to add List<T*>* for annoying reasons, I believe this is a pattern
    // expressed due to the legacy wrapper generator, which we may need to support
    // in a transitionary capacity.
public:
    ContainerBindingMechanismsTestType() : m_arrayOfCppOwnedPointers(2)
    {
        // Yes, these will technically leak, but who cares in test code.
        m_arrayOfCppOwnedPointers[0] = new BindingsTestType(1, "One");
        m_arrayOfCppOwnedPointers[1] = new BindingsTestType(2, "Two");

        m_listOfCppOwnedPointers.Append(new BindingsTestType(1, "One"));
        m_listOfCppOwnedPointers.Append(new BindingsTestType(2, "Two"));

        m_mapOfCppOwnedPointers[1] = new BindingsTestType(1, "One");
        m_mapOfCppOwnedPointers[2] = new BindingsTestType(2, "Two");

        m_singlePointerType = new BindingsTestType(1, "One");
    }
};

EMSCRIPTEN_BINDINGS(CSPContainerTestTypeBindings)
{

    emscripten::class_<ContainerBindingMechanismsTestType>("ContainerBindingMechanismsTestType")
        .class_function(
            "create", +[]() { return ContainerBindingMechanismsTestType(); })
        // Single objects don't need JSDisposable: a bound-class handle already carries
        // [Symbol.dispose] (embind maps it to delete()), unlike containers which marshal
        // to plain JS arrays/objects. Returning by value gives JS an owned, disposable handle.
        .function(
            "getSingleFullTypeByValue", +[](const ContainerBindingMechanismsTestType& self) { return self.GetSingleFullTypeByValue(); })
        .function(
            "getSingleFullTypeAsConstRef", +[](const ContainerBindingMechanismsTestType& self) { return self.GetSingleFullTypeAsConstRef(); })
        .function(
            "getSingleFullTypeAsPointer",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::NonOwningValRef<BindingsTestTypePointer>(self.GetSingleFullTypeAsPointer()); })
        .function(
            "getArrayBasicTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Array<int>> { self.GetArrayBasicTypeByValue() }; })
        .function(
            "getArrayBasicTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Array<int>> { self.GetArrayBasicTypeByConstRef() }; })
        .function("setArrayBasicTypeByValue(value)", &ContainerBindingMechanismsTestType::SetArrayBasicTypeByValue)
        .function("setArrayBasicTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetArrayBasicTypeByConstRef)
        .function(
            "getArrayFullTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>> { self.GetArrayFullTypeByValue() }; })
        .function(
            "getArrayFullTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>> { self.GetArrayFullTypeByConstRef() };
            })
        .function("setArrayFullTypeByValue(value)", &ContainerBindingMechanismsTestType::SetArrayFullTypeByValue)
        .function("setArrayFullTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetArrayFullTypeByConstRef)
        .function("getArrayOfPointersByValue", &ContainerBindingMechanismsTestType::GetArrayOfPointersByValue)
        .function("getArrayOfPointersByConstRef", &ContainerBindingMechanismsTestType::GetArrayOfPointersByConstRef)
        .function("getArrayOfCppOwnedPointers", &ContainerBindingMechanismsTestType::GetArrayOfCppOwnedPointers)
        .function("setArrayOfPointersByValue(value)", &ContainerBindingMechanismsTestType::SetArrayOfPointersByValue)
        .function("setArrayOfPointersByConstRef(value)", &ContainerBindingMechanismsTestType::SetArrayOfPointersByConstRef)
        .function(
            "getArrayStringByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::common::String>> { self.GetArrayStringByValue() }; })
        .function(
            "getArrayStringByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::common::String>> { self.GetArrayStringByConstRef() };
            })
        .function("setArrayStringByValue(value)", &ContainerBindingMechanismsTestType::SetArrayStringByValue)
        .function("setArrayStringByConstRef(value)", &ContainerBindingMechanismsTestType::SetArrayStringByConstRef)
        .function(
            "getListBasicTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::List<int>> { self.GetListBasicTypeByValue() }; })
        .function(
            "getListBasicTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::List<int>> { self.GetListBasicTypeByConstRef() }; })
        .function("setListBasicTypeByValue(value)", &ContainerBindingMechanismsTestType::SetListBasicTypeByValue)
        .function("setListBasicTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetListBasicTypeByConstRef)
        .function(
            "getListFullTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::List<BindingsTestType>> { self.GetListFullTypeByValue() }; })
        .function(
            "getListFullTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::List<BindingsTestType>> { self.GetListFullTypeByConstRef() }; })
        .function("setListFullTypeByValue(value)", &ContainerBindingMechanismsTestType::SetListFullTypeByValue)
        .function("setListFullTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetListFullTypeByConstRef)
        .function("getListOfPointersByValue", &ContainerBindingMechanismsTestType::GetListOfPointersByValue)
        .function("getListOfPointersByConstRef", &ContainerBindingMechanismsTestType::GetListOfPointersByConstRef)
        .function("getListOfCppOwnedPointers", &ContainerBindingMechanismsTestType::GetListOfCppOwnedPointers)
        .function("setListOfPointersByValue(value)", &ContainerBindingMechanismsTestType::SetListOfPointersByValue)
        .function("setListOfPointersByConstRef(value)", &ContainerBindingMechanismsTestType::SetListOfPointersByConstRef)
        .function(
            "getMapBasicTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Map<int, int>> { self.GetMapBasicTypeByValue() }; })
        .function(
            "getMapBasicTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Map<int, int>> { self.GetMapBasicTypeByConstRef() }; })
        .function("setMapBasicTypeByValue(value)", &ContainerBindingMechanismsTestType::SetMapBasicTypeByValue)
        .function("setMapBasicTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetMapBasicTypeByConstRef)
        .function(
            "getMapFullTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) { return bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>> { self.GetMapFullTypeByValue() }; })
        .function(
            "getMapFullTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>> { self.GetMapFullTypeByConstRef() };
            })
        .function("setMapFullTypeByValue(value)", &ContainerBindingMechanismsTestType::SetMapFullTypeByValue)
        .function("setMapFullTypeByConstRef(value)", &ContainerBindingMechanismsTestType::SetMapFullTypeByConstRef)
        .function("getMapOfPointersByValue", &ContainerBindingMechanismsTestType::GetMapOfPointersByValue)
        .function("getMapOfPointersByConstRef", &ContainerBindingMechanismsTestType::GetMapOfPointersByConstRef)
        .function("getMapOfCppOwnedPointers", &ContainerBindingMechanismsTestType::GetMapOfCppOwnedPointers)
        .function("setMapOfPointersByValue(value)", &ContainerBindingMechanismsTestType::SetMapOfPointersByValue)
        .function("setMapOfPointersByConstRef(value)", &ContainerBindingMechanismsTestType::SetMapOfPointersByConstRef)
        .function(
            "getMapStringIntByValue",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>> { self.GetMapStringIntByValue() };
            })
        .function(
            "getMapStringIntByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>> { self.GetMapStringIntByConstRef() };
            })
        .function("setMapStringIntByValue(value)", &ContainerBindingMechanismsTestType::SetMapStringIntByValue)
        .function("setMapStringIntByConstRef(value)", &ContainerBindingMechanismsTestType::SetMapStringIntByConstRef)
        .function(
            "getMapStringStringByValue",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>> { self.GetMapStringStringByValue() };
            })
        .function(
            "getMapStringStringByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>> { self.GetMapStringStringByConstRef() };
            })
        .function("setMapStringStringByValue(value)", &ContainerBindingMechanismsTestType::SetMapStringStringByValue)
        .function("setMapStringStringByConstRef(value)", &ContainerBindingMechanismsTestType::SetMapStringStringByConstRef)
        .function("getCspStringByValue", &ContainerBindingMechanismsTestType::GetCspStringByValue)
        .function("getCspStringByConstRef", &ContainerBindingMechanismsTestType::GetCspStringByConstRef)
        .function("setCspStringByValue", &ContainerBindingMechanismsTestType::SetCspStringByValue)
        .function("setCspStringByConstRef", &ContainerBindingMechanismsTestType::SetCspStringByConstRef)
        .function("getOptionalBasicTypeByValue", &ContainerBindingMechanismsTestType::GetOptionalBasicTypeByValue)
        .function("getOptionalBasicTypeByConstRef", &ContainerBindingMechanismsTestType::GetOptionalBasicTypeByConstRef)
        .function("setOptionalBasicTypeByValue", &ContainerBindingMechanismsTestType::SetOptionalBasicTypeByValue)
        .function("setOptionalBasicTypeByConstRef", &ContainerBindingMechanismsTestType::SetOptionalBasicTypeByConstRef)
        .function("getOptionalFullTypeByValue", &ContainerBindingMechanismsTestType::GetOptionalFullTypeByValue)
        .function("getOptionalFullTypeByConstRef", &ContainerBindingMechanismsTestType::GetOptionalFullTypeByConstRef)
        .function("setOptionalFullTypeByValue", &ContainerBindingMechanismsTestType::SetOptionalFullTypeByValue)
        .function("setOptionalFullTypeByConstRef", &ContainerBindingMechanismsTestType::SetOptionalFullTypeByConstRef)
        .function("getOptionalStringByValue", &ContainerBindingMechanismsTestType::GetOptionalStringByValue)
        .function("getOptionalStringByConstRef", &ContainerBindingMechanismsTestType::GetOptionalStringByConstRef)
        .function("setOptionalStringByValue", &ContainerBindingMechanismsTestType::SetOptionalStringByValue)
        .function("setOptionalStringByConstRef", &ContainerBindingMechanismsTestType::SetOptionalStringByConstRef)
        .function(
            "getOptionalListFullTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<BindingsTestType>>> { self.GetOptionalListFullTypeByValue() };
            })
        .function(
            "getOptionalListFullTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<BindingsTestType>>> { self.GetOptionalListFullTypeByConstRef() };
            })
        .function("setOptionalListFullTypeByValue", &ContainerBindingMechanismsTestType::SetOptionalListFullTypeByValue)
        .function("setOptionalListFullTypeByConstRef", &ContainerBindingMechanismsTestType::SetOptionalListFullTypeByConstRef)
        .function(
            "getListOfOptionalFullTypeByValue",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::List<csp::common::Optional<BindingsTestType>>> { self.GetListOfOptionalFullTypeByValue() };
            })
        .function(
            "getListOfOptionalFullTypeByConstRef",
            +[](const ContainerBindingMechanismsTestType& self) {
                return bindings::utils::JSDisposable<csp::common::List<csp::common::Optional<BindingsTestType>>> { self.GetListOfOptionalFullTypeByConstRef() };
            })
        .function("setListOfOptionalFullTypeByValue", &ContainerBindingMechanismsTestType::SetListOfOptionalFullTypeByValue)
        .function("setListOfOptionalFullTypeByConstRef", &ContainerBindingMechanismsTestType::SetListOfOptionalFullTypeByConstRef);
}
