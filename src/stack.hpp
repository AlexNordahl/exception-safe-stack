#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>  // std::size_t

template <typename T>
class Stack
{
public:
    Stack() = default;
    Stack(const Stack&);
    T& operator=(const Stack&);
    Stack(Stack&&);
    T& operator=(Stack&&);
    ~Stack();

    void push(const T&);
    void push(const T&&);

    T top();
    void pop();

    std::size_t count() const;
    bool is_empty() const noexcept;

private:
    T* m_data {};
    std::size_t m_nelems {}; 
    int m_top {};
};

#endif

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
inline T& Stack<T>::operator=(const Stack& other)
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

    for (std::size_t i = 0; i < count; ++i)
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
inline T& Stack<T>::operator=(Stack&& other)
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
inline std::size_t Stack<T>::count() const
{
    return m_nelems;
}

template <typename T>
inline bool Stack<T>::is_empty() const noexcept
{
    return m_nelems == 0;
}