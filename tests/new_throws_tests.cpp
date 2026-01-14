#include <gtest/gtest.h>
#include "../src/stack.hpp"

constexpr std::size_t big_buffer{10000000000000000};

TEST(NewBadAlloc, ConstructorThrows)
{
    EXPECT_THROW(Stack<double> stack{big_buffer}, std::bad_alloc);

    // no consistency state to check
}

TEST(NewBadAlloc, CopyConstructorThrows)
{
    /*
        Copy Constructor cannot throw bad_alloc,
        because it's not possible to create stack
        that has too big m_nelems and provide it.
    */
}

TEST(NewBadAlloc, CopyAssignmentThrows)
{
    /*
        to add
    */
}

TEST(NewBadAlloc, MoveConstructorThrows)
{

}