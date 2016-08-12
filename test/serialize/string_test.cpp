#include <gtest/gtest.h>
#include "serialize/string.hpp"

TEST(SerializeString, unpack) {
    const uint8_t data[] = { 0xc2, 0xd9, 0xe0, 0xe0, 0xe3, 0xb8, 0,
                             0xd1, 0xe3, 0xe6, 0xe0, 0xd8, 0xab, 0xff };

    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::String schema;
    serialize::Value* value = schema.unpack(buffer, 0);

    auto& s = *dynamic_cast<serialize::StringValue*>(value);
    ASSERT_EQ(s.string(), "Hello, World!");

    delete value;
}
