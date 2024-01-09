set(CMAKE_CXX_STANDARD 23)

# Add the shader source files to the ShaderLibrary target
add_library(Object object.cpp object.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Object PRIVATE glad glm ShaderLibrary VertexBuffer VertexArray IndexBuffer Texture)

target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../primitives)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../shader)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Texture)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../vertex_buffer)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../index_buffer)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../vertex_array)

# You can set additional properties or dependencies for ShaderLibrary here if needed.
