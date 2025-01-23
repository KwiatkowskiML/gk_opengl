message(STATUS "glfw fetcher cmake loaded...")

# -------------------------------
# Fetch glfw from github...
# -------------------------------

include(FetchContent)

FetchContent_Declare(
        libGLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
)

FetchContent_MakeAvailable(libGLFW)
