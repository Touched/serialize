#ifndef SERIALIZE_JSON_HPP_
#define SERIALIZE_JSON_HPP_

#include "visitor.hpp"
#include "structure.hpp"
#include "array.hpp"
#include "numeric.hpp"
#include "pointer.hpp"

namespace serialize {
    template <typename Writer>
    class JsonSerializer : public ValueVisitor {
    public:
        JsonSerializer(Writer& writer) : writer_(writer) {}

        virtual void visit(VirtualValue* virt) {
            writer_.Uint(virt->getValue());
        }

        virtual void visit(PointerValue* pointer) {
            pointer->dereference()->accept(*this);
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
}

#endif /* SERIALIZE_JSON_HPP_ */
