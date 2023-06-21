cmake_minimum_required(VERSION 3.16)

project(lightorama
        VERSION 2.6.1
        DESCRIPTION "C90 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblightorama"
        LANGUAGES C)

set(CMAKE_C_STANDARD 90)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif ()

file(GLOB_RECURSE PUBLIC_HEADERS "include/lightorama/*.h")

add_library(lightorama
        ${PUBLIC_HEADERS}
        src/easy.c
        src/effect.c
        src/heartbeat.c
        src/intensity.c
        src/model.c
        src/time.c
        src/uid.c)

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

add_executable(test_channelset test/channelset.c)
target_link_libraries(test_channelset lightorama)
add_test(channelset test_channelset)

add_executable(test_effect test/effect.c)
target_link_libraries(test_effect lightorama)
add_test(effect test_effect)

add_executable(test_model test/model.c)
target_link_libraries(test_model lightorama)
add_test(model test_model)

add_executable(test_time test/time.c)
target_link_libraries(test_time lightorama)
add_test(time test_time)

enable_testing()
