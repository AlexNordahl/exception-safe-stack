#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>  // std::size_t
#include <iostream>

template <typename T>
class Stack
{
public:
    Stack(std::size_t nelems = 10);
    Stack(const Stack&);
    Stack& operator=(const Stack&);
    Stack(Stack&&);
    Stack& operator=(Stack&&);
    ~Stack();

    void push(const T&);
    // void push(const T&&);

    T top();
    void pop();

    std::size_t count() const noexcept;
    std::size_t size() const noexcept;
    bool is_empty() const noexcept;

private:
    std::size_t growth_factor()
    {
        return 2;
    }

    std::size_t m_top{};
    std::size_t m_nelems{};
    T* m_data{};
};

template <typename T>
inline Stack<T>::Stack(std::size_t nelems) :
    m_top{0},
    m_nelems{nelems}
{
    m_data = new T[m_nelems];
}

template <typename T>
inline Stack<T>::Stack(const Stack& other)
{
    m_nelems = other.m_nelems;
    m_top = other.m_top;
    m_data = new T[m_nelems];

    for (std::size_t i = 0; i < m_nelems; ++i)
    {
        m_data[i] = other.m_data[i];
    }
}

template <typename T>
inline Stack<T>& Stack<T>::operator=(const Stack& other)
{
    if (this == &other)
        return *this;

    if (is_empty() == false)
    {
        delete[] m_data;
    }

    m_nelems = other.m_nelems;
    m_top = other.m_top;
    m_data = new T[m_nelems];

    for (std::size_t i = 0; i < m_nelems; ++i)
    {
        m_data[i] = other.m_data[i];
    }

    return *this;
}

template <typename T>
inline Stack<T>::Stack(Stack&& other)
{
    m_nelems = other.m_nelems;
    m_top = other.m_top;
    m_data = other.m_data;

    other.m_data = nullptr;
    other.m_top = 0;
    other.m_nelems = 0;
}

template <typename T>
inline Stack<T>& Stack<T>::operator=(Stack&& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (is_empty() == false)
    {
        delete[] m_data;
    }

    m_nelems = other.m_nelems;
    m_top = other.m_top;
    m_data = other.m_data;

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
    std::cout << m_top << " " << m_nelems << '\n';
    if (m_top == m_nelems)
    {
        auto new_nelems = m_nelems * growth_factor();
        auto new_buffer = new T[new_nelems];
        for (std::size_t i = 0; i < m_nelems; ++i)
        {
            new_buffer[i] = m_data[i];
        }
        delete[] m_data;
        m_data = new_buffer;
        m_nelems = new_nelems;
    }
    m_data[m_top++] = element;
}

template <typename T>
inline T Stack<T>::top()
{
    return m_data[m_top - 1];
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
    return m_nelems == 0;
}

#endif