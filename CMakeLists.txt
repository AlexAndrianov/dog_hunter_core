# CMakeLists.txt for core

# Define library sources and headers
set(LIB_SOURCES
    src/model.cpp
    # Add more source files as needed
)

set(LIB_HEADERS
    include/model.h
    # Add more header files as needed
)

# Create the library target
add_library(core ${LIB_SOURCES} ${LIB_HEADERS})

# Specify include directories for the library
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
