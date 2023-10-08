# Add the shader source files to the ShaderLibrary target
add_library(ShaderLibrary shader.cpp shader.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(ShaderLibrary PRIVATE glad glm)

target_include_directories(ShaderLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
