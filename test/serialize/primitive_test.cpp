#include <gtest/gtest.h>
#include "serialize/primitive.hpp"

TEST(SerializePrimitive, unpack) {
    const uint8_t data[] = { 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::Schema* schema = new serialize::Byte();
    serialize::Schema::Value* value = schema->unpack(buffer, 2);

    serialize::Byte::Value& byte = *dynamic_cast<serialize::Byte::Value*>(value);

    ASSERT_EQ(*byte, 3);
}
