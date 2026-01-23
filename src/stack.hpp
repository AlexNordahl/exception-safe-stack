#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <utility>

template <typename T>
class Stack
{
public:
    Stack() = default;
    Stack(std::size_t nelems);
    Stack(const Stack&);
    Stack& operator=(const Stack&);
    Stack(Stack&&) noexcept;
    Stack& operator=(Stack&&) noexcept;
    ~Stack();

    void push(const T&);
    void push(T&&);

    T& top();
    const T& top() const;
    void pop();

    std::size_t count() const noexcept;
    std::size_t size() const noexcept;
    bool is_empty() const noexcept;

private:
    void swap(Stack& first, Stack& other);
    std::size_t growth_factor() { return 2; }
    void expand();

    std::size_t m_top{};
    std::size_t m_nelems{};
    T* m_data{};
};

template <typename T>
inline Stack<T>::Stack(std::size_t nelems) :
    m_nelems{nelems == 0 ? 1 : nelems}
{
    m_data = new T[m_nelems];
}

template <typename T>
inline Stack<T>::Stack(const Stack& other) :
    m_top{other.m_top},
    m_nelems{other.m_nelems}
{
    try
    {
        m_data = new T[other.m_nelems];
        for (std::size_t i = 0; i < m_top; ++i)
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
    if (this != &other)
    {
        Stack<T> temp{other};
        swap(*this, temp);
    }

    return *this;
}

template <typename T>
inline Stack<T>::Stack(Stack&& other) noexcept
{
    swap(*this, other);
}

template <typename T>
inline Stack<T>& Stack<T>::operator=(Stack&& other) noexcept
{
    if (this == &other)
        return *this;

    delete[] m_data;

    m_data = other.m_data;
    m_top = other.m_top;
    m_nelems = other.m_nelems;

    other.m_data = nullptr;
    other.m_top = 0;
    other.m_nelems = 0;

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
    expand();

    auto temp_top = m_top;
    m_data[temp_top] = element;
    ++m_top;
}

template <typename T>
inline void Stack<T>::push(T&& element)
{
    expand();

    auto temp_top = m_top;
    m_data[temp_top] = std::move(element);
    ++m_top;
}

template <typename T>
inline T& Stack<T>::top()
{
    if (is_empty())
        throw std::runtime_error("cannot top empty stack");

    return m_data[m_top - 1];
}

template <typename T>
inline const T& Stack<T>::top() const
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
inline void Stack<T>::swap(Stack& first, Stack& other)
{
    std::swap(first.m_top, other.m_top);
    std::swap(first.m_nelems, other.m_nelems);
    std::swap(first.m_data, other.m_data);
}

template <typename T>
inline void Stack<T>::expand()
{
    if (m_top == m_nelems)
    {
        auto new_nelems = m_nelems == 0 ? 1 : m_nelems * growth_factor();
        T* new_buffer = nullptr;

        try
        {
            new_buffer = new T[new_nelems];
            for (std::size_t i = 0; i < m_top; ++i)
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
}

#endif