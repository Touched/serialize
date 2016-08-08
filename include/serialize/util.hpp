#ifndef SERIALIZE_UTIL_HPP_
#define SERIALIZE_UTIL_HPP_

#include <cstdint>

namespace serialize { namespace util {
        std::size_t align(std::size_t offset, std::size_t alignment);
    }
}

#endif /* SERIALIZE_UTIL_HPP_ */
