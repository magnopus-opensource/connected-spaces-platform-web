/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */
#pragma once

#pragma once

#include "emscripten/val.h"
#include <string>
/*
 * A nonsense, instrumentable C++ object that we can bind and test mechanisms with.
 */
class BindingsTestType {
public:
    BindingsTestType();
    BindingsTestType(int value, std::string name);
    BindingsTestType(const BindingsTestType& other);
    BindingsTestType(BindingsTestType&& other) noexcept;
    BindingsTestType& operator=(const BindingsTestType& other);
    BindingsTestType& operator=(BindingsTestType&& other) noexcept;
    ~BindingsTestType();

    int GetValue() const;
    void SetValue(int value);

    const std::string& GetName() const;
    void SetName(std::string name);

    bool operator==(const BindingsTestType& other) const;

    // Test instrumentation, keep track of how many instances of this object exist
    static inline int AliveCount = 0;

private:
    int m_value = 0;
    std::string m_name;

    int m_uniqueID = 0; // Just for instrumentation, so we can decode lots of overlapping logs
    bool m_movedFrom = false; // So we don't decrement the alive count in the destructor of moved-from objects
};

// Allow the pointer type to have a name, as we use it as a raw val via NonOwningVal. Used in other translation units, so in the header.
EMSCRIPTEN_DECLARE_VAL_TYPE(BindingsTestTypePointer);
