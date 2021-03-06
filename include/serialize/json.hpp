#ifndef SERIALIZE_JSON_HPP_
#define SERIALIZE_JSON_HPP_

#include "visitor.hpp"
#include "structure.hpp"
#include "array.hpp"
#include "numeric.hpp"
#include "pointer.hpp"
#include "bitfield.hpp"

namespace serialize {
    template <typename Writer>
    class JsonSerializer : public ValueVisitor {
    public:
        JsonSerializer(Writer& writer) : writer_(writer) {}

        virtual void visit(VirtualValue* virt) {
            writer_.Uint(virt->getValue());
        }

        virtual void visit(PointerValue* pointer) {
            Value* wrapped = pointer->dereference();

            if (wrapped) {
                wrapped->accept(*this);
            } else {
                writer_.Null();
            }
        }

        virtual void visit(WordValue* word) {
            writer_.Uint(word->getValue());
        }

        virtual void visit(HwordValue* hword) {
            writer_.Uint(hword->getValue());
        }

        virtual void visit(ByteValue* byte) {
            writer_.Uint(byte->getValue());
        }

        virtual void visit(ArrayValue* array) {
            writer_.StartArray();

            for (const auto& element : *array) {
                element->accept(*this);
            }

            writer_.EndArray();
        }

        virtual void visit(BitfieldValue* bitfield) {
            writer_.StartObject();

            for (const auto& item : *bitfield) {
                writer_.String(item.first.c_str(), item.first.length());
                writer_.Uint(item.second);
            }

            writer_.EndObject();
        }

        virtual void visit(StructureValue* structure) {
            writer_.StartObject();

            for (const auto& item : *structure) {
                writer_.String(item.first.c_str(), item.first.length());
                item.second->accept(*this);
            }

            writer_.EndObject();
        }

    private:
        Writer& writer_;
    };

    typedef std::unordered_map<std::string, Schema*> SchemaMapping;

    void load_schemas(std::string json_path, SchemaMapping& schemas);
}

#endif /* SERIALIZE_JSON_HPP_ */
