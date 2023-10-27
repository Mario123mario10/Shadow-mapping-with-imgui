# Add the source files of my special light class :) to the ShaderLibrary target
add_library(Light light.cpp light.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Light PRIVATE glad glm)

target_include_directories(Light PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for LightLibrary here if needed.
