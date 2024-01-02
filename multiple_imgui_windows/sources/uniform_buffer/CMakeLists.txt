set(CMAKE_CXX_STANDARD 23)

# Add the shader source files to the ShaderLibrary target
add_library(UniformBuffer uniform_buffer.cpp uniform_buffer.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(UniformBuffer PRIVATE glad glm ShaderLibrary)

target_include_directories(UniformBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
