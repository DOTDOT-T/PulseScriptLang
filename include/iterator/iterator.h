#ifndef PULSE_STL_ITERATOR_H
#define PULSE_STL_ITERATOR_H

namespace PulseLib::STL
{
    /**
     * @brief Simple iterator structure to use with all PSTL containers
     * 
     * @tparam Object Type of object the iterator refers to
     */
    template<typename Object>
    struct Iterator
    {
        using reference = Object&;
        using pointer   = Object*;

        pointer ptr;

        reference operator*() const noexcept { return *ptr; }
        pointer   operator->() const noexcept { return ptr; }

        Iterator& operator++() noexcept { ++ptr; return *this; }     // prefix
        Iterator  operator++(int) noexcept { Iterator tmp = *this; ++ptr; return tmp; } // postfix

        Iterator& operator--() noexcept { --ptr; return *this; }     // prefix decrement
        Iterator  operator--(int) noexcept { Iterator tmp = *this; --ptr; return tmp; } // postfix decrement

        bool operator==(const Iterator& other) const noexcept { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const noexcept { return ptr != other.ptr; }
    };

    /**
     * @brief Reverse iterator that works in the opposite direction of a normal iterator
     * 
     * @tparam Object Type of object the iterator refers to
     */
    template<typename Object>
    struct ReverseIterator
    {
        using reference = Object&;
        using pointer   = Object*;

        Iterator<Object> base; // base iterator pointing after the element

        reference operator*() const noexcept { Iterator<Object> tmp = base; --tmp.ptr; return *tmp; }

        pointer operator->() const noexcept { return &(**this); }

        ReverseIterator& operator++() noexcept { --base.ptr; return *this; }      // prefix
        ReverseIterator  operator++(int) noexcept { ReverseIterator tmp = *this; --base.ptr; return tmp; } // postfix

        ReverseIterator& operator--() noexcept { ++base.ptr; return *this; }      // prefix decrement
        ReverseIterator  operator--(int) noexcept { ReverseIterator tmp = *this; ++base.ptr; return tmp; } // postfix decrement

        bool operator==(const ReverseIterator& other) const noexcept { return base.operator==(other.base); }
        bool operator!=(const ReverseIterator& other) const noexcept { return base.operator!=(other.base); }
    };
};

#endif
