#ifndef SERIALIZE_STRUCTURE_HPP_
#define SERIALIZE_STRUCTURE_HPP_

#include <unordered_map>
#include <vector>
#include <string>
#include <boost/iterator/iterator_adaptor.hpp>
#include "base.hpp"
#include "visitor.hpp"

namespace serialize {
    class Structure;

    class StructureIterator
        : public boost::iterator_adaptor<StructureIterator,
                                         std::unordered_map<std::string,
                                                            std::size_t>::const_iterator,
                                         const std::pair<const std::string&, Value*>,
                                         boost::forward_traversal_tag,
                                         const std::pair<const std::string&, Value*>
                                         > {
    public:
        StructureIterator(StructureValue* value,
                          const StructureIterator::iterator_adaptor_::base_type& it);
        const std::pair<const std::string&, Value*> dereference() const ;
    private:
        friend class boost::iterator_core_access;
        StructureValue* structure_;
    };

    class StructureValue : public CompositeValue {
    public:
        StructureValue(const Structure* schema);

        virtual ~StructureValue();

        virtual std::size_t size() const;

        void setField(const std::string& key, Value* value);

        Value* getField(const std::string& key);

        virtual void accept(ValueVisitor& visitor) {
            visitor.visit(this);
        }

        StructureIterator begin();
        StructureIterator end();

    protected:
        virtual bool equals(const Value& other) const;

        friend Structure;
        friend StructureIterator;
        const Structure* structure_;
        std::vector<Value*> values_;
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
        void calculateAlignment();

        std::unordered_map<std::string, std::size_t> keys_;
        std::vector<Schema*> schemas_;
        std::size_t alignment_;
    };
}

#endif /* SERIALIZE_STRUCTURE_HPP_ */
