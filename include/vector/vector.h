#ifndef PULSESTL_VECTOR_H
#define PULSESTL_VECTOR_H

#include <stdexcept>

#include "iterator/iterator.h"

namespace PulseLib::STL
{
    template<typename Object>
    class Vector
    {
    public:
        Vector()
        {
            item = new Object[capacity];
            size = 0;
        }

        ~Vector()
        {
            delete[] item;
        }

#pragma region MEMORY MANIPUALTION

        void Reserve(int reserveSize)
        {
            if (reserveSize <= capacity) return;

            Object* newBlock = new Object[reserveSize];

            // By using a constexpr, we are telling at the compile time of the template which one to use
            // -> gain time of compilation
            // -> no need to evaluate at each call at runtime
            // -> choose the right re-assign method
            // objectif here is to easily reserve without costing a lot at runtime
            if constexpr (std::is_trivially_copyable_v<Object>) 
            {
                std::memcpy(newBlock, item, size * sizeof(Object)); //copy memory only if its trivially copyable
            } 
            else 
            {
                for (int i = 0; i < size; ++i)
                    newBlock[i] = std::move(item[i]); //move memory if the Object type isnt easy to copy
            }
            

            delete[] item;
            item = newBlock;
            capacity = reserveSize;
        }

        void Clear()
        {
            delete[] item;
            size = 0;
        }

#pragma endregion

#pragma region INSERTION AND DELETE

        void Pushback(const Object& obj)  //for now, the pushback will only insert to the last position, next step -> return the iterator
        {
            Resize();

            item[size] = obj;
            ++size;
        }

        void Insert(Object item, int index)
        {
            Resize();
        }

#pragma endregion

#pragma region UTILITIES

        /**
         * @brief Resize will only resize the vector if we got at the limit of the vector.
         * 
         */
        void Resize()
        {
            if (size >= capacity)
            {
                // simple reallocation: double capacity
                int newCapacity = capacity * 2;
                Reserve(newCapacity);
            }
        }

#pragma endregion

#pragma region ACCESSOR

        /**
         * @brief will return a reference to the last item of the vector. To get an iterator : @see Last()
         * 
         */
        Object& Back()
        {
            if (size == 0)
                throw std::out_of_range("Vector is empty");
        
            return item[size - 1];
        }
    
        Iterator<Object> Last()
        {
            if (size == 0)
                throw std::out_of_range("Vector is empty");
        
            return Iterator<Object>{ item + (size - 1) };
        }

        Iterator<Object> Begin() { return Iterator<Object>{ item }; }
        Iterator<Object> End()   { return Iterator<Object>{ item + size}; }
        ReverseIterator<Object> rBegin() { return ReverseIterator<Object>{ item + size }; }
        ReverseIterator<Object> rEnd()   { return ReverseIterator<Object>{ item }; }

        Object& operator[](int index)
        {
            return item[index];
        }

        /**
         * @brief The size is the amount of "block" of size of Object that has been used and assign with data in the total capacity of the vector.
         * 
         */
        int Size() const { return size; }
        /**
         * @brief the capacity indicate the amount of Object that can be pushed in the vector before needing to resize the vector to a higher capacity.
         * 
         * @return int 
         */
        int Capacity() const { return capacity; }        

#pragma endregion

    private:    
        Object* item;           // pointer to the first object
        int     size = 0;       // size give us the amount of object in the vector
        int     capacity = 10;  //the capacity indicate the total amount of value possible to insert in the vector before needing to reallocate it
    };
};

#endif