#include <catch2/catch_test_macros.hpp>
#include "Task.h"

TEST_CASE("Task creation with correct values", "[task]") {
    Task task(1, "Buy groceries", "Milk, eggs, bread", Priority::HIGH, "2025-12-31");

    REQUIRE(task.getId() == 1);
    REQUIRE(task.getTitle() == "Buy groceries");
    REQUIRE(task.getDescription() == "Milk, eggs, bread");
    REQUIRE(task.getPriority() == Priority::HIGH);
    REQUIRE(task.getDueDate() == "2025-12-31");
    REQUIRE_FALSE(task.isCompleted());
}

TEST_CASE("Task modification", "[task]") {
    Task task(1, "Original", "Desc", Priority::LOW, "2025-01-01");

    SECTION("Mark as completed") {
        task.markCompleted();
        REQUIRE(task.isCompleted());
    }

    SECTION("Update title") {
        task.setTitle("Updated Title");
        REQUIRE(task.getTitle() == "Updated Title");
    }

    SECTION("Update description") {
        task.setDescription("New desc");
        REQUIRE(task.getDescription() == "New desc");
    }

    SECTION("Update priority") {
        task.setPriority(Priority::HIGH);
        REQUIRE(task.getPriority() == Priority::HIGH);
    }

    SECTION("Update due date") {
        task.setDueDate("2025-06-15");
        REQUIRE(task.getDueDate() == "2025-06-15");
    }
}

TEST_CASE("Priority to string conversion", "[task]") {
    Task low(1, "T", "D", Priority::LOW, "2025-01-01");
    Task med(2, "T", "D", Priority::MEDIUM, "2025-01-01");
    Task high(3, "T", "D", Priority::HIGH, "2025-01-01");

    REQUIRE(low.priorityToString() == "LOW");
    REQUIRE(med.priorityToString() == "MEDIUM");
    REQUIRE(high.priorityToString() == "HIGH");
}

TEST_CASE("String to priority conversion", "[task]") {
    REQUIRE(Task::stringToPriority("LOW") == Priority::LOW);
    REQUIRE(Task::stringToPriority("MEDIUM") == Priority::MEDIUM);
    REQUIRE(Task::stringToPriority("HIGH") == Priority::HIGH);
}

TEST_CASE("Invalid string to priority throws exception", "[task]") {
    REQUIRE_THROWS_AS(Task::stringToPriority("INVALID"), std::invalid_argument);
    REQUIRE_THROWS_AS(Task::stringToPriority(""), std::invalid_argument);
}
