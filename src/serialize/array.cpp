#include "serialize/array.hpp"
#include "serialize/util.hpp"

namespace serialize {
    ArrayValue::ArrayValue(const Array* schema, std::size_t length)
        : CompositeValue(schema),
          values_(length) {}

    ArrayValue::~ArrayValue() {
        for (auto& element : values_) {
            delete element;
        }
    }

    std::size_t ArrayValue::size() const {
        std::size_t size(0);

        if (schema_->dynamic()) {
            for (const auto& element : values_) {
                size += element->size();
            }
        } else {
            size = values_.size() * values_.front()->size();
        }

        return size;
    }

    void ArrayValue::setElement(std::size_t index, Value* value) {
        values_[index] = value;
    }

    Value* ArrayValue::getElement(std::size_t index) {
        return values_[index];
    }

    ArrayValue::iterator ArrayValue::begin() {
        return values_.begin();
    }

    ArrayValue::iterator ArrayValue::end() {
        return values_.end();
    }

    bool ArrayValue::equals(const Value& other) const {
        const auto& array = static_cast<const ArrayValue&>(other);
        return std::equal(values_.begin(),
                          values_.end(),
                          array.values_.begin(),
                          Value::compare);
    }

    Array::Array(Schema* element, Virtual* length)
        : element_(element), length_(length) {}

    Array::~Array() {
        delete element_;
    }

    std::size_t Array::alignment() const {
        return element_->alignment();
    }

    Value* Array::unpack(const Buffer& buffer,
                         std::size_t offset,
                         Context* context) const {
        // Get the size from the deserialization context
        auto length = dynamic_cast<const ScalarValue*>(context->getValue(length_))->getValue();

        // Build the array value type
        auto array = new ArrayValue(this, length);
        auto ret = dynamic_cast<Value*>(array);

        auto parent = context->getParent();
        context->setParent(ret);

        // Align the offset once-off (every element has equal alignment)
        offset = util::align(offset, alignment());

        // Populate the array
        for (std::size_t i = 0; i < length; ++i) {

            auto value = element_->unpack(buffer, offset, context);

            // Save the field
            array->setElement(i, value);
            offset += value->size();
        }

        // Restore parent
        context->setParent(parent);

        return ret;
    }
}
