# ------------------------------
# Find all source files
# ------------------------------

file(GLOB LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Ensure main.cpp removed from lib sources
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# ------------------------------
# Define static library
# ------------------------------

add_library(${LIB_NAME} STATIC
        ${LIB_SOURCES}
)

# ------------------------------
# Add includes
# ------------------------------

target_include_directories(${LIB_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${GLAD_INCLUDE_DIR}"
)

# ------------------------------
# Define executable
# ------------------------------

add_executable(${EXEC_NAME}
        main.cpp
)

# ------------------------------
# Link lib to executable
# ------------------------------

find_package(OpenGL REQUIRED)

add_dependencies(${LIB_NAME} glad-generate-files)

target_link_libraries(${LIB_NAME}  PRIVATE
        OpenGL::GL
        glfw
        glad
)

target_link_libraries(${EXEC_NAME} PRIVATE ${LIB_NAME})
