#include <cstdint>
#include <utility>

namespace serialize {
    class Memory {
    public:
        enum class Region {
            bios = 0x0,
            invalid = 0x1,
            ewram = 0x2,
            iwram = 0x3,
            io = 0x4,
            palette = 0x5,
            vram = 0x6,
            oam = 0x7,
            rom = 0x8,
            rom1 = 0xA,
            rom2 = 0xC,
            sram = 0xE,
            max
        };

        static inline Region addressRegion(uint32_t address) {
            Region region;

            switch (address >> 24) {
            case 0:
                region = Region::bios;
                break;
            case 2:
                region = Region::ewram;
                break;
            case 3:
                region = Region::iwram;
                break;
            case 4:
                region = Region::io;
                break;
            case 5:
                region = Region::palette;
                break;
            case 6:
                region = Region::vram;
                break;
            case 7:
                region = Region::oam;
                break;
            case 8:
            case 9:
                region = Region::rom;
                break;
            case 0xA:
            case 0xB:
                region = Region::rom1;
                break;
            case 0xC:
            case 0xD:
                region = Region::rom2;
                break;
            case 0xE:
                region = Region::sram;
                break;
            default:
                return Region::invalid;
            }

            const std::pair<uint32_t, uint32_t>& range = regionRange(region);
            if (address >= range.first && address <= range.second) {
                return region;
            } else {
                return Region::invalid;
            }
        }

        static inline const std::pair<uint32_t, uint32_t> regionRange(Region region) {
            switch (region) {
            case Region::bios:
                return { 0x00000000, 0x00003FFF };
            case Region::ewram:
                return { 0x02000000, 0x0203FFFF };
            case Region::iwram:
                return { 0x03000000, 0x03007FFF };
            case Region::io:
                return { 0x04000000, 0x040003FE };
            case Region::palette:
                return { 0x05000000, 0x050003FF };
            case Region::vram:
                return { 0x06000000, 0x06017FFF };
            case Region::oam:
                return { 0x07000000, 0x070003FF };
            case Region::rom:
                return { 0x08000000, 0x09FFFFFF };
            case Region::rom1:
                return { 0x0A000000, 0x0BFFFFFF };
            case Region::rom2:
                return { 0x0C000000, 0x0DFFFFFF };
            case Region::sram:
                return { 0x0E000000, 0x0E00FFFF };
            default:
                return {};
            }
        }

        static inline bool isRom(Region region) {
            return region == Region::rom || region == Region::rom1 || region == Region::rom2;
        }
    };
}
