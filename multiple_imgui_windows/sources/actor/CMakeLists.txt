set(CMAKE_CXX_STANDARD 23)

# Add the source files of my special light class :) to the ShaderLibrary target
add_library(Actor actor.cpp actor.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Actor PRIVATE glad glm)
target_link_libraries(Actor PUBLIC Object Light ShaderLibrary)

target_include_directories(Actor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for LightLibrary here if needed.
