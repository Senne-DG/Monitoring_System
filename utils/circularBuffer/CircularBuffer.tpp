#ifndef CIRCULAR_BUFFER_TPP
#define CIRCULAR_BUFFER_TPP

#include "CircularBuffer.hpp"
#include <algorithm>
#include <stdexcept>

/*
 * Default Constructor.
 * --------------------
 * Initializes the circular buffer by setting both the input (in_ptr) and output (out_ptr)
 * pointers to the start of the internal array, and marks the buffer as not full.
 */
template<typename T, std::size_t N>
CircularBuffer<T, N>::CircularBuffer()
    : in_ptr(buffer), out_ptr(buffer), full_flag(false)
{}

/*
 * Copy Constructor.
 * -----------------
 * Creates a new circularBuffer as an exact copy of the provided buffer, including its
 * internal state (pointers and full_flag) and the stored elements.
 *
 * @param other The circularBuffer instance to copy from.
 */
template<typename T, std::size_t N>
CircularBuffer<T, N>::CircularBuffer(const CircularBuffer& other)
    : in_ptr(buffer + (other.in_ptr - other.buffer)),
      out_ptr(buffer + (other.out_ptr - other.buffer)),
      full_flag(other.full_flag)
{
    std::copy(other.buffer, other.buffer + N, buffer);
}

/*
 * Assignment Operator.
 * --------------------
 * Copies the contents and internal state (pointer positions and full_flag) from another
 * circularBuffer into this instance.
 *
 * @param other The circularBuffer instance to assign from.
 * @return A reference to this circularBuffer.
 */
template<typename T, std::size_t N>
CircularBuffer<T, N>& CircularBuffer<T, N>::operator=(const CircularBuffer& other)
{
    if (this != &other)
    {
        std::copy(other.buffer, other.buffer + N, buffer);
        in_ptr = buffer + (other.in_ptr - other.buffer);
        out_ptr = buffer + (other.out_ptr - other.buffer);
        full_flag = other.full_flag;
    }
    return *this;
}

/*
 * capacity.
 * ---------
 * Returns the maximum number of elements that can be stored in the buffer.
 *
 * @return The constant capacity N.
 */
template<typename T, std::size_t N>
typename CircularBuffer<T, N>::size_type CircularBuffer<T, N>::capacity() const
{
    return N;
}

/*
 * empty.
 * ------
 * Checks if the buffer is empty. The buffer is considered empty if the full_flag is false
 * and both the input and output pointers are equal.
 *
 * @return true if the buffer is empty; false otherwise.
 */
template<typename T, std::size_t N>
bool CircularBuffer<T, N>::empty() const
{
    return (!full_flag && (in_ptr == out_ptr));
}

/*
 * full.
 * -----
 * Checks if the buffer is full.
 *
 * @return true if the buffer is full; false otherwise.
 */
template<typename T, std::size_t N>
bool CircularBuffer<T, N>::full() const
{
    return full_flag;
}

/*
 * size.
 * -----
 * Returns the current number of elements stored in the buffer. If the buffer is full,
 * it returns the capacity; otherwise, it computes the size using pointer arithmetic.
 *
 * @return The number of elements in the buffer.
 */
template<typename T, std::size_t N>
typename CircularBuffer<T, N>::size_type CircularBuffer<T, N>::size() const
{
    if (full_flag)
        return N;
    return (in_ptr >= out_ptr) ? (in_ptr - out_ptr) : (N - (out_ptr - in_ptr));
}

/*
 * clear.
 * ------
 * Resets the buffer to an empty state by setting both pointers to the start of the array
 * and clearing the full_flag.
 */
template<typename T, std::size_t N>
void CircularBuffer<T, N>::clear()
{
    in_ptr = buffer;
    out_ptr = buffer;
    full_flag = false;
}

/*
 * in.
 * ---
 * Inserts a new element into the buffer. If the buffer is already full, the oldest element
 * is overwritten by advancing the output pointer.
 *
 * @param value The element to insert.
 */
template<typename T, std::size_t N>
void CircularBuffer<T, N>::in(const value_type& value)
{
    // Write the value at the current in_ptr.
    *in_ptr = value;

    // Advance in_ptr, wrapping to the beginning if necessary.
    ++in_ptr;
    if (in_ptr == buffer + N)
        in_ptr = buffer;

    // If the buffer is full, also advance out_ptr to overwrite the oldest element.
    if (full_flag)
    {
        ++out_ptr;
        if (out_ptr == buffer + N)
            out_ptr = buffer;
    }

    // Update full_flag: the buffer is full if in_ptr catches up with out_ptr.
    full_flag = (in_ptr == out_ptr);
}

/*
 * out.
 * ----
 * Removes and returns the oldest element from the buffer. If the buffer is empty, a runtime
 * error is thrown.
 *
 * @return The element removed from the buffer.
 * @throws std::runtime_error if the buffer is empty.
 */
template<typename T, std::size_t N>
typename CircularBuffer<T, N>::value_type CircularBuffer<T, N>::out()
{
    if (empty())
        throw std::runtime_error("Circular buffer underflow");

    value_type value = *out_ptr;
    ++out_ptr;
    if (out_ptr == buffer + N)
        out_ptr = buffer;
    full_flag = false; // Removing an element always makes the buffer not full.
    return value;
}

/*
 * front.
 * -------
 * Provides access to the oldest element in the buffer without removing it.
 *
 * @return A reference to the oldest element.
 */
template<typename T, std::size_t N>
typename CircularBuffer<T, N>::reference CircularBuffer<T, N>::front()
{
    return *out_ptr;
}

template<typename T, std::size_t N>
typename CircularBuffer<T, N>::const_reference CircularBuffer<T, N>::front() const
{
    return *out_ptr;
}

/*
 * back.
 * ------
 * Provides access to the most recently inserted element without removing it.
 * Handles the wrap-around condition by checking if in_ptr is at the beginning.
 *
 * @return A reference to the most recent element.
 */
template<typename T, std::size_t N>
typename CircularBuffer<T, N>::reference CircularBuffer<T, N>::back()
{
    return (in_ptr == buffer) ? *(buffer + N - 1) : *(in_ptr - 1);
}

template<typename T, std::size_t N>
typename CircularBuffer<T, N>::const_reference CircularBuffer<T, N>::back() const
{
    return (in_ptr == buffer) ? *(buffer + N - 1) : *(in_ptr - 1);
}

#endif // CIRCULAR_BUFFER_TPP
