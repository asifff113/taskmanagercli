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

// ── Input Validation Edge Cases ──

TEST_CASE("Empty title throws exception", "[task][validation]") {
    REQUIRE_THROWS_AS(
        Task(1, "", "Desc", Priority::LOW, "2025-01-01"),
        std::invalid_argument
    );
}

TEST_CASE("setTitle with empty string throws exception", "[task][validation]") {
    Task task(1, "Valid", "Desc", Priority::LOW, "2025-01-01");
    REQUIRE_THROWS_AS(task.setTitle(""), std::invalid_argument);
}

TEST_CASE("Invalid date format throws exception", "[task][validation]") {
    REQUIRE_THROWS_AS(
        Task(1, "T", "D", Priority::LOW, "not-a-date"),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(
        Task(1, "T", "D", Priority::LOW, "2025-13-01"),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(
        Task(1, "T", "D", Priority::LOW, "2025-00-15"),
        std::invalid_argument
    );
    REQUIRE_THROWS_AS(
        Task(1, "T", "D", Priority::LOW, "2025-01-32"),
        std::invalid_argument
    );
}

TEST_CASE("Valid date formats are accepted", "[task][validation]") {
    REQUIRE_NOTHROW(Task(1, "T", "D", Priority::LOW, "2025-01-01"));
    REQUIRE_NOTHROW(Task(2, "T", "D", Priority::LOW, "2025-12-31"));
    REQUIRE_NOTHROW(Task(3, "T", "D", Priority::LOW, "N/A"));
    REQUIRE_NOTHROW(Task(4, "T", "D", Priority::LOW, ""));
}

TEST_CASE("setDueDate with invalid format throws", "[task][validation]") {
    Task task(1, "T", "D", Priority::LOW, "2025-01-01");
    REQUIRE_THROWS_AS(task.setDueDate("bad-date"), std::invalid_argument);
    REQUIRE_NOTHROW(task.setDueDate("2025-06-15"));
    REQUIRE_NOTHROW(task.setDueDate("N/A"));
}

TEST_CASE("isValidDate correctly validates dates", "[task][validation]") {
    REQUIRE(Task::isValidDate("2025-01-01"));
    REQUIRE(Task::isValidDate("2025-12-31"));
    REQUIRE_FALSE(Task::isValidDate("2025-13-01"));
    REQUIRE_FALSE(Task::isValidDate("25-01-01"));
    REQUIRE_FALSE(Task::isValidDate("not-a-date"));
    REQUIRE_FALSE(Task::isValidDate(""));
}
