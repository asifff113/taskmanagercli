#include <catch2/catch_test_macros.hpp>
#include "TaskManager.h"
#include "FilterStrategy.h"
#include <memory>

TEST_CASE("Filter completed tasks", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("Task 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Task 2", "D", Priority::LOW, "2025-01-01");
    manager.addTask("Task 3", "D", Priority::MEDIUM, "2025-01-01");
    manager.markTaskCompleted(1);

    manager.setFilterStrategy(std::make_unique<FilterByStatus>(true));
    auto result = manager.filterTasks();

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].getTitle() == "Task 1");
}

TEST_CASE("Filter pending tasks", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("Task 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Task 2", "D", Priority::LOW, "2025-01-01");
    manager.markTaskCompleted(1);

    manager.setFilterStrategy(std::make_unique<FilterByStatus>(false));
    auto result = manager.filterTasks();

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].getTitle() == "Task 2");
}

TEST_CASE("Filter by high priority", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("High 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Low 1", "D", Priority::LOW, "2025-01-01");
    manager.addTask("High 2", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Medium 1", "D", Priority::MEDIUM, "2025-01-01");

    manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::HIGH));
    auto result = manager.filterTasks();

    REQUIRE(result.size() == 2);
    REQUIRE(result[0].getTitle() == "High 1");
    REQUIRE(result[1].getTitle() == "High 2");
}

TEST_CASE("Filter by low priority", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("High 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Low 1", "D", Priority::LOW, "2025-01-01");

    manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::LOW));
    auto result = manager.filterTasks();

    REQUIRE(result.size() == 1);
    REQUIRE(result[0].getTitle() == "Low 1");
}

TEST_CASE("Filter by medium priority", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("Med 1", "D", Priority::MEDIUM, "2025-01-01");
    manager.addTask("High 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Med 2", "D", Priority::MEDIUM, "2025-01-01");

    manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::MEDIUM));
    auto result = manager.filterTasks();

    REQUIRE(result.size() == 2);
}

TEST_CASE("Filter without strategy returns all tasks", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("Task 1", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Task 2", "D", Priority::LOW, "2025-01-01");

    auto result = manager.filterTasks();
    REQUIRE(result.size() == 2);
}

TEST_CASE("Filter strategy getName returns correct label", "[filter][strategy]") {
    FilterByStatus completed(true);
    FilterByStatus pending(false);
    FilterByPriority high(Priority::HIGH);
    FilterByPriority med(Priority::MEDIUM);
    FilterByPriority low(Priority::LOW);

    REQUIRE(completed.getName() == "Completed");
    REQUIRE(pending.getName() == "Pending");
    REQUIRE(high.getName() == "High Priority");
    REQUIRE(med.getName() == "Medium Priority");
    REQUIRE(low.getName() == "Low Priority");
}

TEST_CASE("Filter returns empty when no tasks match", "[filter][strategy]") {
    TaskManager manager;
    manager.addTask("Low 1", "D", Priority::LOW, "2025-01-01");

    manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::HIGH));
    auto result = manager.filterTasks();

    REQUIRE(result.empty());
}
