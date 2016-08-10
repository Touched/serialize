#include "serialize/list.hpp"
#include "serialize/util.hpp"

namespace serialize {
    ListValue::ListValue(const List* schema)
        : CompositeValue(schema), list_(schema) {}

    ListValue::~ListValue() {
        for (auto& element : values_) {
            delete element;
        }
    }

    std::size_t ListValue::size() const {
        // Account for sentinel
        std::size_t size(list_->sentinel_->size());

        if (schema_->dynamic()) {
            for (const auto& element : values_) {
                size += element->size();
            }
        } else {
            size = values_.size() * values_.front()->size();
        }

        return size;
    }

    void ListValue::pushElement(Value* value) {
        values_.push_back(value);
    }

    ListValue::iterator ListValue::begin() {
        return values_.begin();
    }

    ListValue::iterator ListValue::end() {
        return values_.end();
    }

    bool ListValue::equals(const Value& other) const {
        const auto& list = static_cast<const ListValue&>(other);
        return std::equal(values_.begin(),
                          values_.end(),
                          list.values_.begin(),
                          Value::compare);
    }

    List::List(Schema* element, Value* sentinel)
        : element_(element), sentinel_(sentinel) {}

    List::~List() {
        delete element_;
        delete sentinel_;
    }

    std::size_t List::alignment() const {
        return element_->alignment();
    }

    Value* List::unpack(const Buffer& buffer,
                         std::size_t offset,
                         Context* context) const {
        // Build the list value type
        auto list = new ListValue(this);
        auto ret = dynamic_cast<Value*>(list);

        auto parent = context->getParent();
        context->setParent(ret);

        // Align the offset once-off (every element has equal alignment)
        offset = util::align(offset, alignment());

        // Populate the list
        while (true) {
            auto value = element_->unpack(buffer, offset, context);

            if (*value == *sentinel_) {
                break;
            }

            // Save the field
            list->pushElement(value);
            offset += value->size();
        }

        // Restore parent
        context->setParent(parent);

        return ret;
    }
}
