#include <catch2/catch_test_macros.hpp>
#include "TaskManager.h"

TEST_CASE("Add and retrieve tasks", "[manager]") {
    TaskManager manager;
    manager.addTask("Task 1", "Desc 1", Priority::HIGH, "2025-12-01");
    manager.addTask("Task 2", "Desc 2", Priority::LOW, "2025-12-15");

    REQUIRE(manager.getTasks().size() == 2);
    REQUIRE(manager.getTasks()[0].getTitle() == "Task 1");
    REQUIRE(manager.getTasks()[1].getTitle() == "Task 2");
}

TEST_CASE("Remove existing task", "[manager]") {
    TaskManager manager;
    manager.addTask("Task 1", "Desc", Priority::MEDIUM, "2025-01-01");
    manager.addTask("Task 2", "Desc", Priority::MEDIUM, "2025-01-01");

    REQUIRE(manager.removeTask(1));
    REQUIRE(manager.getTasks().size() == 1);
    REQUIRE(manager.getTasks()[0].getTitle() == "Task 2");
}

TEST_CASE("Remove non-existing task returns false", "[manager]") {
    TaskManager manager;
    manager.addTask("Task 1", "Desc", Priority::MEDIUM, "2025-01-01");

    REQUIRE_FALSE(manager.removeTask(99));
    REQUIRE(manager.getTasks().size() == 1);
}

TEST_CASE("Mark task as completed", "[manager]") {
    TaskManager manager;
    manager.addTask("Task 1", "Desc", Priority::HIGH, "2025-06-01");

    REQUIRE(manager.markTaskCompleted(1));
    REQUIRE(manager.findTask(1)->isCompleted());
}

TEST_CASE("Mark non-existing task returns false", "[manager]") {
    TaskManager manager;
    REQUIRE_FALSE(manager.markTaskCompleted(99));
}

TEST_CASE("Find task by ID", "[manager]") {
    TaskManager manager;
    manager.addTask("Find Me", "Desc", Priority::LOW, "2025-03-01");

    REQUIRE(manager.findTask(1) != nullptr);
    REQUIRE(manager.findTask(1)->getTitle() == "Find Me");
    REQUIRE(manager.findTask(99) == nullptr);
}

TEST_CASE("Task ID auto-increments", "[manager]") {
    TaskManager manager;
    manager.addTask("Task 1", "D", Priority::LOW, "2025-01-01");
    manager.addTask("Task 2", "D", Priority::LOW, "2025-01-01");
    manager.addTask("Task 3", "D", Priority::LOW, "2025-01-01");

    REQUIRE(manager.getTasks()[0].getId() == 1);
    REQUIRE(manager.getTasks()[1].getId() == 2);
    REQUIRE(manager.getTasks()[2].getId() == 3);
    REQUIRE(manager.getNextId() == 4);
}

TEST_CASE("setTasks restores tasks and recalculates next ID", "[manager]") {
    TaskManager manager;

    std::vector<Task> tasks;
    tasks.emplace_back(5, "Restored", "D", Priority::HIGH, "2025-01-01");
    tasks.emplace_back(10, "Another", "D", Priority::LOW, "2025-02-01");
    manager.setTasks(tasks);

    REQUIRE(manager.getTasks().size() == 2);
    REQUIRE(manager.getNextId() == 11);
}
