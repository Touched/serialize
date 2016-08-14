#include "serialize/bitfield.hpp"

namespace serialize {
    BitfieldIterator::BitfieldIterator(BitfieldValue* value,
                                         const BitfieldIterator::iterator_adaptor_::base_type& it)
        : BitfieldIterator::iterator_adaptor_(it), bitfield_(value) {}

    const std::pair<std::string, uint32_t> BitfieldIterator::dereference() const {
        auto key = base()->first;
        auto index = base()->second;
        auto value = bitfield_->values_[index];

        return std::make_pair(key, value);
    }

    BitfieldValue::BitfieldValue(const Bitfield* schema, uint32_t value) :
        ScalarValue(schema), bitfield_(schema), values_(schema->masks_.size()), value_(value) {

        for (std::size_t i = 0; i < bitfield_->masks_.size(); ++i) {
            auto& mask = bitfield_->masks_[i];
            values_[i] = (value & mask.mask) >> mask.offset;
        }
    }

    BitfieldValue::~BitfieldValue() {}

    std::size_t BitfieldValue::size() const {
        return bitfield_->size_;
    }

    std::size_t BitfieldValue::getValue() const {
        return value_;
    }

    void BitfieldValue::setField(const std::string& key, uint32_t value) {
        auto index = bitfield_->keys_.at(key);
        auto& mask = bitfield_->masks_[index];

        if (value > mask.max) {
            throw std::runtime_error("Bitfield field value too large");
        }

        values_[index] = value;

        // Recalculate the numeric value
        value_ = (value_ & ~mask.mask) | (value << mask.offset);
    }

    uint32_t BitfieldValue::getField(const std::string& key) {
        auto index = bitfield_->keys_.at(key);
        return values_[index];
    }

    void BitfieldValue::accept(ValueVisitor& visitor) {
        visitor.visit(this);
    }

    BitfieldIterator BitfieldValue::begin() {
        return BitfieldIterator(this, bitfield_->keys_.cbegin());
    }

    BitfieldIterator BitfieldValue::end() {
        return BitfieldIterator(this, bitfield_->keys_.cend());
    }

    bool BitfieldValue::equals(const Value& other) const {
        const auto& bitfield = static_cast<const BitfieldValue&>(other);
        return value_ == bitfield.value_;
    }

    // Schema

    Bitfield::Bitfield() :
        size_(0), offset_(0) {}

    Bitfield::~Bitfield() {}

    std::size_t Bitfield::alignment() const {
        return alignment_;
    }

    Value* Bitfield::unpack(const Buffer& buffer,
                            std::size_t offset,
                            Context* context) const {
        if (offset + size_ > buffer.size()) {
            throw std::out_of_range("Buffer out of range");
        }

        uint32_t value(0);
        const uint8_t* data = buffer.begin() + offset;

        for (std::size_t i = 0; i < size_; i++) {
            value |= *data++ << (i * 8);
        }

        return new BitfieldValue(this, value);
    }

    void Bitfield::addField(const std::string& key, uint32_t width) {
        if (offset_ + width > sizeof(uint32_t) * 8) {
            throw std::runtime_error("Bitfield too large");
        }

        // Calculate bitmask
        uint32_t max = (1 << width) - 1;
        masks_.push_back({ max, max << offset_, offset_ });

        // Adjust offset for the next field
        offset_ += width;

        // Calculate size in bytes
        size_ = (offset_ + 7) / 8;

        if (size_ < 2) {
            // Byte alignment
            alignment_ = 0;
        } else if (size_ < 4) {
            // Half-word alignment
            alignment_ = 1;
        } else {
            // Word alignment
            alignment_ = 2;
        }

        // Add key for field
        std::size_t index = masks_.size() - 1;
        keys_.emplace(std::make_pair(key, index));
    }
}
