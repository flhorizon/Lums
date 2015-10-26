/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    BObject.hpp                                    oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#ifndef LUMS_BOBJECT_HPP
#define LUMS_BOBJECT_HPP

#include <fstream>
#include <LumsInclude/Binary/BValue.hpp>
#include <LumsInclude/ExportDll.hpp>

namespace lm
{
    class BObject
    {
    public:
        class Iterator
        {
        public:
            Iterator(const BObject& object, int index) : _object(object), _index(index) {}
            Iterator        operator++() { Iterator i = *this; _index++; return i; }
            Iterator        operator++(int) { _index++; return *this; }
            Iterator        operator--() { Iterator i = *this; _index--; return i; }
            Iterator        operator--(int) { _index--; return *this; }
            const BPair&    operator*() { return _object._values[_index]; }
            const BPair&    operator->() { return _object._values[_index]; }
            bool            operator==(const Iterator& rhs) { return _index == rhs._index; }
            bool            operator!=(const Iterator& rhs) { return !(*this == rhs); }

        private:
            const BObject&  _object;
            int             _index;
        };

        LUMS_EXPORTED   BObject();
        LUMS_EXPORTED   BObject(const BObject& rhs);
        LUMS_EXPORTED   BObject&    operator=(const BObject& rhs);
        
        Iterator    begin() const { return Iterator(*this, 0); }
        Iterator    end() const { return Iterator(*this, _size); }
        LUMS_EXPORTED   void        parse(std::ifstream& file);
        
        bool
        hasProperty(const char* str) const
        {
            for (int i = 0; i < _size; ++i)
            {
                if (strcmp(_values[i].first, str) == 0)
                    return true;
            }
            return false;
        }

        LUMS_EXPORTED   const BValue& operator[](const char* str) const;
        LUMS_EXPORTED   ~BObject();

    private:
        int       _size;
        BPair*    _values;
    };
}

#endif
