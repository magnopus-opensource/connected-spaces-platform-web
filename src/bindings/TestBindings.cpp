/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */


#include "emscripten/bind.h"
#include <string>

/*
 * A nonsense, instrumentable C++ object that we can bind and test mechanisms with.
 */
class BindingsTestType
{
public:
    BindingsTestType() {
        ++AliveCount;
    }
    BindingsTestType(int value, std::string name) : Value(value), Name(std::move(name)) {++AliveCount;}

    BindingsTestType(const BindingsTestType& other) : Value(other.Value), Name(other.Name) { ++AliveCount; }
    BindingsTestType(BindingsTestType&& other) noexcept : Value(other.Value), Name(std::move(other.Name)) { ++AliveCount; }

    /* No Constructor/Destructor dance when doing copy/move assignment, so AliveCount remains stable*/
    BindingsTestType& operator=(const BindingsTestType&) = default;
    BindingsTestType& operator=(BindingsTestType&&) noexcept = default;

    ~BindingsTestType(){--AliveCount;}

    int GetValue() const { return Value; }
    void SetValue(int value) { Value = value; }

    const std::string& GetName() const { return Name; }
    void SetName(std::string name) { Name = std::move(name); }

    // Test instrumentation, keep track of how many instances of this object exist
    static inline int AliveCount = 0;

private:
    int Value = 0;
    std::string Name;
};

EMSCRIPTEN_BINDINGS(CSPTestBindings)
{
    emscripten::class_<BindingsTestType>("BindingsTestType")
        .constructor<>()
        .constructor<int, std::string>()
        .property("value", &BindingsTestType::GetValue, &BindingsTestType::SetValue)
        .property("name", &BindingsTestType::GetName, &BindingsTestType::SetName)
        .class_property("aliveCount", &BindingsTestType::AliveCount);
}
