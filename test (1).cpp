// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"

#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        // initialize random seed
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart pointer to hold our collection
    unique_ptr<vector<int>> collection;

    void SetUp() override
    {
        // create a new collection to be used in the test
        collection.reset(new vector<int>);
    }

    void TearDown() override
    {
        // erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
        {
            collection->push_back(rand() % 100);
        }
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that the smart pointer exists when created.
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    ASSERT_TRUE(collection);
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

/*
// This test will always fail.
TEST_F(CollectionTest, AlwaysFail)
{
  FAIL();
}
*/

// Test that adding a single value to an empty collection works.
TEST_F(CollectionTest, CanAddSingleValueToEmptyVector)
{
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 1);
}

// Test that adding five values to a collection works.
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    ASSERT_TRUE(collection->empty());

    add_entries(5);

    EXPECT_FALSE(collection->empty());
    EXPECT_EQ(collection->size(), 5);
}

// Test that max_size is greater than or equal to size for 0, 1, 5, and 10 entries.
TEST_F(CollectionTest, MaxSizeIsGreaterThanOrEqualToSizeForMultipleEntryCounts)
{
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(1);
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(4); // total now 5
    EXPECT_GE(collection->max_size(), collection->size());

    add_entries(5); // total now 10
    EXPECT_GE(collection->max_size(), collection->size());
}

// Test that capacity is greater than or equal to size for 0, 1, 5, and 10 entries.
TEST_F(CollectionTest, CapacityIsGreaterThanOrEqualToSizeForMultipleEntryCounts)
{
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(1);
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(4); // total now 5
    EXPECT_GE(collection->capacity(), collection->size());

    add_entries(5); // total now 10
    EXPECT_GE(collection->capacity(), collection->size());
}

// Test that resizing increases the collection size.
TEST_F(CollectionTest, ResizeIncreasesCollectionSize)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5);

    collection->resize(10);

    EXPECT_EQ(collection->size(), 10);
    EXPECT_GE(collection->capacity(), collection->size());
}

// Test that resizing decreases the collection size.
TEST_F(CollectionTest, ResizeDecreasesCollectionSize)
{
    add_entries(10);
    ASSERT_EQ(collection->size(), 10);

    collection->resize(5);

    EXPECT_EQ(collection->size(), 5);
}

// Test that resizing to zero makes the collection empty.
TEST_F(CollectionTest, ResizeToZeroMakesCollectionEmpty)
{
    add_entries(10);
    ASSERT_FALSE(collection->empty());

    collection->resize(0);

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Test that clear erases the collection.
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5);
    ASSERT_FALSE(collection->empty());

    collection->clear();

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Test that erase(begin, end) erases the collection.
TEST_F(CollectionTest, EraseBeginToEndErasesCollection)
{
    add_entries(5);
    ASSERT_EQ(collection->size(), 5);

    collection->erase(collection->begin(), collection->end());

    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

// Test that reserve increases capacity but does not change size.
TEST_F(CollectionTest, ReserveIncreasesCapacityButNotSize)
{
    add_entries(5);
    const size_t originalSize = collection->size();
    const size_t originalCapacity = collection->capacity();

    collection->reserve(originalCapacity + 20);
    
    EXPECT_EQ(collection->size(), originalSize);
    EXPECT_GE(collection->capacity(), originalCapacity + 20);
}

// Negative test: verify out_of_range is thrown when at() uses an invalid index.
TEST_F(CollectionTest, AtThrowsOutOfRangeWhenIndexIsInvalid)
{
    ASSERT_TRUE(collection->empty());
    EXPECT_THROW(collection->at(0), out_of_range);
}

// Positive custom test: verify front() and back() return the same value when one item exists.
TEST_F(CollectionTest, FrontAndBackMatchWhenVectorHasOneElement)
{
    collection->push_back(42);

    ASSERT_EQ(collection->size(), 1);
    EXPECT_EQ(collection->front(), 42);
    EXPECT_EQ(collection->back(), 42);
    EXPECT_EQ(collection->front(), collection->back());
}

// Negative custom test: verify pop_back on an empty vector is not performed.
// This proves the collection remains unchanged when empty.
TEST_F(CollectionTest, EmptyVectorDoesNotContainValueAtIndexZero)
{
    ASSERT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
    EXPECT_THROW(collection->at(0), out_of_range);
}