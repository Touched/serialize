#ifndef SERIALIZE_BASE_HPP_
#define SERIALIZE_BASE_HPP_

#include <typeinfo>
#include <cstdint>
#include "buffer.hpp"
#include "context.hpp"

namespace serialize {
    class Schema;
    class ValueVisitor;

    class Value {
    public:
        Value(const Schema* schema);
        virtual ~Value();
        virtual std::size_t size() const = 0;
        virtual void accept(ValueVisitor& visitor) = 0;

        bool operator==(const Value& other) const {
            return typeid(*this) == typeid(other)
                && schema_ == other.schema_
                && equals(other);
        }

        inline bool operator!=(const Value& other) const {
            return !(*this == other);
        }

        static inline bool compare(const Value* lhs, const Value* rhs) {
            return *lhs == *rhs;
        }
    protected:
        virtual bool equals(const Value& other) const = 0;
        const Schema* schema_;
    };

    class Schema {
    public:
        virtual ~Schema();

	virtual std::size_t alignment() const;

	virtual Value* unpack(const Buffer& buffer,
			      std::size_t offset,
			      Context* context=new Context()) const = 0;
    };

    class Composite;

    class CompositeValue : public Value {
    public:
        CompositeValue(const Composite* schema);
        virtual ~CompositeValue();
    };

    class Composite : public Schema {
    public:
	Composite();
	virtual ~Composite();
    };

    class Scalar;

    class ScalarValue : public Value {
    public:
        ScalarValue(const Scalar* schema);
        virtual ~ScalarValue();
        virtual std::size_t getValue() const = 0;
    };

    class Scalar : public Schema {
    public:
	Scalar();
	virtual ~Scalar();
    };
}

#endif /* SERIALIZE_BASE_HPP_ */
