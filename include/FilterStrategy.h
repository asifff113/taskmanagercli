#pragma once
#include <vector>
#include <string>
#include "Task.h"

// Strategy interface for filtering tasks
class FilterStrategy {
public:
    virtual ~FilterStrategy() = default;
    virtual std::vector<Task> filter(const std::vector<Task>& tasks) const = 0;
    virtual std::string getName() const = 0;
};

// Filters tasks by completion status
class FilterByStatus : public FilterStrategy {
public:
    explicit FilterByStatus(bool completed) : completed_(completed) {}

    std::vector<Task> filter(const std::vector<Task>& tasks) const override {
        std::vector<Task> result;
        for (const auto& task : tasks) {
            if (task.isCompleted() == completed_) {
                result.push_back(task);
            }
        }
        return result;
    }

    std::string getName() const override {
        return completed_ ? "Completed" : "Pending";
    }

private:
    bool completed_;
};

// Filters tasks by priority level
class FilterByPriority : public FilterStrategy {
public:
    explicit FilterByPriority(Priority priority) : priority_(priority) {}

    std::vector<Task> filter(const std::vector<Task>& tasks) const override {
        std::vector<Task> result;
        for (const auto& task : tasks) {
            if (task.getPriority() == priority_) {
                result.push_back(task);
            }
        }
        return result;
    }

    std::string getName() const override {
        switch (priority_) {
            case Priority::LOW:    return "Low Priority";
            case Priority::MEDIUM: return "Medium Priority";
            case Priority::HIGH:   return "High Priority";
            default:               return "Unknown";
        }
    }

private:
    Priority priority_;
};
