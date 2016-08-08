#include "serialize/context.hpp"

namespace serialize {
    Context::Context()
        : parent_(nullptr) {}

    Context::~Context() {}

    const Value* Context::getParent() const {
        return parent_;
    }

    void Context::setParent(const Value* parent) {
        parent_ = parent;
    }

    const Value* Context::getValue(const Schema* schema) const {
        return values_.at(schema);
    }

    void Context::setValue(const Schema* schema, const Value* value) {
        auto result = values_.emplace(std::make_pair(schema, value));

        if (result.second == false) {
            throw std::runtime_error("Duplicate schema");
        }
    }
}
