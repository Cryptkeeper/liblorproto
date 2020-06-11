cmake_minimum_required(VERSION 3.16)

project(lightorama
        VERSION 1.0.0
        DESCRIPTION "C99 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblightorama"
        LANGUAGES C)

set(CMAKE_C_STANDARD 90)

add_library(lightorama include/protocol.h src/protocol.c src/io.c include/io.h include/model.h src/brightness_curve.c include/brightness_curve.h src/math.c src/math.h)

set_target_properties(lightorama PROPERTIES PUBLIC_HEADER "include/protocol.h;include/io.h;include/model.h;include/brightness_curve.h")

install(TARGETS lightorama
        LIBRARY DESTINATION lib/lightorama
        PUBLIC_HEADER DESTINATION include/lightorama)