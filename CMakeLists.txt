cmake_minimum_required(VERSION 3.0)

set(HUNTER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.hunter/")

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/sumedhghaisas/hunter/archive/for_node_opencv.tar.gz"
    SHA1 "4b5d8683fd25a29ec6741f0ef2d5c8db7e0d7d4f"
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

add_library(${PROJECT_NAME} SHARED main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} tesseract::tesseract)


