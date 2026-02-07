#pragma once
#include <string>
#include <stdexcept>

enum class Priority { LOW, MEDIUM, HIGH };

class Task {
public:
    Task(int id, const std::string& title, const std::string& description,
         Priority priority, const std::string& dueDate);

    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    Priority getPriority() const;
    std::string getDueDate() const;
    bool isCompleted() const;

    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setPriority(Priority priority);
    void setDueDate(const std::string& dueDate);
    void markCompleted();

    std::string priorityToString() const;
    static Priority stringToPriority(const std::string& str);

private:
    int id_;
    std::string title_;
    std::string description_;
    Priority priority_;
    std::string dueDate_;
    bool completed_;
};
