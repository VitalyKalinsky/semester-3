#include <cstddef>
#include <iostream>
#include <stdexcept>

class StackOverflowException : public std::runtime_error
{
public:
    StackOverflowException() : std::runtime_error("Stack overflow: Stack is full!") {};
};

class StackUnderflowException : public std::runtime_error
{
public:
    StackUnderflowException() : std::runtime_error("Stack underflow: Stack is empty!") {};
};

class StackOutOfRangeException : public std::runtime_error
{
public:
    StackOutOfRangeException() : std::runtime_error("Invalid index: Out of Range!") {};
};

template <class T, size_t max_size>
class MyStack
{
    size_t top_index;
    T data[max_size];
    // Обязательными операциями со стеком являются "Push" и "Pop","GetSize" и "Capacity"
    // Необязательной - может быть выбор по индексу (operator[]).
    // Для того, чтобы гарантировать корректное выполнение этих операций
    // следует генерировать исключительные ситуации.
public:
    MyStack() : top_index(0) {}

    void push(T element)
    {
        if (top_index >= max_size)
        {
            throw StackOverflowException();
        }

        data[top_index++] = element;
    }

    T pop()
    {
        if (top_index <= 0)
        {
            throw StackUnderflowException();
        }

        return data[--top_index];
    }

    size_t get_size() const
    {
        return top_index;
    }
    size_t get_capacity() const
    {
        return max_size;
    }

    T &operator[](size_t index)
    {
        if (index >= top_index)
        {
            throw StackOutOfRangeException();
        }
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        if (index >= top_index)
        {
            throw StackOutOfRangeException();
        }
        return data[index];
    }
};