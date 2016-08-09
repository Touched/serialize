#ifndef SERIALIZE_VIRTUAL_HPP_
#define SERIALIZE_VIRTUAL_HPP_

#include "base.hpp"
#include "visitor.hpp"

namespace serialize {
    class Virtual;

    class VirtualValue : public ScalarValue {
    public:
        VirtualValue(const Virtual* schema, const ScalarValue* wrapped);
        virtual ~VirtualValue();
        virtual std::size_t size() const;
        virtual std::size_t getValue() const;
        virtual void accept(ValueVisitor& visitor) {
            visitor.visit(this);
        }

    protected:
        const ScalarValue* wrapped_;
    };

    /**
     * A value that will be stored in the deserialization context.
     */
    class Virtual : public Scalar {
    public:
        Virtual(Scalar* wrapped);
        virtual ~Virtual();
        virtual std::size_t alignment() const;
	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

    protected:
        Scalar* wrapped_;
    };
}

#endif /* SERIALIZE_VIRTUAL_HPP_ */
