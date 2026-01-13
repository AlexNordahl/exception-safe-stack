#include "../src/stack.hpp"
#include <gtest/gtest.h>

TEST(Push, FillingStack)
{
    Stack<double> stack1{5};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(4);
    stack1.push(5);
    
    ASSERT_EQ(stack1.size(), 5);
    ASSERT_EQ(stack1.count(), 5);
    ASSERT_EQ(stack1.top(), 5);
    ASSERT_FALSE(stack1.is_empty());
}

TEST(Push, OverfillingStack)
{
    Stack<double> stack1{5};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(4);
    stack1.push(5);
    stack1.push(6);
    
    ASSERT_EQ(stack1.size(), 10);
    ASSERT_EQ(stack1.count(), 6);
    ASSERT_EQ(stack1.top(), 6);
    ASSERT_FALSE(stack1.is_empty());
}