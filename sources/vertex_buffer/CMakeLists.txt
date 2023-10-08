# Add the shader source files to the ShaderLibrary target
add_library(VertexBuffer vertex_buffer.cpp vertex_buffer.h)

target_sources(VertexBuffer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../primitives/primitives.h")

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(VertexBuffer PRIVATE glad glm)

target_include_directories(VertexBuffer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../primitives")
target_include_directories(VertexBuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_link_libraries(VertexBuffer PUBLIC primitives)

# You can set additional properties or dependencies for ShaderLibrary here if needed.