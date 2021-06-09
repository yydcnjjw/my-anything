find_package(Boost REQUIRED COMPONENTS regex)

set(PROJECT_SOURCES
  parser/parser.cc
  )

add_library(org_parser SHARED ${PROJECT_SOURCES})
target_link_libraries(org_parser
  Boost::regex
  )
target_include_directories(org_parser
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  )
