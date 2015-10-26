/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    BValue.hpp                                     oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_BVALUE_HPP
#define LUMS_BVALUE_HPP

#include <fstream>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    class BArray;
    class BObject;

    enum class BType
    {
        None,
        Object,
        Array,
        Int,
        Float,
        String
    };

    using BInt = int;
    using BFloat = float;
    using BString = char*;

    class BValue
    {
    public:
        LUMS_EXPORTED BValue();
        LUMS_EXPORTED BValue(const BValue& rhs);
        LUMS_EXPORTED BValue& operator=(const BValue& rhs);

        LUMS_EXPORTED void    parse(std::ifstream& file);
        LUMS_EXPORTED bool    is(BType type) const { return type == _type; }
        bool    isObject() const { return is(BType::Object); }
        bool    isArray() const { return is(BType::Array); }
        bool    isInt() const { return is(BType::Int); }
        bool    isFloat() const { return is(BType::Float); }
        bool    isString() const { return is(BType::String); }

        LUMS_EXPORTED BInt            asInt() const;
        LUMS_EXPORTED BFloat          asFloat() const;
        LUMS_EXPORTED const BObject&  asObject() const;
        LUMS_EXPORTED const BString   asString() const;
        LUMS_EXPORTED const BArray&   asArray() const;

        LUMS_EXPORTED const BValue&   operator[](int i) const;
        LUMS_EXPORTED const BValue&   operator[](const char* str) const;

        LUMS_EXPORTED ~BValue();

    private:
        BType           _type;
        union
        {
            BObject*    _object;
            BArray*     _array;
            BInt        _int;
            BFloat      _float;
            BString     _string;
        };
    };

    using BPair = std::pair<char*, BValue>;
}

#endif
