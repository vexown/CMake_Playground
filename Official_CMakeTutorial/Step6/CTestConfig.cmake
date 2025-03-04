# For this tutorial, a public dashboard server is used and its corresponding CTestConfig.cmake file is provided for you in this step's root directory. 
# In practice, this file would be downloaded from a project's Settings page on the CDash instance intended to host the test results. 
# Once downloaded from CDash, the file should not be modified locally.

# See more details here: https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Support%20for%20a%20Testing%20Dashboard.html

set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
