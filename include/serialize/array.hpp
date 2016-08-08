#ifndef SERIALIZE_ARRAY_HPP_
#define SERIALIZE_ARRAY_HPP_

#include <vector>
#include "base.hpp"
#include "virtual.hpp"

namespace serialize {
    class Array;

    class ArrayValue : public CompositeValue {
    public:
        ArrayValue(const Array* schema, std::size_t length);

        virtual ~ArrayValue();

        virtual std::size_t size() const;

        void setElement(std::size_t index, Value* value);

        Value* getElement(std::size_t index);

    protected:
        std::vector<Value*> values_;
    };

    class Array : public Composite {
    public:
        Array(Schema* element, Virtual* length);

        virtual ~Array();

        virtual std::size_t alignment() const;

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

    protected:
        Schema* element_;
        Virtual* length_;
    };
}

#endif /* SERIALIZE_ARRAY_HPP_ */
