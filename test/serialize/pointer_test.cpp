#include <gtest/gtest.h>
#include "serialize/pointer.hpp"

TEST(SerializePointer, unpack) {
    const uint8_t data[] = { 1, 2, 3, 4, 2, 0, 0, 0x8 };
    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::Schema* schema = new serialize::Pointer(new serialize::Hword);
    serialize::Value* value = schema->unpack(buffer, 4);

    auto& pointer = *dynamic_cast<serialize::PointerValue*>(value);
    auto& number = *dynamic_cast<serialize::HwordValue*>(pointer.dereference());

    ASSERT_EQ(*number, 0x403);
}
