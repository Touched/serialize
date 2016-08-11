#include <algorithm>
#include "serialize/structure.hpp"
#include "serialize/util.hpp"

namespace serialize {
    StructureIterator::StructureIterator(StructureValue* value,
                                         const StructureIterator::iterator_adaptor_::base_type& it)
        : StructureIterator::iterator_adaptor_(it), structure_(value) {}

    const std::pair<const std::string&, Value*> StructureIterator::dereference() const {
        const std::string& key = base()->first;
        auto value = structure_->values_[base()->second];

        return std::make_pair(key, value);
    }

    StructureValue::StructureValue(const Structure* schema)
        : CompositeValue(schema),
          structure_(schema),
          values_(structure_->schemas_.size(), nullptr) {}

    StructureValue::~StructureValue() {
        for (auto& value : values_) {
            delete value;
        }
    }

    std::size_t StructureValue::size() const {
        std::size_t size(0);

        for (auto& value : values_) {
            size += value->size();
        }

        return size;
    }

    void StructureValue::setField(const std::string& key, Value* value) {
        auto index = structure_->keys_.at(key);
        values_[index] = value;
    }

    Value* StructureValue::getField(const std::string& key) {
        auto index = structure_->keys_.at(key);
        return values_[index];
    }

    StructureIterator StructureValue::begin() {
        return StructureIterator(this, structure_->keys_.cbegin());
    }

    StructureIterator StructureValue::end() {
        return StructureIterator(this, structure_->keys_.cend());
    }

    bool StructureValue::equals(const Value& other) const {
        const auto& structure = static_cast<const StructureValue&>(other);

        return std::equal(values_.begin(),
                          values_.end(),
                          structure.values_.begin(),
                          Value::compare);
    }

    Structure::Structure() {}

    Structure::~Structure() {
        for (auto& schema : schemas_) {
            delete schema;
        }
    }

    std::size_t Structure::alignment() const {
        return alignment_;
    }

    Value* Structure::unpack(const Buffer& buffer,
                             std::size_t offset,
                             Context* context) const {
        auto structure = new StructureValue(this);
        auto ret = dynamic_cast<Value*>(structure);

        auto parent = context->getParent();
        context->setParent(ret);

        for (std::size_t i = 0 ; i < schemas_.size(); ++i) {
            auto& schema = schemas_[i];

            offset = util::align(offset, schema->alignment());

            auto value = schema->unpack(buffer, offset, context);

            // Save the field
            structure->values_[i] = value;
            offset += value->size();
        }

        // Restore parent
        context->setParent(parent);

        return ret;
    }

    void Structure::addField(const std::string& key, Schema* valueSchema) {
        schemas_.push_back(valueSchema);
        std::size_t index = schemas_.size() - 1;
        keys_.emplace(std::make_pair(key, index));
        calculateAlignment();
    }

    void Structure::calculateAlignment() {
        auto max = *std::max_element(schemas_.begin(),
                                     schemas_.end(),
                                     [](const Schema* a, const Schema* b) {
                                         return a->alignment() < b->alignment();
                                     });

        alignment_ = max->alignment();
    }
}
