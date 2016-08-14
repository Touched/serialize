#ifndef SERIALIZE_BITFIELD_HPP_
#define SERIALIZE_BITFIELD_HPP_

#include <unordered_map>
#include <vector>
#include <string>
#include <boost/iterator/iterator_adaptor.hpp>
#include "base.hpp"
#include "visitor.hpp"

namespace serialize {
    /**
     * Groups of bits packed into a single integer.
     */
    class Bitfield;

    class BitfieldIterator
        : public boost::iterator_adaptor<BitfieldIterator,
                                         std::unordered_map<std::string,
                                                            std::size_t>::const_iterator,
                                         const std::pair<std::string, uint32_t>,
                                         boost::forward_traversal_tag,
                                         const std::pair<std::string, uint32_t>
                                         > {
    public:
        BitfieldIterator(BitfieldValue* value,
                          const BitfieldIterator::iterator_adaptor_::base_type& it);
        const std::pair<std::string, uint32_t> dereference() const;
    private:
        friend class boost::iterator_core_access;
        BitfieldValue* bitfield_;
    };

    class BitfieldValue : public ScalarValue {
    public:
        BitfieldValue(const Bitfield* schema, uint32_t value);

        virtual ~BitfieldValue();

        virtual std::size_t size() const;

        virtual std::size_t getValue() const;

        void setField(const std::string& key, uint32_t value);

        uint32_t getField(const std::string& key);

        virtual void accept(ValueVisitor& visitor);

        BitfieldIterator begin();

        BitfieldIterator end();

    protected:
        friend BitfieldIterator;

        virtual bool equals(const Value& other) const;

        const Bitfield* bitfield_;
        std::vector<uint32_t> values_;
        uint32_t value_;
    };

    class Bitfield : public Scalar {
    public:
        Bitfield();

        virtual ~Bitfield();

        virtual std::size_t alignment() const;

        virtual void accept(SchemaVisitor& visitor);

	virtual Value* unpack(const Buffer& buffer,
                              std::size_t offset,
                              Context* context=new Context()) const;

        void addField(const std::string& key, uint32_t width);

    protected:
        friend BitfieldValue;

        struct Mask {
            uint32_t max;
            uint32_t mask;
            uint32_t offset;
        };

        std::unordered_map<std::string, std::size_t> keys_;
        std::vector<Mask> masks_;
        std::size_t alignment_;
        uint32_t size_;
        uint32_t offset_;
    };
}

#endif /* SERIALIZE_BITFIELD_HPP_ */
