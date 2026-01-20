#include <gtest/gtest.h>

#include <stdexcept>

#include "../src/stack.hpp"

struct ThrowOnAssign
{
    static inline std::size_t assign_counter = 0;
    static inline std::size_t throw_at = 0;
    static inline std::size_t alive = 0;

    int data{0};

    ThrowOnAssign() { ++alive; }

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
            throw std::runtime_error("ThrowOnAssign: operator= throws");
        data = other.data;
        return *this;
    }

    ~ThrowOnAssign() { --alive; }

    static void Reset(std::size_t new_throw_at = 0)
    {
        assign_counter = 0;
        throw_at = new_throw_at;
    }
};

TEST(StackExceptions, CopyConstructorThrows_NoLeak_SourceIntact)
{
    ThrowOnAssign::Reset(0);
    EXPECT_EQ(ThrowOnAssign::alive, 0);

    {
        Stack<ThrowOnAssign> src{3};
        src.push(ThrowOnAssign{1});
        src.push(ThrowOnAssign{2});

        const auto alive_before = ThrowOnAssign::alive;
        EXPECT_EQ(src.count(), 2);
        EXPECT_EQ(src.top().data, 2);
        EXPECT_EQ(src.size(), 3);

        ThrowOnAssign::Reset(2);

        EXPECT_THROW(Stack<ThrowOnAssign>{src}, std::runtime_error);

        EXPECT_EQ(src.count(), 2);
        EXPECT_EQ(src.top().data, 2);
        EXPECT_EQ(src.size(), 3);
        EXPECT_EQ(ThrowOnAssign::alive, alive_before);
    }

    EXPECT_EQ(ThrowOnAssign::alive, 0);
}

TEST(StackExceptions, CopyAssignmentThrows_NoLeak_SourceIntact)
{
    ThrowOnAssign::Reset(0);
    EXPECT_EQ(ThrowOnAssign::alive, 0);

    {
        Stack<ThrowOnAssign> src{3};
        src.push(ThrowOnAssign{1});
        src.push(ThrowOnAssign{2});

        const auto alive_before = ThrowOnAssign::alive;
        EXPECT_EQ(src.count(), 2);
        EXPECT_EQ(src.top().data, 2);
        EXPECT_EQ(src.size(), 3);

        ThrowOnAssign::Reset(2);

        Stack<ThrowOnAssign> other {};
        EXPECT_THROW(other = src, std::runtime_error);

        EXPECT_EQ(src.count(), 2);
        EXPECT_EQ(src.top().data, 2);
        EXPECT_EQ(src.size(), 3);
        EXPECT_EQ(ThrowOnAssign::alive, alive_before);
    }

    EXPECT_EQ(ThrowOnAssign::alive, 0);
}

// TEST(StackExceptions, MoveConstructorThrows_NoLeak_SourceIntact)
// {
//     ThrowOnAssign::Reset(0);
//     EXPECT_EQ(ThrowOnAssign::alive, 0);

//     {
//         Stack<ThrowOnAssign> src{3};
//         src.push(ThrowOnAssign{1});
//         src.push(ThrowOnAssign{2});

//         const auto alive_before = ThrowOnAssign::alive;
//         EXPECT_EQ(src.count(), 2);
//         EXPECT_EQ(src.top().data, 2);
//         EXPECT_EQ(src.size(), 3);

//         ThrowOnAssign::Reset(2);

//         EXPECT_THROW(Stack<ThrowOnAssign>{std::move(src)}, std::runtime_error);

//         EXPECT_EQ(src.count(), 2);
//         EXPECT_EQ(src.top().data, 2);
//         EXPECT_EQ(src.size(), 3);
//         EXPECT_EQ(ThrowOnAssign::alive, alive_before);
//     }

//     EXPECT_EQ(ThrowOnAssign::alive, 0);
// }

TEST(StackExceptions, PushThrows_NoGrow_CountUnchanged)
{
    ThrowOnAssign::Reset(0);
    EXPECT_EQ(ThrowOnAssign::alive, 0);

    {
        Stack<ThrowOnAssign> s{5};
        s.push(ThrowOnAssign{10});
        s.push(ThrowOnAssign{20});
        EXPECT_EQ(s.count(), 2);
        EXPECT_EQ(s.top().data, 20);

        const auto alive_before = ThrowOnAssign::alive;

        ThrowOnAssign::Reset(1);
        EXPECT_THROW(s.push(ThrowOnAssign{30}), std::runtime_error);

        EXPECT_EQ(s.count(), 2);
        EXPECT_EQ(s.top().data, 20);

        EXPECT_EQ(ThrowOnAssign::alive, alive_before);
    }

    EXPECT_EQ(ThrowOnAssign::alive, 0);
}

TEST(StackExceptions, PushGrowThrows_StackIntact_NoLeak)
{
    ThrowOnAssign::Reset(0);
    EXPECT_EQ(ThrowOnAssign::alive, 0);

    {
        Stack<ThrowOnAssign> s{2};
        s.push(ThrowOnAssign{1});
        s.push(ThrowOnAssign{2});
        EXPECT_EQ(s.count(), 2);
        EXPECT_EQ(s.size(), 2);
        EXPECT_EQ(s.top().data, 2);

        const auto alive_before = ThrowOnAssign::alive;

        ThrowOnAssign::Reset(1);
        EXPECT_THROW(s.push(ThrowOnAssign{3}), std::runtime_error);

        EXPECT_EQ(s.count(), 2);
        EXPECT_EQ(s.size(), 2);
        EXPECT_EQ(s.top().data, 2);

        EXPECT_EQ(ThrowOnAssign::alive, alive_before);

        ThrowOnAssign::Reset(0);
        EXPECT_NO_THROW(s.pop());
        EXPECT_EQ(s.top().data, 1);
    }

    EXPECT_EQ(ThrowOnAssign::alive, 0);
}

TEST(StackExceptions, CopyAssignmentThrows_TargetUnchanged_NoLeak)
{
    ThrowOnAssign::Reset(0);
    EXPECT_EQ(ThrowOnAssign::alive, 0);

    {
        Stack<ThrowOnAssign> a{3};
        a.push(ThrowOnAssign{1});
        a.push(ThrowOnAssign{2});

        Stack<ThrowOnAssign> b{4};
        b.push(ThrowOnAssign{7});
        b.push(ThrowOnAssign{8});

        const auto b_count_before = b.count();
        const auto b_size_before = b.size();
        const auto b_top_before = b.top().data;
        const auto alive_before = ThrowOnAssign::alive;

        ThrowOnAssign::Reset(2);
        EXPECT_THROW(b = a, std::runtime_error);

        EXPECT_EQ(b.count(), b_count_before);
        EXPECT_EQ(b.size(), b_size_before);
        EXPECT_EQ(b.top().data, b_top_before);

        EXPECT_EQ(ThrowOnAssign::alive, alive_before);
    }

    EXPECT_EQ(ThrowOnAssign::alive, 0);
}
