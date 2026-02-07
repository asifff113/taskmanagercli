#include "Storage.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool Storage::saveToFile(const std::vector<Task>& tasks, const std::string& filename) {
    json j = json::array();
    for (const auto& task : tasks) {
        j.push_back({
            {"id",          task.getId()},
            {"title",       task.getTitle()},
            {"description", task.getDescription()},
            {"priority",    task.priorityToString()},
            {"dueDate",     task.getDueDate()},
            {"completed",   task.isCompleted()}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << j.dump(2);  // pretty-print with 2-space indent
    return file.good();
}

std::vector<Task> Storage::loadFromFile(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream file(filename);
    if (!file.is_open()) return tasks;

    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "[Storage] JSON parse error: " << e.what() << std::endl;
        return tasks;
    }

    if (!j.is_array()) return tasks;

    for (const auto& item : j) {
        try {
            int id = item.at("id").get<int>();
            std::string title = item.at("title").get<std::string>();
            std::string desc  = item.at("description").get<std::string>();
            std::string prio  = item.at("priority").get<std::string>();
            std::string date  = item.at("dueDate").get<std::string>();
            bool done         = item.at("completed").get<bool>();

            Task task(id, title, desc, Task::stringToPriority(prio), date);
            if (done) task.markCompleted();
            tasks.push_back(task);
        } catch (const std::exception& e) {
            std::cerr << "[Storage] Skipping invalid task: " << e.what() << std::endl;
            continue;  // skip corrupted entries, keep loading others
        }
    }
    return tasks;
}
