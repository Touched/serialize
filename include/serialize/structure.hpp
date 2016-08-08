#ifndef SERIALIZE_STRUCTURE_HPP_
#define SERIALIZE_STRUCTURE_HPP_

#include <unordered_map>
#include <vector>
#include <string>

#include "base.hpp"

namespace serialize {
    class Structure;

    class StructureValue : public CompositeValue {
    public:
        StructureValue(const Structure* schema);

        virtual ~StructureValue();

        virtual std::size_t size() const;

        void setField(const std::string& key, Value* value);

        Value* getField(const std::string& key);

        friend Structure;
    protected:
        std::vector<Value*> values_;
        const Structure* structure_;
    };

    class Structure : public Composite {
    public:
        Structure();

        virtual ~Structure();

        virtual std::size_t alignment() const;

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

        void addField(const std::string& key, Schema* valueSchema);

        friend StructureValue;
    protected:
        std::unordered_map<std::string, std::size_t> keys_;
        std::vector<Schema*> schemas_;
    };
}

#endif /* SERIALIZE_STRUCTURE_HPP_ */
