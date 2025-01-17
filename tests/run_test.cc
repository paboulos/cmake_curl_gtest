#include "libapi.hpp"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>

int run(int argc, char** argv); // declaration
TEST(RunTest, RunOutputsCorrectLetter) {
  std::string expected {"ABC Foo is BETA\n"};
  std::stringstream buffer;
  // redirect cout
  auto prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
  run(0, NULL);
  auto output = buffer.str();
  // restore original buffer
  std::cout.rdbuf(prevcoutbuf);
  EXPECT_EQ(expected, output);
}