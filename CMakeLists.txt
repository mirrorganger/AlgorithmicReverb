cmake_minimum_required(VERSION 3.15)
project(AlgorithmicReverb)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(deps/EuterpeSnd)
add_subdirectory(deps/JUCE)

add_subdirectory(SchroederReverb)
add_subdirectory(DattoroReverb)
