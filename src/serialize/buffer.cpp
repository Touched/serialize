#include "serialize/buffer.hpp"

namespace serialize {
    Buffer::Buffer(const uint8_t* begin, const uint8_t* end)
	: begin_(begin), end_(end), size_(end_ - begin_) {}
}
