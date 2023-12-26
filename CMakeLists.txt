cmake_minimum_required(VERSION 3.16)

project(lightorama
        VERSION 3.1.0
        DESCRIPTION "C99 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblightorama"
        LANGUAGES C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE PUBLIC_HEADERS "include/lightorama/*.h")
file(GLOB_RECURSE SRC_FILES "src/*.c")

add_library(lightorama ${PUBLIC_HEADERS} ${SRC_FILES})

set_target_properties(lightorama PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_include_directories(lightorama PUBLIC include)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

install(TARGETS lightorama
        LIBRARY DESTINATION lib/lightorama
        PUBLIC_HEADER DESTINATION include/lightorama)

add_executable(test_buffer test/buffer.c)
target_link_libraries(test_buffer lightorama)
add_test(buffer test_buffer)

add_executable(test_compress test/compress.c)
target_link_libraries(test_compress lightorama)
add_test(compress test_compress)

add_executable(test_model test/model.c)
target_link_libraries(test_model lightorama)
add_test(model test_model)

add_executable(test_time test/time.c)
target_link_libraries(test_time lightorama)
add_test(time test_time)

add_executable(test_intensity test/intensity.c)
target_link_libraries(test_intensity lightorama)
add_test(intensity test_intensity)

enable_testing()
