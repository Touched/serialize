#ifndef SERIALIZE_LIST_HPP_
#define SERIALIZE_LIST_HPP_

#include <list>
#include "base.hpp"
#include "virtual.hpp"
#include "visitor.hpp"

namespace serialize {
    class List;

    class ListValue : public CompositeValue {
    public:
        typedef std::list<Value*>::iterator iterator;

        ListValue(const List* schema);

        virtual ~ListValue();

        virtual std::size_t size() const;

        void pushElement(Value* value);

        iterator begin();

        iterator end();

        virtual void accept(ValueVisitor& visitor) {
            visitor.visit(this);
        }

    protected:
        virtual bool equals(const Value& other) const;

        std::list<Value*> values_;
    };

    class List : public Composite {
    public:
        List(Schema* element, Value* sentinel);

        virtual ~List();

        virtual std::size_t alignment() const;

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

    protected:
        Schema* element_;
        Value* sentinel_;
    };
}

#endif /* SERIALIZE_LIST_HPP_ */
