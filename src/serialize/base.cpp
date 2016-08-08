#include "serialize/base.hpp"

namespace serialize {
    Schema::Value::Value(const Schema* schema) : schema_(schema) {}

    Leaf::Leaf() {}
    Leaf::~Leaf() {}
    Leaf::Value::Value(const Schema* schema) : Schema::Value(schema) {}
    Leaf::Value::~Value() {}

    std::size_t Schema::alignment() const {
	return 1;
    }
}
