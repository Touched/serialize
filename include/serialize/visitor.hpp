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
};

#endif /* SERIALIZE_VISITOR_HPP_ */
