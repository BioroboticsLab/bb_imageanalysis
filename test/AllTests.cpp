

#include "gtest/gtest.h"
#include "LocalizerTest.cpp"
//#include "RecognizerTest.cpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
