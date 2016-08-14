#include <limits>
#include "serialize/string.hpp"

enum class PokecodeSpecial : uint8_t {
    codepage1 = 0xF8,
    codepage2 = 0xF9,
    scroll = 0xFA,
    page = 0xFB,
    control = 0xFC,
    interpolate = 0xFD,
    linebreak = 0xFE,
    terminator = 0xFF,
};

static const std::string pokecode_mapping[][std::numeric_limits<uint8_t>::max()] = {
    {
        " ", "À", "Á", "Â", "Ç", "È", "É", "Ê", "Ë", "Ì", "こ",
        "Î", "Ï", "Ò", "Ó", "Ô", "Œ", "Ù", "Ú", "Û", "Ñ", "ß",
        "à", "á", "ね", "ç", "è", "é", "ê", "ë", "ì", "ま", "î",
        "ï", "ò", "ó", "ô", "œ", "ù", "ú", "û", "ñ", "º", "ª",
        "°", "&", "+", "あ", "ぃ", "ぅ", "ぇ", "ぉ", "{lv}", "=", "ょ",
        "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず", "ぜ", "ぞ", "だ",
        "ぢ", "づ", "で", "ど", "ば", "び", "ぶ", "べ", "ぼ", "ぱ", "ぴ",
        "ぷ", "ぺ", "ぽ", "っ", "¿", "¡", "{pk}", "{mn}", "{po}", "{ke}", "{bl}",
        "{oc}", "{k}", "Í", "%", "(", ")", "セ", "ソ", "タ", "チ", "ツ",
        "テ", "ト", "ナ", "ニ", "ヌ", "â", "ノ", "ハ", "ヒ", "フ", "ヘ",
        "ホ", "í", "ミ", "ム", "メ", "モ", "ヤ", "ユ", "ヨ", "ラ", "リ",
        "⬆", "⬇", "⬅", "➡", "ヲ", "ン", "ァ", "ィ", "ゥ", "ェ", "ォ",
        "ャ", "ュ", "ョ", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ",
        "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "バ", "ビ", "ブ", "ベ",
        "ボ", "パ", "ピ", "プ", "ペ", "ポ", "ッ", "0", "1", "2", "3",
        "4", "5", "6", "7", "8", "9", "!", "?", ".", "-", "・",
        "…", "“", "”", "‘", "’", "♂", "♀", "$", ",", "×", "/",
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K",
        "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V",
        "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r",
        "s", "t", "u", "v", "w", "x", "y", "z", "▶", ":", "Ä",
        "Ö", "Ü", "ä", "ö", "ü", "⬆", "⬇", "⬅", "\\l", "\\p",
    }, {
        "{key:a}", "{key:b}", "{key:l}", "{key:r}", "{key:start}",
        "{key:select}", "{key:up}", "{key:down}", "{key:left}", "{key:right}",
        "{key:}", "{key:updown}", "{key:leftright}", "{key:none}"
    }, {
        "{sym:up}", "{sym:down}", "{sym:left}", "{sym:right}",
        "{sym:plus}", "{sym:lv}", "{sym:ap}", "{sym:id}",
        "{sym:nr}", "{sym:ul}", "{sym:1}", "{sym:2}",
        "{sym:3}", "{sym:4}", "{sym:5}", "{sym:6}",
        "{sym:7}", "{sym:8}", "{sym:9}", "{sym:lbracket}",
        "{sym:rbracket}", "{sym:circle}", "{sym:triangle}", "{sym:x}",
    }
};

namespace serialize {
    StringValue::StringValue(const String* schema, std::size_t size, const std::string&& value)
        : BinaryValue(schema), size_(size), value_(value) {}

    StringValue::~StringValue() {}

    std::size_t StringValue::size() const {
        return size_;
    }

    const std::string& StringValue::string() const {
        return value_;
    }

    void StringValue::accept(ValueVisitor& visitor) {
        visitor.visit(this);
    }

    bool StringValue::equals(const Value& other) const {
        const auto& string = static_cast<const StringValue&>(other);
        return string.value_ == value_;
    }

    String::String() {}

    String::~String() {}

    std::size_t String::alignment() const {
        return 0;
    }

    void String::accept(SchemaVisitor& visitor) {
        visitor.visit(this);
    }

    Value* String::unpack(const Buffer& buffer, std::size_t offset, Context* context) const {
        std::string result;
        const uint8_t* it;

        for (it = buffer.begin();
             it < buffer.end() && PokecodeSpecial(*it) != PokecodeSpecial::terminator;
             ++it) {
            switch (PokecodeSpecial(*it)) {
            case PokecodeSpecial::codepage1:
                // GBA controls
                it++;
                result.append(pokecode_mapping[1][*it]);
                break;
            case PokecodeSpecial::codepage2:
                it++;
                result.append(pokecode_mapping[2][*it]);
                break;
            case PokecodeSpecial::scroll:
                result.append("\\l");
                break;
            case PokecodeSpecial::page:
                result.append("\\p");
                break;
            case PokecodeSpecial::control:
                break;
            case PokecodeSpecial::interpolate:
                break;
            case PokecodeSpecial::linebreak:
                result.push_back('\n');
                break;
            default:
                result.append(pokecode_mapping[0][*it]);
                break;
            }
        }

        return new StringValue(this, std::distance(buffer.begin(), it) + 1, std::move(result));
    }
}
