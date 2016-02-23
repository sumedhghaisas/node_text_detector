cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/sumedhghaisas/hunter/archive/for_node_opencv.tar.gz"
    SHA1 "9a57b227303abe1faf67b2f7c78edb346f58899b"
)

project(node_text_detector)

hunter_add_package(tesseract)
hunter_add_package(PNG)
hunter_add_package(OpenCV)

find_package(OpenCV REQUIRED)
find_package(tesseract CONFIG REQUIRED)

set(srcs
  src/sync.cc
  src/ocr/ocr.cpp
)

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME} SHARED ${srcs})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} tesseract::tesseract ${CMAKE_JS_LIB})

set(ClassifierFiles
    ${CMAKE_CURRENT_LIST_DIR}/Resources/trained_classifierNM1.xml
    ${CMAKE_CURRENT_LIST_DIR}/Resources/trained_classifierNM2.xml
)

foreach(classifier ${ClassifierFiles})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${classifier} ${CMAKE_BINARY_DIR}/Release/)
endforeach()
