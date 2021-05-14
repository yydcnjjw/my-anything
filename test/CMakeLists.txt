enable_testing()

find_package(GTest REQUIRED)

add_subdirectory(document)

add_executable(my_anything_test
  test.cc
  )

target_link_libraries(my_anything_test
  GTest::gtest
  GTest::gtest_main
  my_anything_lib)
