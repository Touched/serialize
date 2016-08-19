#include <gtest/gtest.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include "serialize/json.hpp"

class JsonSchemaTest : public ::testing::TestWithParam<const char*> {
public:
    std::string BasePath() const {
        return std::string("test/fixtures/schemas/") + GetParam() + "/";
    }

    std::string GetSchemaPath() const {
        return BasePath() + "schema.json";
    }

    std::string GetInputPath() const {
        return BasePath() + "input.txt";
    }

    std::string GetOutputPath() const {
        return BasePath() + "output.json";
    }
};

TEST_P(JsonSchemaTest, loadSchema) {
    serialize::SchemaMapping mapping;
    serialize::load_schemas(GetSchemaPath(), mapping);

    // Read input
    std::ifstream input_file(GetInputPath());
    std::vector<uint8_t> input;
    while (!input_file.eof()) {
        unsigned byte;
        std::string buffer;
        std::stringstream stream;

        // Read two characters from the input stream
        if (input_file >> std::setw(2) >> buffer) {
            // Parse hex
            stream << buffer;
            stream >> std::hex >> byte;

            input.push_back(byte);
        }
    };

    // Unpack the schema
    auto schema = mapping[GetParam()];
    serialize::Buffer buffer(input.data(), input.data() + input.size());
    auto value = schema->unpack(buffer, 0);

    // Serialize
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    serialize::JsonSerializer<
        rapidjson::Writer<rapidjson::StringBuffer>> serializer(writer);
    value->accept(serializer);

    // Load expected output
    std::ifstream expected_output_file(GetOutputPath());
    std::string expected_output;
    expected_output_file >> expected_output;

    ASSERT_STREQ(sb.GetString(), expected_output.c_str());

    delete schema;
    delete value;

}

INSTANTIATE_TEST_CASE_P(SerializeJsonSchema,
                        JsonSchemaTest,
                        ::testing::Values("simple"));
