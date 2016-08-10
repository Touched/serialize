#ifndef SERIALIZE_POINTER_HPP_
#define SERIALIZE_POINTER_HPP_

#include "numeric.hpp"
#include "visitor.hpp"

namespace serialize {
    class Pointer;

    class PointerValue : public WordValue {
    public:
        PointerValue(const Pointer* schema, uint32_t address, Value* wrapped);
        virtual ~PointerValue();
        Value* dereference() const;

        virtual void accept(ValueVisitor& visitor) {
            visitor.visit(this);
        }

    protected:
        virtual bool equals(const Value& other) const;

        Value* wrapped_;
    };

    class Pointer : public Word {
    public:
        Pointer(Schema* wrapped);
        virtual ~Pointer();
	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

    protected:
        Schema* wrapped_;
    };
}

#endif /* SERIALIZE_POINTER_HPP_ */
