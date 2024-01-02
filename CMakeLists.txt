cmake_minimum_required(VERSION 3.16)

project(lorproto
        VERSION 4.0.0
        DESCRIPTION "C99 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblorproto"
        LANGUAGES C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE PUBLIC_HEADERS "include/lorproto/*.h")
file(GLOB_RECURSE SRC_FILES "src/*.c")

add_library(lorproto ${PUBLIC_HEADERS} ${SRC_FILES})

set_target_properties(lorproto PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_include_directories(lorproto PUBLIC include)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else ()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

install(TARGETS lorproto
        LIBRARY DESTINATION lib/lorproto
        PUBLIC_HEADER DESTINATION include/lorproto)

add_executable(test_buffer test/buffer.c)
target_link_libraries(test_buffer lorproto)
add_test(buffer test_buffer)

add_executable(test_channel_format test/channel_format.c)
target_link_libraries(test_channel_format lorproto)
add_test(channel_format test_channel_format)

add_executable(test_compress test/compress.c)
target_link_libraries(test_compress lorproto)
add_test(compress test_compress)

add_executable(test_effect test/effect.c)
target_link_libraries(test_effect lorproto)
add_test(effect test_effect)

add_executable(test_intensity test/intensity.c)
target_link_libraries(test_intensity lorproto)
add_test(intensity test_intensity)

add_executable(test_model test/model.c)
target_link_libraries(test_model lorproto)
add_test(model test_model)

add_executable(test_time test/time.c)
target_link_libraries(test_time lorproto)
add_test(time test_time)

enable_testing()
