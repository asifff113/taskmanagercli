#include <catch2/catch_test_macros.hpp>
#include "Storage.h"
#include <cstdio>

TEST_CASE("Save and load tasks round-trip", "[storage]") {
    const std::string testFile = "test_tasks_temp.dat";

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
    auto tasks = Storage::loadFromFile("this_file_does_not_exist_12345.dat");
    REQUIRE(tasks.empty());
}

TEST_CASE("Save empty task list", "[storage]") {
    const std::string testFile = "test_empty_temp.dat";

    std::vector<Task> empty;
    REQUIRE(Storage::saveToFile(empty, testFile));

    auto loaded = Storage::loadFromFile(testFile);
    REQUIRE(loaded.empty());

    std::remove(testFile.c_str());
}

TEST_CASE("Save and load preserves all priority levels", "[storage]") {
    const std::string testFile = "test_priorities_temp.dat";

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
