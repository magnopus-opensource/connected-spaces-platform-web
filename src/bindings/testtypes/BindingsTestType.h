/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */

#pragma once

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
#include <string>

/*
 * A nonsense, instrumentable C++ object that we can bind and test mechanisms with.
 */
class BindingsTestType {
public:
    BindingsTestType() { ++AliveCount; }
    BindingsTestType(int value, std::string name) : m_value(value), m_name(std::move(name)) { ++AliveCount; }

    BindingsTestType(const BindingsTestType& other) : m_value(other.m_value), m_name(other.m_name) { ++AliveCount; }
    BindingsTestType(BindingsTestType&& other) noexcept : m_value(other.m_value), m_name(std::move(other.m_name)) { ++AliveCount; }

    /* No Constructor/Destructor dance when doing copy/move assignment, so AliveCount remains stable*/
    BindingsTestType& operator=(const BindingsTestType&) = default;
    BindingsTestType& operator=(BindingsTestType&&) noexcept = default;

    ~BindingsTestType() { --AliveCount; }

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
