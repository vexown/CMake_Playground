// A simple program that computes the square root of a number
#include <cmath>
#include <iostream>
#include <string>

#include "TutorialConfig.h"

// should we include the MathFunctions header?
#ifdef USE_MYMATH
#  include "MathFunctions.h"
#endif

int main(int argc, char* argv[])
{
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }

  // convert input to double
  const double inputValue = std::stod(argv[1]);
  double outputValue = 0.0;

if(argc == 3)
{
  /* convert arg to unsigned int */
  const unsigned int inputArg2 = std::stod(argv[2]);
  std::cout << "Main Arg[2] is: " << inputArg2 << std::endl;

  if(inputArg2 == 1U)
  {
    outputValue = mysqrt2(inputValue);
  }
  else
  {
    std::cout << "Invalid Arg[2] - exiting program... " << std::endl;
    return 1;
  }
}
else
{
  // which square root function should we use?
#ifdef USE_MYMATH
  outputValue = mysqrt(inputValue);
#else
  outputValue = sqrt(inputValue);
#endif
}

  std::cout << "The square root of " << inputValue << " is " << outputValue
            << std::endl;
  return 0;
}
