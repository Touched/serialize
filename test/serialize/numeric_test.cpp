#include <gtest/gtest.h>
#include "serialize/numeric.hpp"

TEST(SerializeNumeric, unpack) {
    const uint8_t data[] = { 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::Schema* schema = new serialize::Hword();
    serialize::Value* value = schema->unpack(buffer, 2);

    auto& byte = *dynamic_cast<serialize::HwordValue*>(value);

    ASSERT_EQ(*byte, 0x403);
}
