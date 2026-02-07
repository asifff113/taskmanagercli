cmake_minimum_required(VERSION 3.14)
project(TaskManagerCLI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Avoid response files so clangd can parse compile_commands.json
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

# Static linking so executables run without external DLLs
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

include(FetchContent)

# ── nlohmann/json for robust serialization ──
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# ── Core library (shared by all executables) ──
add_library(task_core STATIC
    src/Task.cpp
    src/TaskManager.cpp
    src/Storage.cpp
)
target_include_directories(task_core PUBLIC include)
target_link_libraries(task_core PUBLIC nlohmann_json::nlohmann_json)

# ── CLI executable (simple menu) ──
add_executable(task_manager src/main.cpp)
target_link_libraries(task_manager PRIVATE task_core)

# ── FTXUI Terminal UI ──
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(task_manager_ui src/ui_main.cpp)
target_link_libraries(task_manager_ui PRIVATE task_core
    ftxui::screen ftxui::dom ftxui::component)

# ── Testing with Catch2 v3 ──
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    tests/test_task.cpp
    tests/test_task_manager.cpp
    tests/test_sort_strategies.cpp
    tests/test_filter_strategies.cpp
    tests/test_storage.cpp
)
target_link_libraries(tests PRIVATE task_core Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
