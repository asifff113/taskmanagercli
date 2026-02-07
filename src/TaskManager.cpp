#include "TaskManager.h"

TaskManager::TaskManager() : nextId_(1) {}

void TaskManager::addTask(const std::string& title, const std::string& description,
                           Priority priority, const std::string& dueDate) {
    tasks_.emplace_back(nextId_++, title, description, priority, dueDate);
}

bool TaskManager::removeTask(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
        [id](const Task& t) { return t.getId() == id; });
    if (it != tasks_.end()) {
        tasks_.erase(it);
        return true;
    }
    return false;
}

bool TaskManager::markTaskCompleted(int id) {
    Task* task = findTask(id);
    if (task) {
        task->markCompleted();
        return true;
    }
    return false;
}

Task* TaskManager::findTask(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
        [id](const Task& t) { return t.getId() == id; });
    if (it != tasks_.end()) {
        return &(*it);
    }
    return nullptr;
}

const Task* TaskManager::findTask(int id) const {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
        [id](const Task& t) { return t.getId() == id; });
    if (it != tasks_.end()) {
        return &(*it);
    }
    return nullptr;
}

const std::vector<Task>& TaskManager::getTasks() const {
    return tasks_;
}

void TaskManager::setSortStrategy(std::unique_ptr<SortStrategy> strategy) {
    sortStrategy_ = std::move(strategy);
}

void TaskManager::setFilterStrategy(std::unique_ptr<FilterStrategy> strategy) {
    filterStrategy_ = std::move(strategy);
}

void TaskManager::sortTasks() {
    if (sortStrategy_) {
        sortStrategy_->sort(tasks_);
    }
}

std::vector<Task> TaskManager::filterTasks() const {
    if (filterStrategy_) {
        return filterStrategy_->filter(tasks_);
    }
    return tasks_;
}

void TaskManager::setTasks(const std::vector<Task>& tasks) {
    tasks_ = tasks;
    nextId_ = 1;
    for (const auto& task : tasks_) {
        if (task.getId() >= nextId_) {
            nextId_ = task.getId() + 1;
        }
    }
}

int TaskManager::getNextId() const {
    return nextId_;
}
