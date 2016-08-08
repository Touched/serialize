#include <gtest/gtest.h>
#include "serialize/array.hpp"
#include "serialize/structure.hpp"
#include "serialize/numeric.hpp"

TEST(SerializeArray, unpack) {
    const uint8_t data[] = { 4, 0, 0, 0, 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    auto length = new serialize::Virtual(new serialize::Word);

    serialize::Structure schema;
    schema.addField("length", length);
    schema.addField("array", new serialize::Array(new serialize::Byte, length));

    serialize::Value* value = schema.unpack(buffer, 0);

    auto& structure = *dynamic_cast<serialize::StructureValue*>(value);
    auto& array = *dynamic_cast<serialize::ArrayValue*>(structure.getField("array"));
    auto& element = *dynamic_cast<serialize::ByteValue*>(array.getElement(0));
    ASSERT_EQ(*element, 0x1);

    delete value;
}

TEST(SerializeArray, unpackNested) {
    const uint8_t data[] = { 2, 0, 2, 0, 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    auto width = new serialize::Virtual(new serialize::Hword);
    auto height = new serialize::Virtual(new serialize::Hword);

    serialize::Structure schema;
    schema.addField("width", width);
    schema.addField("height", height);
    schema.addField("grid",
                    new serialize::Array(new serialize::Array(new serialize::Byte,
                                                              width), height));

    serialize::Value* value = schema.unpack(buffer, 0);

    auto& structure = *dynamic_cast<serialize::StructureValue*>(value);
    auto& grid = *dynamic_cast<serialize::ArrayValue*>(structure.getField("grid"));
    auto& row = *dynamic_cast<serialize::ArrayValue*>(grid.getElement(1));
    auto& element = *dynamic_cast<serialize::ByteValue*>(row.getElement(0));

    ASSERT_EQ(*element, 0x3);

    delete value;
}
