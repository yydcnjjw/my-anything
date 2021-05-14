add_subdirectory(document)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6
  # Qt5
  COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(spdlog REQUIRED)
find_package(Xapian REQUIRED)
find_package(Protobuf REQUIRED)

set(PROJECT_SOURCES
  app.cc
  ui/cmdline.cc
  service/command_service.cc
  service/shortcut_service.cc
  service/complete_service.cc
  )


add_library(my_anything_lib
  ${PROJECT_SOURCES}
  )

set(MY_ANYTHING_INC
  ${VENDOR_SOURCE_DIR}/di/include
  ${VENDOR_SOURCE_DIR}/di/extension/include
  ${VENDOR_SOURCE_DIR}/cld3/src
  ${VENDOR_BINARY_DIR}/cld3
  ${VENDOR_SOURCE_DIR}/RxCpp/Rx/v2/src/rxcpp
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(MY_ANYTHING_LIB
  Qt${QT_VERSION_MAJOR}::Widgets
  ${XAPIAN_LIBRARIES}
  spdlog::spdlog
  cld3
  ${Protobuf_LITE_LIBRARIES}
  org_parser
  )

target_include_directories(my_anything_lib SYSTEM
  PUBLIC
  ${MY_ANYTHING_INC}
  )

target_link_libraries(my_anything_lib
  PUBLIC
  ${MY_ANYTHING_LIB})


add_executable(my_anything
  main.cc
  )

target_link_libraries(my_anything PRIVATE
  my_anything_lib)
