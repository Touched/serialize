#include <gtest/gtest.h>
#include "serialize/bitfield.hpp"

TEST(SerializeBitfield, unpack) {
    const uint8_t data[] = { 0xf7, 0x68 };
    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::Bitfield schema;
    schema.addField("a", 7);
    schema.addField("b", 9);

    serialize::Value* value = schema.unpack(buffer, 0);

    auto& bitfield = *dynamic_cast<serialize::BitfieldValue*>(value);
    auto a = bitfield.getField("a");
    auto b = bitfield.getField("b");

    ASSERT_EQ(a, 119);
    ASSERT_EQ(b, 209);

    delete value;
}
