#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Task.h"
#include "SortStrategy.h"
#include "FilterStrategy.h"

class TaskManager {
public:
    TaskManager();

    void addTask(const std::string& title, const std::string& description,
                 Priority priority, const std::string& dueDate);
    bool removeTask(int id);
    bool markTaskCompleted(int id);

    Task* findTask(int id);
    const std::vector<Task>& getTasks() const;

    void setSortStrategy(std::unique_ptr<SortStrategy> strategy);
    void setFilterStrategy(std::unique_ptr<FilterStrategy> strategy);

    void sortTasks();
    std::vector<Task> filterTasks() const;

    void setTasks(const std::vector<Task>& tasks);
    int getNextId() const;

private:
    std::vector<Task> tasks_;
    int nextId_;
    std::unique_ptr<SortStrategy> sortStrategy_;
    std::unique_ptr<FilterStrategy> filterStrategy_;
};
