#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "serialize/list.hpp"
#include "serialize/structure.hpp"
#include "serialize/numeric.hpp"

TEST(SerializeList, unpack) {
    const uint8_t data[] = { 1, 9, 2, 8, 3, 7, 0xFF, 0xFF };
    serialize::Buffer buffer(data, data + sizeof(data));

    // Create the value schema
    auto pair = new serialize::Structure;
    auto first = new serialize::Byte;
    auto second = new serialize::Byte;
    pair->addField("first", first);
    pair->addField("second", second);

    // Create the sentinel value
    auto sentinel = new serialize::StructureValue(pair);
    sentinel->setField("first", new serialize::ByteValue(first, 0xFF));
    sentinel->setField("second", new serialize::ByteValue(second, 0xFF));

    // Create the final list schema
    serialize::List schema(pair, sentinel);

    // Unpack the data
    serialize::Value* value = schema.unpack(buffer, 0);

    // Build array
    std::vector<int> output;

    auto& list = *dynamic_cast<serialize::ListValue*>(value);
    for (auto& element : list) {
        auto& pair = *dynamic_cast<serialize::StructureValue*>(element);

        auto& first = *dynamic_cast<serialize::ByteValue*>(pair.getField("first"));
        auto& second = *dynamic_cast<serialize::ByteValue*>(pair.getField("second"));

        output.push_back(first.getValue());
        output.push_back(second.getValue());
    }

    ASSERT_THAT(output, testing::ElementsAre(1, 9, 2, 8, 3, 7));

    delete value;
}
