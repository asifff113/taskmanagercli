#include "Task.h"

Task::Task(int id, const std::string& title, const std::string& description,
           Priority priority, const std::string& dueDate)
    : id_(id), title_(title), description_(description),
      priority_(priority), dueDate_(dueDate), completed_(false) {}

int Task::getId() const { return id_; }
std::string Task::getTitle() const { return title_; }
std::string Task::getDescription() const { return description_; }
Priority Task::getPriority() const { return priority_; }
std::string Task::getDueDate() const { return dueDate_; }
bool Task::isCompleted() const { return completed_; }

void Task::setTitle(const std::string& title) { title_ = title; }
void Task::setDescription(const std::string& description) { description_ = description; }
void Task::setPriority(Priority priority) { priority_ = priority; }
void Task::setDueDate(const std::string& dueDate) { dueDate_ = dueDate; }
void Task::markCompleted() { completed_ = true; }

std::string Task::priorityToString() const {
    switch (priority_) {
        case Priority::LOW:    return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH:   return "HIGH";
        default:               return "UNKNOWN";
    }
}

Priority Task::stringToPriority(const std::string& str) {
    if (str == "LOW")    return Priority::LOW;
    if (str == "MEDIUM") return Priority::MEDIUM;
    if (str == "HIGH")   return Priority::HIGH;
    throw std::invalid_argument("Invalid priority: " + str);
}
