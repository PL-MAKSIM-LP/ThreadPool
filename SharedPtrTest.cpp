#include <gtest/gtest.h>
#include "SharedPtr.h"

struct Dummy {
  int value;
  Dummy(int v) : value(v) {}
};

TEST(MySharedPtrTest, ConstructorCreatesValidPointer) {
  MySharedPtr<Dummy> ptr(Dummy(42));
  EXPECT_TRUE(ptr);
  EXPECT_EQ(ptr->value, 42);
  EXPECT_EQ((*ptr).value, 42);
  EXPECT_EQ(ptr.use_count(), 1);
}

TEST(MySharedPtrTest, CopyConstructorIncrementsUseCount) {
  MySharedPtr<Dummy> ptr1(Dummy(10));
  MySharedPtr<Dummy> ptr2 = ptr1;
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr1->value, 10);
  EXPECT_EQ(ptr2->value, 10);
}

TEST(MySharedPtrTest, CopyAssignmentIncrementsUseCount) {
  MySharedPtr<Dummy> ptr1(Dummy(55));
  MySharedPtr<Dummy> ptr2;
  ptr2 = ptr1;
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr2->value, 55);
}

TEST(MySharedPtrTest, MoveConstructorTransfersOwnership) {
  MySharedPtr<Dummy> ptr1(Dummy(77));
  size_t count = ptr1.use_count();
  MySharedPtr<Dummy> ptr2 = std::move(ptr1);
  EXPECT_FALSE(ptr1);  // ptr1 должен быть пуст
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr2.use_count(), count);
  EXPECT_EQ(ptr2->value, 77);
}

TEST(MySharedPtrTest, MoveAssignmentTransfersOwnership) {
  MySharedPtr<Dummy> ptr1(Dummy(100));
  MySharedPtr<Dummy> ptr2;
  ptr2 = std::move(ptr1);
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr2->value, 100);
}

TEST(MySharedPtrTest, NullAfterMove) {
  MySharedPtr<Dummy> ptr1(Dummy(5));
  MySharedPtr<Dummy> ptr2 = std::move(ptr1);
  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
}
