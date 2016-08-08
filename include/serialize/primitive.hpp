#ifndef SERIALIZE_PRIMITIVE_HPP_
#define SERIALIZE_PRIMITIVE_HPP_

#include <cstdint>
#include <limits>
#include <boost/endian/conversion.hpp>
#include <boost/integer/static_log2.hpp>
#include "base.hpp"

namespace serialize {
    template <typename T>
    class Primitive : public Leaf {
    public:
	class Value : public Leaf::Value {
	public:
	    Value(const Schema* schema, T value)
		: Leaf::Value(schema), value_(value) {
		if (!schema->valid(this)) {
		    throw std::runtime_error("Invalid value for schema");
		}
	    }

	    virtual std::size_t size() const {
		return sizeof(T);
	    }

	    T operator*() const {
		return value_;
	    }

	protected:
	    T value_;
	};

	Primitive(T min=std::numeric_limits<T>::min(), T max=std::numeric_limits<T>::max())
	    : min_(min), max_(max) {}

	virtual std::size_t alignment() const {
	    return boost::static_log2<sizeof(T)>::value;
	}

	virtual bool valid(const Schema::Value* value) const {
	    try {
		auto& primitive = *dynamic_cast<const Primitive::Value*>(value);
		return *primitive >= min_ && *primitive < max_;
	    } catch (std::bad_cast& e) {
		return false;
	    }
	}

	virtual Schema::Value* unpack(const Buffer& buffer,
				      std::size_t offset,
				      const Schema::Value* context=nullptr) const {
	    if (offset + sizeof(T) >= buffer.size()) {
		throw std::out_of_range("Buffer out of range");
	    }

	    T value(0);
	    const uint8_t* data = buffer.begin() + offset;

	    for (std::size_t i = 0; i < sizeof(T); i++) {
		value |= *data++ << (i * 8);
	    }

	    return dynamic_cast<Schema::Value*>(new Value(this, value));
	}

    protected:
	T min_, max_;
    };

    typedef Primitive<uint8_t> Byte;
    typedef Primitive<uint16_t> Hword;
    typedef Primitive<uint32_t> Word;
}

#endif /* SERIALIZE_PRIMITIVE_HPP_ */
