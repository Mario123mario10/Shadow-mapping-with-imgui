# Add the shader source files to the ShaderLibrary target
add_library(VertexArray vertex_array.cpp vertex_array.h)

target_link_libraries(VertexArray PRIVATE glad glm VertexBuffer)

# Link ShaderLibrary with Glad and other dependencies
target_include_directories(VertexArray PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../vertex_buffer")
target_include_directories(VertexArray PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../primitives")
target_include_directories(VertexArray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
