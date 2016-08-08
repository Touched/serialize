#ifndef SERIALIZE_BASE_HPP_
#define SERIALIZE_BASE_HPP_

#include <cstdint>
#include "buffer.hpp"

namespace serialize {
    class Schema {
    public:
	class Value {
	public:
	    Value(const Schema* schema);
	    virtual std::size_t size() const = 0;
	protected:
	    const Schema* schema_;
	};

	virtual std::size_t alignment() const;

	virtual bool valid(const Value* value) const = 0;

	virtual Value* unpack(const Buffer& begin,
			      std::size_t offset,
			      const Value* parent=nullptr) const = 0;
    };

    class Composite : public Schema {
    public:
	class Value : public Schema::Value {
	public:
	    Value(const Schema* schema);
	    virtual ~Value();
	};

	Composite();
	virtual ~Composite();
    };

    class Leaf : public Schema {
    public:
	class Value : public Schema::Value {
	public:
	    Value(const Schema* schema);
	    virtual ~Value();
	};

	Leaf();
	virtual ~Leaf();
    };
}

#endif /* SERIALIZE_BASE_HPP_ */
