add_executable(org_test
  parser/document.cc
  parser/headline.cc
  parser/block.cc
  parser/drawer.cc
  )

target_link_libraries(org_test
  GTest::gtest
  GTest::gtest_main
  org_parser)

add_executable(org_main
  parser/main.cc
  )
target_link_libraries(org_main
  org_parser)
