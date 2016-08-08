#include "serialize/util.hpp"

namespace serialize { namespace util {
        std::size_t align(std::size_t offset, std::size_t alignment) {
            // We can use bit hacks since alignment must be a power of 2
            alignment = (1 << alignment) - 1;
            return (offset + alignment) & ~alignment;
        }
    }
}
