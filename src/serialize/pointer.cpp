#include "serialize/memory.hpp"
#include "serialize/pointer.hpp"

namespace serialize {
    PointerValue::PointerValue(const Pointer* schema, uint32_t address, Value* wrapped)
        : WordValue(schema, address), wrapped_(wrapped) {}

    PointerValue::~PointerValue() {
        delete wrapped_;
    }

    Value* PointerValue::dereference() const {
        return wrapped_;
    }

    bool PointerValue::equals(const Value& other) const {
        const auto& pointer = static_cast<const PointerValue&>(other);
        return *pointer.wrapped_ == *wrapped_;
    }

    Pointer::Pointer(Schema* wrapped)
        : wrapped_(wrapped) {}

    Pointer::~Pointer() {
        delete wrapped_;
    }

    void Pointer::accept(SchemaVisitor& visitor) {
        visitor.visit(this);
    }

    Value* Pointer::unpack(const Buffer& buffer,
                           std::size_t offset,
                           Context* context) const {
        if (offset + sizeof(uint32_t) > buffer.size()) {
            throw std::out_of_range("Buffer out of range");
        }

        uint32_t address(0);
        const uint8_t* data = buffer.begin() + offset;

        for (std::size_t i = 0; i < sizeof(uint32_t); i++) {
            address |= *data++ << (i * 8);
        }

        if (address == 0) {
            return new PointerValue(this, 0, nullptr);
        }

        Memory::Region region = Memory::addressRegion(address);
        if (Memory::isRom(region)) {
            uint32_t pointerAsOffset = address - Memory::regionRange(region).first;

            if (pointerAsOffset > buffer.size()) {
                throw std::out_of_range("Cannot follow pointer: Buffer out of range");
            }

            Value* value = wrapped_->unpack(buffer, pointerAsOffset, context);
            return new PointerValue(this, address, value);
        } else {
            throw std::runtime_error("Address not in the ROM");
        }
    }
}
