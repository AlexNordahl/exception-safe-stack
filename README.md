## Table of contents

1. Introduction
2. Comparisons
3. Summary
4. References

## Introduction

Project is a C++17 oriented implementation of a stack data structure that provides strong exception guarantee. Inspired by famous article published in November-December 1994 issue of the C++ Report by Tom Cargill. I will compare my stack template to that found in the article, explaining design choices and tradeoffs along the way.

## Comparisons
**Uninitialized members**
```cpp
Stack<int> y;
Stack<int> x = y;
assert( y.count() == 0 );
printf( "%u\n", x.count() );
    <outputs>
17736
```
The original stack had issues of outputing garbage values when uninitialized.
This is easily solved by using uniform inicialization in class body.
Now object is always in viable state.
```cpp
std::size_t m_top{};
std::size_t m_nelems{};
T* m_data{};
```

**Default And Parameterized  Constructor**
```cpp
template <class T>
Stack<T>::Stack()
{
top = -1;
v = new T[nelems=10];
if( v == 0 )
    throw "out of memory";
}
```
This version sets number of elements to 10 which is not very flexible.
```cpp
Stack() = default;
// -------------------------
template <typename T>
inline Stack<T>::Stack(std::size_t nelems) :
    m_nelems{nelems == 0 ? 1 : nelems}
{
    m_data = new T[m_nelems];
}
```
I introduced an additional constructor that allows the user to create a stack with an arbitrary size, instead of relying on the default size of 10.
The default constructor is kept because it is useful when creating “empty” objects, for example when a Stack is used as an element type in a std::vector or other STL containers that require default-constructible types. I also got rid of checking if value returned by new is 0 which is unnecessary because new can throw.
### Exceptions Thrown By T
The default constructor, copy constructor and assignment operator of T may throw exceptions just as the corresponding members of Stack do.

**Copy Constructor** (Problematic Version)
```cpp
template <class T>
Stack<T>::Stack(const Stack<T>& s)
{
  v = new T[nelems = s.nelems];        // leak
  if( v == 0 )
    throw "out of memory";
  if( s.top > -1 ){
    for(top = 0; top <= s.top; top++)
      v[top] = s.v[top];               // throw
    top--;
  }
}
```
If the copy assignment of an element throws an exception during the loop, the dynamically allocated memory is leaked because it is never released.
```cpp
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
```
In this version, if copying an element throws an exception, the allocated memory is properly released and the exception is rethrown. The member variables m_top and m_nelems are initialized in the member initializer list and are not reset after the exception. This is not a problem, because if an exception is thrown, the construction of the object is considered unsuccessful and the partially constructed object will not be used.

**Copy Assignment Operator** (Naive Approach)

```cpp
template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& s)
{
  delete [ ] v;                         // v undefined
  v = new T[nelems=s.nelems];           // throw
  if( v == 0 )
    throw "out of memory";
  if( s.top > -1 ){
    for(top = 0; top <= s.top; top++)
      v[top] = s.v[top];
    top--;
  }
  return *this;
}
```
Here is the issue that require addressing in the context of exceptions. First of which is deleting data **v** at the start of method. In case new o operator= throws, that data is gone and also pointer **v** is left in undefined state.
```cpp
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
inline void Stack<T>::swap(Stack& first, Stack& other)
{
    std::swap(first.m_top, other.m_top);
    std::swap(first.m_nelems, other.m_nelems);
    std::swap(first.m_data, other.m_data);
}
```
The solution to this problem is the copy-and-swap idiom, which reduces code duplication and provides the strong exception guarantee. It works by creating a local copy of the object and then swapping its internal data with the current object. Once the swap is complete, the temporary copy is safely destroyed, releasing the old resources.

In this implementation, a temporary copy is explicitly created inside the function rather than being passed as a parameter. This avoids ambiguity with the move assignment operator.

**Move Operations**
```cpp
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
```
Here is a straightforward and safe implementation of the move constructor and move assignment operator for Stack. Both operations transfer ownership of the dynamically allocated buffer and reset the moved-from object to a default, empty state.
I also marked them as noexcept since they cannot throw and this empowers compiler to do better optimization.

**Push**
```cpp
template <class T>
void Stack<T>::push(T element)
{
  top++;
  if( top == nelems-1 ){
    T* new_buffer = new T[nelems+=10]; // leak
    if( new_buffer == 0 )
      throw "out of memory";
    for(int i = 0; i < top; i++)
      new_buffer[i] = v[i];            // throw
    delete [ ] v;
    v = new_buffer;
  }
  v[top] = element;
}
```
In the naive implementation top is left in invalid state if something in the method throws. Also new_buffer is not properly deleted causing leak.
```cpp
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
                new_buffer[i] = m_data[i];
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
```
Similarly to previous solutions if new or operator= throws data is released and exception rethrown. In case assigning new value to m_data fails m_top is incremented after.
I also introduced push overload that supports move semantics.

**Pop and Top**
```cpp
template <class T>
T Stack<T>::pop()
{
  if( top < 0 )
    throw "pop on empty stack";
  return v[top--];                     // throw
}
```
This design is dangerous because it requires copying the element. If the copy operation throws an exception, the element has already been removed from the stack, which leads to data loss and leaves the stack in an inconsistent state.

To avoid this problem, the pop operation is split into two separate operations: pop and top.
```cpp
template <typename T>
inline void Stack<T>::pop()
{
    if (m_top > 0)
        --m_top;
    else
        throw std::runtime_error("cannot pop empty stack");
}

template <typename T>
inline T& Stack<T>::top()
{
    if (is_empty())
        throw std::runtime_error("cannot top empty stack");

    return m_data[m_top - 1];
}
```
In this design, pop simply removes the top element by decrementing m_top and does not return a value. Access to the element is provided separately through top(), which returns a reference. This separation ensures exception safety: no element is removed unless all potentially throwing operations have already completed successfully.

**Accessor Methods**
```cpp
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
```
Very straightforward and marked as const and noexcept for better optimization.

**Destructor**
```cpp
template <typename T>
inline Stack<T>::~Stack()
{
    delete[] m_data;
}
```
Implicitly marked as noexcept, m_data might be nullptr but this is ok since deleting nullptr is well-defined.
## Summary
This project presents a modern C++17 implementation of a stack that provides the strong exception guarantee. By applying RAII, and idioms such as copy-and-swap, the design eliminates resource leaks and undefined behavior present in the original implementation. The result is a robust, exception-safe container that remains consistent even when operations on the stored type throw exceptions.
## References
Article: https://ptgmedia.pearsoncmg.com/imprint_downloads/informit/aw/meyerscddemo/DEMO/MAGAZINE/CA_FRAME.HTM

Copy-and-swap idiom: https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom