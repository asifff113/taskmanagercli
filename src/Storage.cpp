#include "Storage.h"
#include <fstream>
#include <sstream>

bool Storage::saveToFile(const std::vector<Task>& tasks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& task : tasks) {
        file << task.getId() << "|"
             << task.getTitle() << "|"
             << task.getDescription() << "|"
             << task.priorityToString() << "|"
             << task.getDueDate() << "|"
             << (task.isCompleted() ? "1" : "0") << "\n";
    }
    return true;
}

std::vector<Task> Storage::loadFromFile(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream file(filename);
    if (!file.is_open()) return tasks;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string idStr, title, desc, priority, dueDate, completedStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, title, '|');
        std::getline(ss, desc, '|');
        std::getline(ss, priority, '|');
        std::getline(ss, dueDate, '|');
        std::getline(ss, completedStr, '|');

        if (idStr.empty()) continue;

        Task task(std::stoi(idStr), title, desc,
                  Task::stringToPriority(priority), dueDate);
        if (completedStr == "1") task.markCompleted();
        tasks.push_back(task);
    }
    return tasks;
}
