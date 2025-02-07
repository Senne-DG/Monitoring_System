#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <cstddef>

/**
 * @brief A fixed-size circular buffer that supports overwriting.
 *
 * This template class implements a circular (ring) buffer with a fixed capacity.
 * When the buffer is full and a new element is inserted, the oldest element is overwritten.
 *
 * @tparam T The type of elements stored in the buffer.
 * @tparam N The maximum number of elements the buffer can hold.
 */
template<typename T, std::size_t N>
class CircularBuffer
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    /*
     * @brief Default constructor.
     *
     * Initializes the circular buffer by setting both the input (in_ptr)
     * and output (out_ptr) pointers to the start of the internal array.
     * The buffer is initialized as empty.
     */
    CircularBuffer();

    /*
     * @brief Copy constructor.
     *
     * Constructs a new circularBuffer by copying the contents and state
     * (including pointer positions and full flag) from an existing buffer.
     *
     * @param other The circularBuffer to copy from.
     */
    CircularBuffer(const CircularBuffer& other);

    /*
     * @brief Assignment operator.
     *
     * Copies the contents and internal state from another circularBuffer.
     *
     * @param other The circularBuffer to copy from.
     * @return A reference to this circularBuffer.
     */
    CircularBuffer& operator=(const CircularBuffer& other);

    /*
     * @brief Returns the capacity of the buffer.
     *
     * @return The maximum number of elements that can be stored in the buffer.
     */
    size_type capacity() const;

    /*
     * @brief Checks whether the buffer is empty.
     *
     * @return true if the buffer is empty, false otherwise.
     */
    bool empty() const;

    /*
     * @brief Checks whether the buffer is full.
     *
     * @return true if the buffer is full, false otherwise.
     */
    bool full() const;

    /*
     * @brief Returns the current number of elements in the buffer.
     *
     * @return The number of elements currently stored.
     */
    size_type size() const;

    /*
     * @brief Clears the buffer.
     *
     * Resets the buffer to an empty state, resetting the pointers and full flag.
     */
    void clear();

    /*
     * @brief Inserts an element into the buffer.
     *
     * Inserts the given value at the current input position. If the buffer
     * is already full, the oldest element is overwritten.
     *
     * @param value The element to be inserted.
     */
    void in(const value_type& value);

    /*
     * @brief Removes and returns the oldest element from the buffer.
     *
     * Removes the element at the current output position and advances the output pointer.
     *
     * @return The element that was removed.
     * @throw std::runtime_error if the buffer is empty.
     */
    value_type out();

    /*
     * @brief Accesses the oldest element in the buffer.
     *
     * @return A reference to the oldest element (front) in the buffer.
     */
    reference front();

    /*
     * @brief Accesses the oldest element in the buffer (const version).
     *
     * @return A const reference to the oldest element (front) in the buffer.
     */
    const_reference front() const;

    /*
     * @brief Accesses the most recently inserted element in the buffer.
     *
     * @return A reference to the most recent element (back) in the buffer.
     */
    reference back();

    /*
     * @brief Accesses the most recently inserted element in the buffer (const version).
     *
     * @return A const reference to the most recent element (back) in the buffer.
     */
    const_reference back() const;

private:
    value_type buffer[N];  // Internal fixed-size array to store elements.
    pointer in_ptr;        // Pointer to the next insertion position.
    pointer out_ptr;       // Pointer to the oldest element (next element to be removed).
    bool full_flag;        // Flag to indicate if the buffer is full.
};

// Include the template implementation file.
#include "CircularBuffer.tpp"

#endif // CIRCULAR_BUFFER_HPP
