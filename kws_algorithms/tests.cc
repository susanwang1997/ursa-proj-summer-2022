#include <thread>
#include <UnitTest++/UnitTest++.h>

#include "TwoStageParallel.hh"
#include "unittests/LevelCoverStrategy_test.hh"
#include "unittests/KWSGraph_test.hh"
#include "unittests/Keywords_test.hh"
#include "unittests/Matrix_test.hh"
#include "unittests/AvgShortestDistance_test.hh"
#include "unittests/Activation_test.hh"
#include "unittests/TwoStageParallel_test.hh"

int execute_tests()
{
  cout << "#THREADS: " << thread::hardware_concurrency() << endl;
  return UnitTest::RunAllTests();
}

int main(int argc, char* argv[]) {
  if(argc>1) {
    string flag = argv[1];
    if(flag=="-rc") { //check for race condition by executing test many times
      int num_of_failures = 0;
      while (num_of_failures==0) {
        num_of_failures = execute_tests();
      }
    }
    else {
      cout << "ERROR: Invalid flag. The only flag that can be set is -rc for checking race conditions.\n";
    }
  }
  else {
    execute_tests();
  }
}
