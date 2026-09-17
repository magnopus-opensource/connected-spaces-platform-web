#include "NativeBuffer.h"

#include "emscripten/bind.h"
#include <stdexcept>

namespace bindings::containers {

NativeBuffer::NativeBuffer(std::size_t length) : m_data(nullptr), m_length(length)
{
    if (length == 0) {
        throw std::runtime_error("Length must be greater than 0");
    }

    m_data = new std::uint8_t[length]();
}

NativeBuffer::~NativeBuffer() { delete[] m_data; }

NativeBuffer* NativeBuffer::CreateFromByteArray(const ByteArray& byteArray)
{
    bindings::containers::NativeBuffer* buffer = new bindings::containers::NativeBuffer(byteArray["length"].as<std::size_t>());

    // Copy ByteArray contents using a view and TypeArray.set
    bindings::containers::MakeByteArrayView(buffer->GetData(), buffer->GetLength()).call<void>("set", byteArray);

    return buffer;
}

ByteArray MakeByteArrayCopy(const std::uint8_t* data, std::size_t length)
{
    /*
     * `new Uint8Array(typedArray)` in JavaScript copies the typed array, so this creates a detached
     * copy of the viewed memory.
     */
    return ByteArray { emscripten::val::global("Uint8Array").new_(MakeByteArrayView(data, length)) };
}

ByteArray MakeByteArrayView(const std::uint8_t* data, std::size_t length)
{
    if (data == nullptr || length == 0) {
        return ByteArray { emscripten::val::global("Uint8Array").new_(emscripten::val(0)) };
    }

    return ByteArray { emscripten::val(emscripten::typed_memory_view(length, data)) };
}

} // namespace bindings::containers

EMSCRIPTEN_BINDINGS(CSPNativeBuffer)
{
    emscripten::register_type<ByteArray>("Uint8Array");

    emscripten::class_<bindings::containers::NativeBuffer>("NativeBuffer")
        .class_function(
            "create(length)", +[](std::size_t length) { return new bindings::containers::NativeBuffer(length); }, emscripten::allow_raw_pointers(),
            emscripten::nonnull<emscripten::ret_val>())
        .class_function(
            "createFromByteArray(byteArray)", +[](ByteArray byteArray) { return bindings::containers::NativeBuffer::CreateFromByteArray(byteArray); },
            emscripten::allow_raw_pointers(), emscripten::nonnull<emscripten::ret_val>())
        .property("length", &bindings::containers::NativeBuffer::GetLength)
        .function(
            "getView", +[](const bindings::containers::NativeBuffer& self) { return bindings::containers::MakeByteArrayView(self.GetData(), self.GetLength()); })
        .function(
            "toByteArray", +[](const bindings::containers::NativeBuffer& self) { return bindings::containers::MakeByteArrayCopy(self.GetData(), self.GetLength()); });
}
