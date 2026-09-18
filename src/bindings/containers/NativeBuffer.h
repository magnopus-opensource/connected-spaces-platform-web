#pragma once

#include "emscripten/val.h"
#include <cstddef>
#include <cstdint>

// Val type representing a Uint8Array in JavaScript
EMSCRIPTEN_DECLARE_VAL_TYPE(ByteArray);

namespace bindings::containers {

/*
 * A contiguous block of memory used for transferring binary data to and from JavaScript.
 */
class NativeBuffer {
public:
    // Create the NativeBuffer from a ByteArray (Uint8Array), copying its contents.
    static NativeBuffer* CreateFromByteArray(const ByteArray& byteArray);

    explicit NativeBuffer(std::size_t length);

    NativeBuffer(const NativeBuffer&) = delete;
    NativeBuffer& operator=(const NativeBuffer&) = delete;
    NativeBuffer(NativeBuffer&&) = delete;
    NativeBuffer& operator=(NativeBuffer&&) = delete;

    std::uint8_t* GetData() { return m_data.data(); }
    const std::uint8_t* GetData() const { return m_data.data(); }
    std::size_t GetLength() const { return m_data.size(); }

private:
    std::vector<std::uint8_t> m_data;
};

/*
 * Create a ByteArray (Uint8Array) copy of the supplied memory block.
 * This is an independent copy with its own lifetime.
 */
ByteArray MakeByteArrayCopy(const std::uint8_t* data, std::size_t length);

/*
 * Create a ByteArray (Uint8Array) view into the supplied memory block.
 * This is a view into the live memory which does not keep it alive, so the caller must ensure that
 * the original memory block remains valid while the view is used.
 */
ByteArray MakeByteArrayView(const std::uint8_t* data, std::size_t length);

} // namespace bindings::containers
