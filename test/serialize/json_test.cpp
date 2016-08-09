#include <gtest/gtest.h>
#include <rapidjson/writer.h>
#include "serialize/json.hpp"

TEST(SerializeJson, buildsJson) {
    const uint8_t data[] = { 4, 0, 0, 0, 8, 0, 0, 8, 1, 2, 3, 4 };
    serialize::Buffer buffer(data, data + sizeof(data));

    auto length = new serialize::Virtual(new serialize::Word);

    serialize::Structure schema;
    schema.addField("length", length);
    schema.addField("array",
                    new serialize::Pointer(new serialize::Array(new serialize::Byte, length)));

    serialize::Value* value = schema.unpack(buffer, 0);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    serialize::JsonSerializer<rapidjson::Writer<rapidjson::StringBuffer>> serializer(writer);
    value->accept(serializer);

    ASSERT_STREQ(sb.GetString(), "{\"array\":[1,2,3,4],\"length\":4}");

    delete value;
}
