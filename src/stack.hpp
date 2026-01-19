#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>

template <typename T>
class Stack
{
public:
    Stack() = default;
    Stack(std::size_t nelems);
    Stack(const Stack&);
    Stack& operator=(const Stack&);
    Stack(Stack&&);
    Stack& operator=(Stack&&);
    ~Stack();

    void push(const T&);
    void push(T&&);

    T top() const;
    void pop();

    std::size_t count() const noexcept;
    std::size_t size() const noexcept;
    bool is_empty() const noexcept;

private:
    std::size_t growth_factor() { return 2; }
    void steal_data(Stack&&);

    std::size_t m_top{};
    std::size_t m_nelems{};
    T* m_data{};
};

template <typename T>
inline Stack<T>::Stack(std::size_t nelems) :
    m_top{0},
    m_nelems{nelems == 0 ? 1 : nelems}
{
    m_data = new T[m_nelems];
}

template <typename T>
inline Stack<T>::Stack(const Stack& other) :
    m_top{other.m_top},
    m_nelems{other.m_nelems}
{
    m_data = nullptr;
    try
    {
        m_data = new T[other.m_nelems];
        for (std::size_t i = 0; i < m_nelems; ++i)
            m_data[i] = other.m_data[i];
    }
    catch (...)
    {
        delete[] m_data;
        throw;
    }
}

template <typename T>
inline Stack<T>& Stack<T>::operator=(const Stack& other)
{
    if (this == &other)
        return *this;

    T* try_data = nullptr;

    try
    {
        try_data = new T[other.m_nelems];
        for (std::size_t i = 0; i < other.m_nelems; ++i)
        {
            try_data[i] = other.m_data[i];
        }
    }
    catch (...)
    {
        delete[] try_data;
        throw;
    }

    if (m_data != nullptr)
        delete[] m_data;

    m_data = try_data;
    m_nelems = other.m_nelems;
    m_top = other.m_top;

    return *this;
}

template <typename T>
inline Stack<T>::Stack(Stack&& other)
{
    steal_data(std::move(other));
}

template <typename T>
inline Stack<T>& Stack<T>::operator=(Stack&& other)
{
    if (this == &other)
        return *this;

    if (m_data != nullptr)
        delete[] m_data;

    steal_data(std::move(other));

    return *this;
}

template <typename T>
inline Stack<T>::~Stack()
{
    delete[] m_data;
}

template <typename T>
inline void Stack<T>::push(const T& element)
{
    push(T{element});
}

template <typename T>
inline void Stack<T>::push(T&& element)
{
    if (m_top == m_nelems)
    {
        auto new_nelems = m_nelems == 0 ? 1 : m_nelems * growth_factor();
        T* new_buffer = nullptr;

        try
        {
            new_buffer = new T[new_nelems];
            for (std::size_t i = 0; i < m_nelems; ++i)
                new_buffer[i] = std::move(m_data[i]);
        }
        catch (...)
        {
            delete[] new_buffer;
            throw;
        }

        delete[] m_data;
        m_data = new_buffer;
        m_nelems = new_nelems;
    }

    auto temp_top = m_top;
    m_data[temp_top] = std::move(element);
    ++m_top;
}

template <typename T>
inline T Stack<T>::top() const
{
    if (is_empty())
        throw std::runtime_error("cannot top empty stack");

    return m_data[m_top - 1];
}

template <typename T>
inline void Stack<T>::pop()
{
    if (m_top > 0)
        --m_top;
    else
        throw std::runtime_error("cannot pop empty stack");
}

template <typename T>
inline std::size_t Stack<T>::count() const noexcept
{
    return m_top;
}

template <typename T>
inline std::size_t Stack<T>::size() const noexcept
{
    return m_nelems;
}

template <typename T>
inline bool Stack<T>::is_empty() const noexcept
{
    return m_top == 0;
}

template <typename T>
inline void Stack<T>::steal_data(Stack&& other)
{
    m_nelems = other.m_nelems;
    m_top = other.m_top;
    m_data = other.m_data;

    other.m_data = nullptr;
    other.m_top = 0;
    other.m_nelems = 1;
}

#endif