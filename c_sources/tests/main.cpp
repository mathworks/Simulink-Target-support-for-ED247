#include "gtest/gtest.h"

//#include "tap.h"

int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);

  /*
  testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
  listeners.Append(new tap::TapListener());
  */

  return RUN_ALL_TESTS();

}