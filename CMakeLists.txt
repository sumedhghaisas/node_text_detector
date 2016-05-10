cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/sumedhghaisas/hunter/archive/for_node_opencv.tar.gz"
    SHA1 "9a57b227303abe1faf67b2f7c78edb346f58899b"
)

project(scene_text_detector)

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

set(new_srcs
  src/new_ocr/sync.cpp
  src/new_ocr/AvailableChar.cpp
  src/new_ocr/AvailablePlate.cpp
  src/new_ocr/begin.cpp
  src/new_ocr/ExtractPlates.cpp
  src/new_ocr/FindChars.cpp
  src/new_ocr/Pre_process.cpp
)

add_library(new_ocr SHARED ${new_srcs})
set_target_properties(new_ocr PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(new_ocr ${OpenCV_LIBS} tesseract::tesseract ${CMAKE_JS_LIB})

set(ClassifierFiles
    ${CMAKE_CURRENT_LIST_DIR}/Resources/trained_classifierNM1.xml
    ${CMAKE_CURRENT_LIST_DIR}/Resources/trained_classifierNM2.xml
)

foreach(classifier ${ClassifierFiles})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${classifier} ${CMAKE_BINARY_DIR}/Release/)
endforeach()
