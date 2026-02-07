#include <catch2/catch_test_macros.hpp>
#include "TaskManager.h"
#include "SortStrategy.h"
#include <memory>

TEST_CASE("Sort by priority (high to low)", "[sort][strategy]") {
    TaskManager manager;
    manager.addTask("Low task", "D", Priority::LOW, "2025-01-01");
    manager.addTask("High task", "D", Priority::HIGH, "2025-01-01");
    manager.addTask("Medium task", "D", Priority::MEDIUM, "2025-01-01");

    manager.setSortStrategy(std::make_unique<SortByPriority>());
    manager.sortTasks();

    auto& tasks = manager.getTasks();
    REQUIRE(tasks[0].getPriority() == Priority::HIGH);
    REQUIRE(tasks[1].getPriority() == Priority::MEDIUM);
    REQUIRE(tasks[2].getPriority() == Priority::LOW);
}

TEST_CASE("Sort by due date (earliest first)", "[sort][strategy]") {
    TaskManager manager;
    manager.addTask("Later", "D", Priority::LOW, "2025-12-31");
    manager.addTask("Earlier", "D", Priority::LOW, "2025-01-15");
    manager.addTask("Middle", "D", Priority::LOW, "2025-06-01");

    manager.setSortStrategy(std::make_unique<SortByDueDate>());
    manager.sortTasks();

    auto& tasks = manager.getTasks();
    REQUIRE(tasks[0].getDueDate() == "2025-01-15");
    REQUIRE(tasks[1].getDueDate() == "2025-06-01");
    REQUIRE(tasks[2].getDueDate() == "2025-12-31");
}

TEST_CASE("Sort by name (alphabetical)", "[sort][strategy]") {
    TaskManager manager;
    manager.addTask("Charlie", "D", Priority::LOW, "2025-01-01");
    manager.addTask("Alpha", "D", Priority::LOW, "2025-01-01");
    manager.addTask("Bravo", "D", Priority::LOW, "2025-01-01");

    manager.setSortStrategy(std::make_unique<SortByName>());
    manager.sortTasks();

    auto& tasks = manager.getTasks();
    REQUIRE(tasks[0].getTitle() == "Alpha");
    REQUIRE(tasks[1].getTitle() == "Bravo");
    REQUIRE(tasks[2].getTitle() == "Charlie");
}

TEST_CASE("Sort strategy getName returns correct label", "[sort][strategy]") {
    SortByPriority sp;
    SortByDueDate sd;
    SortByName sn;

    REQUIRE(sp.getName() == "Priority");
    REQUIRE(sd.getName() == "Due Date");
    REQUIRE(sn.getName() == "Name");
}

TEST_CASE("Sort without strategy set does nothing", "[sort][strategy]") {
    TaskManager manager;
    manager.addTask("B", "D", Priority::LOW, "2025-01-01");
    manager.addTask("A", "D", Priority::LOW, "2025-01-01");

    manager.sortTasks(); // no strategy set

    REQUIRE(manager.getTasks()[0].getTitle() == "B");
    REQUIRE(manager.getTasks()[1].getTitle() == "A");
}

TEST_CASE("Switching sort strategies", "[sort][strategy]") {
    TaskManager manager;
    manager.addTask("Zebra", "D", Priority::LOW, "2025-12-01");
    manager.addTask("Apple", "D", Priority::HIGH, "2025-01-01");

    manager.setSortStrategy(std::make_unique<SortByName>());
    manager.sortTasks();
    REQUIRE(manager.getTasks()[0].getTitle() == "Apple");

    manager.setSortStrategy(std::make_unique<SortByPriority>());
    manager.sortTasks();
    REQUIRE(manager.getTasks()[0].getTitle() == "Apple"); // HIGH priority comes first
    REQUIRE(manager.getTasks()[0].getPriority() == Priority::HIGH);
}
