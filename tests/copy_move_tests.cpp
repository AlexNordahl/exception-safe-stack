#include "../src/stack.hpp"
#include <gtest/gtest.h>

TEST(Copying, CopyConstructor)
{
    Stack<double> stack1{10};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);

    Stack stack2{stack1};

    ASSERT_EQ(stack1.size(), 10);
    ASSERT_EQ(stack1.count(), 3);

    ASSERT_EQ(stack2.size(), 10);
    ASSERT_EQ(stack2.count(), 3);

    ASSERT_EQ(stack1.top(), 3);
}

TEST(Copying, CopyAssignment)
{
    Stack<double> stack1{10};
    Stack<double> stack2{5};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);

    stack2 = stack1;

    ASSERT_EQ(stack1.size(), 10);
    ASSERT_EQ(stack1.count(), 3);

    ASSERT_EQ(stack2.size(), 10);
    ASSERT_EQ(stack2.count(), 3);

    ASSERT_EQ(stack1.top(), 3);
    ASSERT_EQ(stack2.top(), 3);
}

TEST(Copying, CopyConstructorUninitialized)
{
    Stack<int> stack1;
    Stack<int> stack2 {stack1};

    ASSERT_EQ(stack1.size(), 0);
    ASSERT_EQ(stack1.count(), 0);

    ASSERT_EQ(stack2.size(), 0);
    ASSERT_EQ(stack2.count(), 0);

    ASSERT_TRUE(stack1.is_empty());
    ASSERT_TRUE(stack2.is_empty());
}

TEST(Copying, CopyAssignmentUninitialized)
{
    Stack<int> stack1, stack2;
    stack2 = stack1;

    ASSERT_EQ(stack1.size(), 0);
    ASSERT_EQ(stack1.count(), 0);

    ASSERT_EQ(stack2.size(), 0);
    ASSERT_EQ(stack2.count(), 0);

    ASSERT_TRUE(stack1.is_empty());
    ASSERT_TRUE(stack2.is_empty());
}

TEST(Copying, MoveConstructor)
{
    Stack<double> stack1{10};

    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    
    Stack stack2 {std::move(stack1)};

    ASSERT_EQ(stack1.size(), 1);
    ASSERT_EQ(stack1.count(), 0);
    ASSERT_TRUE(stack1.is_empty());

    ASSERT_EQ(stack2.size(), 10);
    ASSERT_EQ(stack2.count(), 3);
    ASSERT_FALSE(stack2.is_empty());

    ASSERT_EQ(stack2.top(), 3);
}