set(CMAKE_CXX_STANDARD 23)

# Add the shader source files to the ShaderLibrary target
add_library(IndexBuffer index_buffer.cpp index_buffer.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(IndexBuffer PRIVATE glad)

target_include_directories(IndexBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
