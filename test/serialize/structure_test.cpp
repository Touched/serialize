#include <gtest/gtest.h>
#include "serialize/structure.hpp"
#include "serialize/numeric.hpp"

TEST(SerializeStructure, unpack) {
    const uint8_t data[] = { 1, 0, 0, 0, 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    serialize::Structure schema;
    schema.addField("a", new serialize::Byte);
    schema.addField("b", new serialize::Word);

    serialize::Value* value = schema.unpack(buffer, 0);

    auto& structure = *dynamic_cast<serialize::StructureValue*>(value);
    auto& a = *dynamic_cast<serialize::ByteValue*>(structure.getField("a"));
    auto& b = *dynamic_cast<serialize::WordValue*>(structure.getField("b"));

    ASSERT_EQ(*a, 0x1);
    ASSERT_EQ(*b, 0x4030201);

    delete value;
}
