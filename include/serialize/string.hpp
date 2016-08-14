#ifndef SERIALIZE_STRING_HPP_
#define SERIALIZE_STRING_HPP_

#include <string>
#include "base.hpp"
#include "visitor.hpp"

namespace serialize {
    class String;

    class StringValue : public BinaryValue {
    public:
        StringValue(const String* schema, std::size_t size, const std::string&& value);

        virtual ~StringValue();

        virtual std::size_t size() const;

        virtual const std::string& string() const;

        virtual void accept(ValueVisitor& visitor);

    protected:
        virtual bool equals(const Value& other) const;

        std::size_t size_;
        std::string value_;
    };

    class String : public Binary {
    public:
        String();

        virtual ~String();

        virtual std::size_t alignment() const;

        virtual void accept(SchemaVisitor& visitor);

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

    };
}

#endif /* SERIALIZE_STRING_HPP_ */
