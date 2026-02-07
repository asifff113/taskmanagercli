#include <catch2/catch_test_macros.hpp>
#include "Storage.h"
#include <cstdio>
#include <fstream>

TEST_CASE("Save and load tasks round-trip", "[storage]") {
    const std::string testFile = "test_tasks_temp.json";

    std::vector<Task> tasks;
    tasks.emplace_back(1, "Task 1", "Description 1", Priority::HIGH, "2025-06-15");
    tasks.emplace_back(2, "Task 2", "Description 2", Priority::LOW, "2025-12-01");
    tasks[0].markCompleted();

    REQUIRE(Storage::saveToFile(tasks, testFile));

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.size() == 2);

    REQUIRE(loaded[0].getId() == 1);
    REQUIRE(loaded[0].getTitle() == "Task 1");
    REQUIRE(loaded[0].getDescription() == "Description 1");
    REQUIRE(loaded[0].getPriority() == Priority::HIGH);
    REQUIRE(loaded[0].getDueDate() == "2025-06-15");
    REQUIRE(loaded[0].isCompleted());

    REQUIRE(loaded[1].getId() == 2);
    REQUIRE(loaded[1].getTitle() == "Task 2");
    REQUIRE(loaded[1].getPriority() == Priority::LOW);
    REQUIRE_FALSE(loaded[1].isCompleted());

    std::remove(testFile.c_str());
}

TEST_CASE("Load from non-existent file returns empty", "[storage]") {
    auto tasks = Storage::loadFromFile("this_file_does_not_exist_12345.json");
    REQUIRE(tasks.empty());
}

TEST_CASE("Save empty task list", "[storage]") {
    const std::string testFile = "test_empty_temp.json";

    std::vector<Task> empty;
    REQUIRE(Storage::saveToFile(empty, testFile));

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.empty());

    std::remove(testFile.c_str());
}

TEST_CASE("Save and load preserves all priority levels", "[storage]") {
    const std::string testFile = "test_priorities_temp.json";

    std::vector<Task> tasks;
    tasks.emplace_back(1, "Low", "D", Priority::LOW, "2025-01-01");
    tasks.emplace_back(2, "Med", "D", Priority::MEDIUM, "2025-01-01");
    tasks.emplace_back(3, "High", "D", Priority::HIGH, "2025-01-01");

    Storage::saveToFile(tasks, testFile);
    auto loaded = Storage::loadFromFile(testFile);

    REQUIRE(loaded[0].getPriority() == Priority::LOW);
    REQUIRE(loaded[1].getPriority() == Priority::MEDIUM);
    REQUIRE(loaded[2].getPriority() == Priority::HIGH);

    std::remove(testFile.c_str());
}

// ── Edge Case Tests for Robust Storage ──

TEST_CASE("Corrupted JSON file returns empty gracefully", "[storage][edge]") {
    const std::string testFile = "test_corrupt_temp.json";

    std::ofstream f(testFile);
    f << "{ this is not valid json!!!";
    f.close();

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.empty());

    std::remove(testFile.c_str());
}

TEST_CASE("JSON with missing fields skips bad entries", "[storage][edge]") {
    const std::string testFile = "test_missing_fields.json";

    std::ofstream f(testFile);
    f << R"([
        {"id": 1, "title": "Valid", "description": "D", "priority": "HIGH", "dueDate": "2025-01-01", "completed": false},
        {"id": 2, "title": "Missing priority"},
        {"id": 3, "title": "Also Valid", "description": "D", "priority": "LOW", "dueDate": "2025-06-01", "completed": true}
    ])";
    f.close();

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.size() == 2);
    REQUIRE(loaded[0].getTitle() == "Valid");
    REQUIRE(loaded[1].getTitle() == "Also Valid");

    std::remove(testFile.c_str());
}

TEST_CASE("Titles with special characters survive round-trip", "[storage][edge]") {
    const std::string testFile = "test_special_chars.json";

    std::vector<Task> tasks;
    tasks.emplace_back(1, "Task with | pipe", "Desc with \"quotes\"", Priority::HIGH, "2025-01-01");
    tasks.emplace_back(2, "Task with\nnewline", "Line1\nLine2", Priority::LOW, "2025-06-01");

    Storage::saveToFile(tasks, testFile);
    auto loaded = Storage::loadFromFile(testFile);

    REQUIRE(loaded.size() == 2);
    REQUIRE(loaded[0].getTitle() == "Task with | pipe");
    REQUIRE(loaded[0].getDescription() == "Desc with \"quotes\"");
    REQUIRE(loaded[1].getTitle() == "Task with\nnewline");

    std::remove(testFile.c_str());
}

TEST_CASE("Non-array JSON returns empty", "[storage][edge]") {
    const std::string testFile = "test_notarray.json";

    std::ofstream f(testFile);
    f << R"({"key": "value"})";
    f.close();

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.empty());

    std::remove(testFile.c_str());
}

TEST_CASE("Empty JSON array loads successfully", "[storage][edge]") {
    const std::string testFile = "test_emptyarray.json";

    std::ofstream f(testFile);
    f << "[]";
    f.close();

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.empty());

    std::remove(testFile.c_str());
}
