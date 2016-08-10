#include "serialize/base.hpp"

namespace serialize {
    Schema::~Schema() {}
    std::size_t Schema::alignment() const { return 1; }

    Value::~Value() {}
    Value::Value(const Schema* schema) : schema_(schema) {}

    Scalar::Scalar() {}
    Scalar::~Scalar() {}
    bool Scalar::dynamic() const {
        return false;
    }

    ScalarValue::ScalarValue(const Scalar* schema) : Value(schema) {}
    ScalarValue::~ScalarValue() {}

    Composite::Composite() {}
    Composite::~Composite() {}
    bool Composite::dynamic() const {
        return true;
    }

    CompositeValue::CompositeValue(const Composite* schema) : Value(schema) {}
    CompositeValue::~CompositeValue() {}
}
