cmake_minimum_required(VERSION 3.16)

project(lightorama
        VERSION 1.0.0
        DESCRIPTION "C99 library implementing the Light-O-Rama \(LOR\) communication protocol"
        HOMEPAGE_URL "https://github.com/Cryptkeeper/liblightorama"
        LANGUAGES C)

set(CMAKE_C_STANDARD 90)

add_library(lightorama protocol.h protocol.c io.c io.h model.h brightness_curve.c brightness_curve.h math.c math.h)

set_target_properties(lightorama PROPERTIES PUBLIC_HEADER "protocol.h;io.h;model.h;brightness_curve.h")

install(TARGETS lightorama
        LIBRARY DESTINATION lib/lightorama
        PUBLIC_HEADER DESTINATION include/lightorama)