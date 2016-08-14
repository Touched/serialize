#ifndef SERIALIZE_VISITOR_HPP_
#define SERIALIZE_VISITOR_HPP_

namespace serialize {
    class ArrayValue;
    class BitfieldValue;
    class ListValue;
    class PointerValue;
    class StringValue;
    class StructureValue;
    class VirtualValue;

    template <typename T>
    class NumericValue;

    class ValueVisitor {
    public:
        ValueVisitor() {}
        virtual ~ValueVisitor() {}

        virtual void visit(NumericValue<uint8_t>*) {}
        virtual void visit(NumericValue<uint16_t>*) {}
        virtual void visit(NumericValue<uint32_t>*) {}
        virtual void visit(ArrayValue*) {}
        virtual void visit(BitfieldValue*) {}
        virtual void visit(ListValue*) {}
        virtual void visit(PointerValue*) {}
        virtual void visit(StringValue*) {}
        virtual void visit(StructureValue*) {}
        virtual void visit(VirtualValue*) {}
    };

    class Array;
    class Bitfield;
    class Compression;
    class List;
    class Pointer;
    class String;
    class Structure;
    class Virtual;

    template <typename T>
    class Numeric;

    class SchemaVisitor {
    public:
        SchemaVisitor() {}
        virtual ~SchemaVisitor() {}

        virtual void visit(Numeric<uint8_t>*) {}
        virtual void visit(Numeric<uint16_t>*) {}
        virtual void visit(Numeric<uint32_t>*) {}
        virtual void visit(Array*) {}
        virtual void visit(Bitfield*) {}
        virtual void visit(Compression*) {}
        virtual void visit(List*) {}
        virtual void visit(Pointer*) {}
        virtual void visit(String*) {}
        virtual void visit(Structure*) {}
        virtual void visit(Virtual*) {}
    };
};

#endif /* SERIALIZE_VISITOR_HPP_ */
