#include "../src/stack.hpp"
#include <gtest/gtest.h>

TEST(Push, FillingUninitializedStack)
{
    Stack<double> stack1{};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(4);
    stack1.push(5);
    
    ASSERT_EQ(stack1.size(), 8);
    ASSERT_EQ(stack1.count(), 5);
    ASSERT_EQ(stack1.top(), 5);
    ASSERT_FALSE(stack1.is_empty());
}

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

TEST(Pop, PopOneItem)
{
    Stack<double> stack1{5};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(4);

    stack1.pop();

    ASSERT_EQ(stack1.size(), 5);
    ASSERT_EQ(stack1.count(), 3);
    ASSERT_EQ(stack1.top(), 3);
    ASSERT_FALSE(stack1.is_empty());
}

TEST(Pop, PopAndAdd)
{
    Stack<double> stack1{5};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    stack1.push(4);

    stack1.pop();

    stack1.push(4);

    ASSERT_EQ(stack1.size(), 5);
    ASSERT_EQ(stack1.count(), 4);
    ASSERT_EQ(stack1.top(), 4);
    ASSERT_FALSE(stack1.is_empty());
}