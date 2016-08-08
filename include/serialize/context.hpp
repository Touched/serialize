#ifndef SERIALIZE_CONTEXT_HPP_
#define SERIALIZE_CONTEXT_HPP_

#include <unordered_map>

namespace serialize {
    class Schema;
    class Value;
    class Virtual;

    /**
     * Deserialization context
     */
    class Context
    {
    public:
        Context();

        const Value* getParent() const;
        void setParent(const Value*);

        const Value* getValue(const Schema*) const;
        void setValue(const Schema*, const Value*);


        virtual ~Context();

    private:
        const Value* parent_;
        std::unordered_map<const Schema*, const Value*> values_;
    };
}

#endif /* SERIALIZE_CONTEXT_HPP_ */
