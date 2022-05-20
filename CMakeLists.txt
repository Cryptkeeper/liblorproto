cmake_minimum_required(VERSION 3.16)

project(lightorama
        VERSION 2.0.0
        DESCRIPTION "C90 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblightorama"
        LANGUAGES C)

set(CMAKE_C_STANDARD 90)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif ()

add_library(lightorama
        src/effect.c
        src/heartbeat.c
        src/intensity.c
        src/time.c
        src/uid.c)

set_target_properties(lightorama PROPERTIES PUBLIC_HEADER "include/effect.h;include/heartbeat.h;include/intensity.h;include/liblightorama.h;include/model.h;include/platform.h;include/time.h;include/uid.h;include/version.h;")

install(TARGETS lightorama
        LIBRARY DESTINATION lib/lightorama
        PUBLIC_HEADER DESTINATION include/lightorama)

add_executable(test_duration test/time.c)
target_link_libraries(test_duration lightorama)
add_test(duration test_duration)

enable_testing()
