#pragma once
#include <string>
#include <vector>
#include "Task.h"

class Storage {
public:
    static bool saveToFile(const std::vector<Task>& tasks, const std::string& filename);
    static std::vector<Task> loadFromFile(const std::string& filename);
};
