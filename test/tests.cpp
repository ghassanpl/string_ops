/// Copyright 2017-2020 Ghassan.pl
/// Usage of the works is permitted provided that this instrument is retained with
/// the works, so that any entity that uses the works is notified of this instrument.
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.

#define FMT_USE_WINDOWS_H 0
#define FMT_HEADER_ONLY 1
#include "../include/string_ops.h"
#include <gtest/gtest.h>

using namespace string_ops;

TEST(isalpha, works_for_all_ascii)
{
  
}

TEST(make_sv, works_for_general_case)
{

}
TEST(make_sv, works_for_nulls)
{

}
TEST(make_sv, fails_for_invalid_range)
{
  std::string hello = "hello";
  EXPECT_EQ(make_sv(hello.end(), hello.begin()), "hello");
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
