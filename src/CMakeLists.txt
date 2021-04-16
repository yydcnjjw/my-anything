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
find_package(rxcpp PATHS /home/yydcnjjw/workspace/code/project/RxCpp/deploy REQUIRED)

set(TS_FILES my-anything_zh_CN.ts)

set(PROJECT_SOURCES
  main.cc
  app.cc
  ui/cmdline.cc
  service/command_service.cc
  service/shortcut_service.cc
  service/complete_service.cc
  ${TS_FILES}
  )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(my-anything
    ${PROJECT_SOURCES}
    )

  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  add_executable(my-anything
    ${PROJECT_SOURCES}
    )

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(my-anything SYSTEM
  PUBLIC
  /home/yydcnjjw/workspace/code/project/di/include
  /home/yydcnjjw/workspace/code/project/di/extension/include
  )

target_link_libraries(my-anything PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  ${XAPIAN_LIBRARIES}
  spdlog::spdlog
  rxcpp)

