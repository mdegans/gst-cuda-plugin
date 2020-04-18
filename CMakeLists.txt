cmake_minimum_required(VERSION 3.10)

project(GstCudaPlugin CUDA CXX)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(gst-cuda-plugin)