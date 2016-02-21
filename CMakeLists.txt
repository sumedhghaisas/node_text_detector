cmake_minimum_required(VERSION 3.0)

set(HUNTER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.hunter/")

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/sumedhghaisas/hunter/archive/for_node_opencv.tar.gz"
    SHA1 "1b65534f55772663e141276b2ef6ab265eef66d9"
)

project(node_text_detector)

set(HUNTER_STATUS_DEBUG ON)

hunter_add_package(tesseract)
hunter_add_package(PNG)
hunter_add_package(OpenCV)

find_package(OpenCV REQUIRED)
find_package(tesseract CONFIG REQUIRED)

set(srcs
  src/sync.cc
  src/ocr/ocr.cpp
)

add_library(${PROJECT_NAME} SHARED ${srcs})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} tesseract::tesseract PNG::png)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

FILE(GLOB_RECURSE DLL_FILES ${HUNTER_ROOT} "*.dll")

foreach(str ${DLL_FILES})
  ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${str} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/.
  )
endforeach()

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Resources/trained_classifierNM1.xml ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/.
)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Resources/trained_classifierNM2.xml ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/.
)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources/tessdata ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/tessdata/.
)



