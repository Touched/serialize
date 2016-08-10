#ifndef SERIALIZE_NUMERIC_HPP_
#define SERIALIZE_NUMERIC_HPP_

#include <cstdint>
#include <limits>
#include <boost/endian/conversion.hpp>
#include <boost/integer/static_log2.hpp>
#include "base.hpp"
#include "visitor.hpp"

namespace serialize {
    template <typename T>
    class Numeric;

    template <typename T>
    class NumericValue : public ScalarValue {
    public:
        NumericValue(const Numeric<T>* schema, T value)
            : ScalarValue(schema), value_(value) {
            // TODO: Validate min/max constraints
        }

        virtual std::size_t size() const {
            return sizeof(T);
        }

        virtual std::size_t getValue() const {
            return value_;
        }

        T operator*() const {
            return value_;
        }

        virtual void accept(ValueVisitor& visitor) {
            visitor.visit(this);
        }

    protected:
        virtual bool equals(const Value& other) const {
            const auto& numeric = static_cast<const NumericValue<T>&>(other);
            return numeric.value_ == this->value_;
        }

        T value_;
    };

    template <typename T>
    class Numeric : public Scalar {
    public:
	Numeric(T min=std::numeric_limits<T>::min(), T max=std::numeric_limits<T>::max())
	    : min_(min), max_(max) {}

	virtual std::size_t alignment() const {
	    return boost::static_log2<sizeof(T)>::value;
	}

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const {
	    if (offset + sizeof(T) > buffer.size()) {
		throw std::out_of_range("Buffer out of range");
	    }

	    T value(0);
	    const uint8_t* data = buffer.begin() + offset;

	    for (std::size_t i = 0; i < sizeof(T); i++) {
		value |= *data++ << (i * 8);
	    }

	    return dynamic_cast<Value*>(new NumericValue<T>(this, value));
	}

    protected:
	T min_, max_;
    };

    typedef Numeric<uint8_t> Byte;
    typedef Numeric<uint16_t> Hword;
    typedef Numeric<uint32_t> Word;

    typedef NumericValue<uint8_t> ByteValue;
    typedef NumericValue<uint16_t> HwordValue;
    typedef NumericValue<uint32_t> WordValue;
}

#endif /* SERIALIZE_NUMERIC_HPP_ */
