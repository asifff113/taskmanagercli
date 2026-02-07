#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include "Task.h"

// Strategy interface for sorting tasks
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<Task>& tasks) = 0;
    virtual std::string getName() const = 0;
};

// Sorts tasks from HIGH to LOW priority
class SortByPriority : public SortStrategy {
public:
    void sort(std::vector<Task>& tasks) override {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
        });
    }
    std::string getName() const override { return "Priority"; }
};

// Sorts tasks by due date (earliest first)
class SortByDueDate : public SortStrategy {
public:
    void sort(std::vector<Task>& tasks) override {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.getDueDate() < b.getDueDate();
        });
    }
    std::string getName() const override { return "Due Date"; }
};

// Sorts tasks alphabetically by title
class SortByName : public SortStrategy {
public:
    void sort(std::vector<Task>& tasks) override {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.getTitle() < b.getTitle();
        });
    }
    std::string getName() const override { return "Name"; }
};
