#include <iostream>
#include <string>
#include <memory>
#include "TaskManager.h"
#include "SortStrategy.h"
#include "FilterStrategy.h"
#include "Storage.h"

const std::string DATA_FILE = "tasks.json";

void printMenu() {
    std::cout << "\n============================\n";
    std::cout << "   Task Manager CLI\n";
    std::cout << "============================\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. Remove Task\n";
    std::cout << "3. Mark Task Completed\n";
    std::cout << "4. List All Tasks\n";
    std::cout << "5. Sort Tasks\n";
    std::cout << "6. Filter Tasks\n";
    std::cout << "7. Save & Exit\n";
    std::cout << "============================\n";
    std::cout << "Choice: ";
}

void printTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "\nNo tasks found.\n";
        return;
    }
    std::cout << "\n" << std::string(70, '-') << "\n";
    std::cout << "ID  | Status | Priority | Due Date   | Title\n";
    std::cout << std::string(70, '-') << "\n";
    for (const auto& task : tasks) {
        std::cout << task.getId() << "   | "
                  << (task.isCompleted() ? "[x]" : "[ ]") << "    | "
                  << task.priorityToString() << "\t  | "
                  << task.getDueDate() << " | "
                  << task.getTitle() << "\n";
    }
    std::cout << std::string(70, '-') << "\n";
}

void addTask(TaskManager& manager) {
    std::string title, description, dueDate;
    int priority;

    std::cin.ignore();
    std::cout << "Title: ";
    std::getline(std::cin, title);
    std::cout << "Description: ";
    std::getline(std::cin, description);
    std::cout << "Priority (1=Low, 2=Medium, 3=High): ";
    std::cin >> priority;
    std::cin.ignore();
    std::cout << "Due Date (YYYY-MM-DD): ";
    std::getline(std::cin, dueDate);

    Priority p = Priority::MEDIUM;
    if (priority == 1) p = Priority::LOW;
    else if (priority == 3) p = Priority::HIGH;

    manager.addTask(title, description, p, dueDate);
    std::cout << "Task added successfully!\n";
}

void removeTask(TaskManager& manager) {
    int id;
    std::cout << "Enter task ID to remove: ";
    std::cin >> id;
    if (manager.removeTask(id)) {
        std::cout << "Task removed.\n";
    } else {
        std::cout << "Task not found.\n";
    }
}

void completeTask(TaskManager& manager) {
    int id;
    std::cout << "Enter task ID to mark completed: ";
    std::cin >> id;
    if (manager.markTaskCompleted(id)) {
        std::cout << "Task marked as completed.\n";
    } else {
        std::cout << "Task not found.\n";
    }
}

void sortTasks(TaskManager& manager) {
    std::cout << "Sort by:\n";
    std::cout << "1. Priority (High to Low)\n";
    std::cout << "2. Due Date (Earliest first)\n";
    std::cout << "3. Name (A-Z)\n";
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            manager.setSortStrategy(std::make_unique<SortByPriority>());
            break;
        case 2:
            manager.setSortStrategy(std::make_unique<SortByDueDate>());
            break;
        case 3:
            manager.setSortStrategy(std::make_unique<SortByName>());
            break;
        default:
            std::cout << "Invalid choice.\n";
            return;
    }
    manager.sortTasks();
    std::cout << "Tasks sorted.\n";
    printTasks(manager.getTasks());
}

void filterTasks(TaskManager& manager) {
    std::cout << "Filter by:\n";
    std::cout << "1. Completed tasks\n";
    std::cout << "2. Pending tasks\n";
    std::cout << "3. High priority\n";
    std::cout << "4. Medium priority\n";
    std::cout << "5. Low priority\n";
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            manager.setFilterStrategy(std::make_unique<FilterByStatus>(true));
            break;
        case 2:
            manager.setFilterStrategy(std::make_unique<FilterByStatus>(false));
            break;
        case 3:
            manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::HIGH));
            break;
        case 4:
            manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::MEDIUM));
            break;
        case 5:
            manager.setFilterStrategy(std::make_unique<FilterByPriority>(Priority::LOW));
            break;
        default:
            std::cout << "Invalid choice.\n";
            return;
    }
    auto filtered = manager.filterTasks();
    printTasks(filtered);
}

int main() {
    TaskManager manager;

    auto loaded = Storage::loadFromFile(DATA_FILE);
    if (!loaded.empty()) {
        manager.setTasks(loaded);
        std::cout << "Loaded " << loaded.size() << " tasks from file.\n";
    }

    int choice = 0;
    while (true) {
        printMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: addTask(manager); break;
            case 2: removeTask(manager); break;
            case 3: completeTask(manager); break;
            case 4: printTasks(manager.getTasks()); break;
            case 5: sortTasks(manager); break;
            case 6: filterTasks(manager); break;
            case 7:
                if (Storage::saveToFile(manager.getTasks(), DATA_FILE))
                    std::cout << "Tasks saved. Goodbye!\n";
                else
                    std::cerr << "Error: Failed to save tasks to " << DATA_FILE << "\n";
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
