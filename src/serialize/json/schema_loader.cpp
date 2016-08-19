#include <unordered_map>
#include <rapidjson/document.h>
#include <rapidjson/memorystream.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include "serialize/json.hpp"

#include <iostream>

namespace serialize {
    rapidjson::Document load_json(std::string json_path) {
        boost::iostreams::mapped_file json(json_path);
        rapidjson::MemoryStream s(json.data(), json.size());

        rapidjson::Document d;
        d.ParseStream(s);
        return d;
    }

    Schema* parse_type(rapidjson::Value& value, SchemaMapping& schemas);


    void required_field(rapidjson::Value& options, const char* field) {
        if (!options.HasMember(field)) {
            throw std::runtime_error("Options for type `" +
                                     std::string(options["type"].GetString()) +
                                     "` missing required field `" +
                                     std::string(field) +
                                     "`.");
        }
    }

    void required_field(rapidjson::Value& options,
                        const char* field,
                        rapidjson::Type type) {
        static const std::unordered_map<rapidjson::Type,
                                        const std::string> type_names = {
            { rapidjson::Type::kNullType, "null" },
            { rapidjson::Type::kFalseType, "false" },
            { rapidjson::Type::kTrueType, "true" },
            { rapidjson::Type::kObjectType, "object" },
            { rapidjson::Type::kArrayType, "array" },
            { rapidjson::Type::kNumberType, "number" },
        };

        required_field(options, field);

        auto actual_type = options[field].GetType();
        if (actual_type != type) {
            throw std::runtime_error("Options for type `" +
                                     std::string(options["type"].GetString()) +
                                     "` required field `" +
                                     std::string(field) +
                                     "` to be of type `" + type_names.at(type) +
                                     "` but got `" + type_names.at(actual_type) +
                                     "`.");
        }
    }

    Schema* structure_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        required_field(options, "fields", rapidjson::Type::kObjectType);

        auto& fields = options["fields"];
        auto result = new Structure;

        for (auto m = fields.MemberBegin(); fields.MemberEnd() != m; ++m) {
            result->addField(m->name.GetString(), parse_type(m->value, schemas));
        }

        return result;
    }

    Schema* virtual_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        required_field(options, "id", rapidjson::Type::kStringType);
        required_field(options, "wraps", rapidjson::Type::kObjectType);

        Schema* wrapped = parse_type(options["wraps"], schemas);
        Schema* result = new Virtual(dynamic_cast<Scalar*>(wrapped));

        schemas.emplace(std::make_pair(options["id"].GetString(), result));
        return result;
    }

    template <typename T>
    Schema* numeric_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        // TODO: Handle min/max
        return new T;
    }

    Schema* pointer_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        required_field(options, "target");
        Schema* wrapped = parse_type(options["target"], schemas);
        return new Pointer(wrapped);
    }

    Schema* array_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        required_field(options, "element");
        required_field(options, "length", rapidjson::Type::kStringType);

        Schema* element = parse_type(options["element"], schemas);
        Schema* length = schemas.at(options["length"].GetString());
        return new Array(element, dynamic_cast<Virtual*>(length));
    }

    Schema* bitfield_builder(rapidjson::Value& options, SchemaMapping& schemas) {
        required_field(options, "fields", rapidjson::Type::kObjectType);

        auto& fields = options["fields"];
        auto result = new Bitfield;

        for (auto m = fields.MemberBegin(); fields.MemberEnd() != m; ++m) {
            if (!m->value.IsNumber()) {
                throw std::runtime_error("Bitfield field must be a number");
            }

            result->addField(m->name.GetString(), m->value.GetUint());
        }

        return result;
    }

    typedef Schema* (*Builder)(rapidjson::Value& options, SchemaMapping& schemas);
    Schema* parse_type(rapidjson::Value& value, SchemaMapping& schemas) {
        static const std::unordered_map<std::string, Builder> builders = {
            { "structure", structure_builder },
            { "virtual", virtual_builder },
            { "byte", numeric_builder<Byte> },
            { "hword", numeric_builder<Hword> },
            { "word", numeric_builder<Word> },
            { "pointer", pointer_builder },
            { "array", array_builder },
            { "bitfield", bitfield_builder },
        };

        if (value.IsString()) {
            // Reference a custom type
            const std::string& type_name = value.GetString();

            try {
                return schemas.at(type_name);
            } catch (std::out_of_range& e) {
                throw std::runtime_error("Undefined type: " + type_name);
            }
        } else if (value.IsObject()) {
            // Parse and validate the type field
            if (!value.HasMember("type")) {
                throw std::runtime_error("`type` field missing");
            }

            auto& type = value["type"];

            if (!type.IsString()) {
                throw std::runtime_error("`type` field must be a string");
            }

            const std::string& type_name = type.GetString();

            try {
                return builders.at(type_name)(value, schemas);
            } catch (std::out_of_range& e) {
                throw std::runtime_error("Undefined type: " + type_name);
            }
        } else {
            throw std::runtime_error("Cannot parse type");
        }
    }

    void load_schemas(std::string json_path, SchemaMapping& schemas) {
        using namespace rapidjson;
        Document json = load_json(json_path);

        if (!json.IsObject()) {
            throw std::runtime_error("JSON document must be an object");
        }

        for (auto m = json.MemberBegin(); json.MemberEnd() != m; ++m) {
            Schema* schema = parse_type(m->value, schemas);
            schemas.emplace(std::make_pair(m->name.GetString(), schema));
        }
    }
}
