#include <gtest/gtest.h>

#include "../src/stack.hpp"

struct ThrowOnAssign
{
    static inline std::size_t assign_counter = 0;
    static inline std::size_t throw_at = 0;  // 0 => never throw
    static inline int alive = 0;

    int data{0};

    ThrowOnAssign()
    {
        ++alive;
    }

    explicit ThrowOnAssign(int x) :
        data(x)
    {
        ++alive;
    }

    ThrowOnAssign(const ThrowOnAssign& other) :
        data(other.data)
    {
        ++alive;
    }

    ThrowOnAssign& operator=(const ThrowOnAssign& other)
    {
        if (throw_at != 0 && ++assign_counter == throw_at)
        {
            throw std::runtime_error("ThrowOnAssign: operator= throws");
        }
        data = other.data;
        return *this;
    }

    ~ThrowOnAssign()
    {
        --alive;
    }

    static void Reset(std::size_t new_throw_at = 0)
    {
        assign_counter = 0;
        throw_at = new_throw_at;
    }
};

TEST(T_Throws, CopyConstructorThrows)
{
    ThrowOnAssign::Reset(3);

    Stack<ThrowOnAssign> stack1 {3};

    Stack<ThrowOnAssign> stack2 = stack1;
}