# ------------------------------
# Find all source files
# ------------------------------

file(GLOB_RECURSE LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

message(STATUS "LIB_SOURCES=${LIB_SOURCES}")

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
        libStb
        assimp
        imgui
        glm-header-only
)

target_link_libraries(${EXEC_NAME} PRIVATE ${LIB_NAME})

# -----------------------------
# Copy resources
# -----------------------------

add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources
    COMMENT "Copying shaders")

add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/src/shaders ${PROJECT_BINARY_DIR}/src/shaders
    COMMENT "Copying models")

if(WIN32)
    # Copy GLFW DLL
    add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:glfw>"
            "$<TARGET_FILE_DIR:${EXEC_NAME}>"
        COMMENT "Copying glfw DLL"
    )

    # Copy Assimp DLL
    add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:assimp>"
            "$<TARGET_FILE_DIR:${EXEC_NAME}>"
        COMMENT "Copying assimp DLL"
    )

    # Copy Glad DLL (if it is built as a shared library)
    add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:glad>"
            "$<TARGET_FILE_DIR:${EXEC_NAME}>"
        COMMENT "Copying glad DLL"
    )
endif()
