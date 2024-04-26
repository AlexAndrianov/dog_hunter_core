# CMakeLists.txt for core

find_package(Qt6 6.2 REQUIRED COMPONENTS Core)

# Define library sources and headers
set(LIB_SOURCES
    src/model.cpp
    src/RequestHandler.cpp
    # Add more source files as needed
)

set(LIB_HEADERS
    include/model.h
    include/RequestHandler.h
    # Add more header files as needed
)

# Create the library target
add_library(core ${LIB_SOURCES} ${LIB_HEADERS})

target_link_libraries(core PRIVATE
    Qt6::Core
)

# Specify include directories for the library
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify compile features or requirements for your library
target_compile_features(core PUBLIC cxx_std_23)
