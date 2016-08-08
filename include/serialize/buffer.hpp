#ifndef SERIALIZE_BUFFER_HPP_
#define SERIALIZE_BUFFER_HPP_

#include <cstdint>

namespace serialize {
    class Buffer {
    public:
        Buffer(const uint8_t* begin, const uint8_t* end);

        inline std::size_t size() const {
            return size_;
        }

        inline const uint8_t* begin() const {
            return begin_;
        }

        inline const uint8_t* end() const {
            return end_;
        }
    private:
	const uint8_t* begin_;
	const uint8_t* end_;
        std::size_t size_;
    };
}

#endif /* SERIALIZE_BUFFER_HPP_ */
