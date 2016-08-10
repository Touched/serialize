#include "serialize/virtual.hpp"

namespace serialize {
    VirtualValue::VirtualValue(const Virtual* schema, const ScalarValue* wrapped)
        : ScalarValue(schema), wrapped_(wrapped) {}

    VirtualValue::~VirtualValue() {
        delete wrapped_;
    }

    std::size_t VirtualValue::size() const {
        return wrapped_->size();
    }

    std::size_t VirtualValue::getValue() const {
        return wrapped_->getValue();
    }

    bool VirtualValue::equals(const Value& other) const {
        const auto& v = static_cast<const VirtualValue&>(other);
        return *v.wrapped_ == *wrapped_;
    }

    Virtual::Virtual(Scalar* wrapped) : wrapped_(wrapped) {}

    Virtual::~Virtual() {
        delete wrapped_;
    }

    std::size_t Virtual::alignment() const {
        return wrapped_->alignment();
    }

    Value* Virtual::unpack(const Buffer& buffer,
                           std::size_t offset,
                           Context* context) const {
        auto wrapped = wrapped_->unpack(buffer, offset, context);

        // Store the value in the context
        context->setValue(this, wrapped);

        return new VirtualValue(this, dynamic_cast<ScalarValue*>(wrapped));
    }
}
