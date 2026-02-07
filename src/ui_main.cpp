#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"

#include "TaskManager.h"
#include "SortStrategy.h"
#include "FilterStrategy.h"
#include "Storage.h"

using namespace ftxui;

const std::string DATA_FILE = "tasks.json";

// ── Helper: build a table element from a task list ──
Element TaskTable(const std::vector<::Task>& tasks) {
    if (tasks.empty()) {
        return text("  No tasks found.") | dim;
    }

    std::vector<std::vector<std::string>> rows;
    rows.push_back({"ID", "Status", "Priority", "Due Date", "Title"});
    for (const auto& t : tasks) {
        rows.push_back({
            std::to_string(t.getId()),
            t.isCompleted() ? "[x]" : "[ ]",
            t.priorityToString(),
            t.getDueDate(),
            t.getTitle(),
        });
    }

    auto table = Table(rows);
    table.SelectAll().Border(LIGHT);
    table.SelectRow(0).Decorate(bold);
    table.SelectRow(0).SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(DOUBLE);

    for (int i = 1; i < (int)rows.size(); i++) {
        if (rows[i][2] == "HIGH") {
            table.SelectCell(2, i).Decorate(color(Color::Red));
        } else if (rows[i][2] == "MEDIUM") {
            table.SelectCell(2, i).Decorate(color(Color::Yellow));
        } else {
            table.SelectCell(2, i).Decorate(color(Color::Green));
        }
        if (rows[i][1] == "[x]") {
            table.SelectCell(1, i).Decorate(color(Color::Green));
        }
    }

    return table.Render();
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    TaskManager manager;

    // Load existing tasks
    auto loaded = Storage::loadFromFile(DATA_FILE);
    if (!loaded.empty()) {
        manager.setTasks(loaded);
    }

    // ── State ──
    int selected_tab = 0;
    std::vector<std::string> tab_labels = {
        " Tasks ", " Add Task ", " Sort ", " Filter ",
    };

    // Add task form state
    std::string new_title;
    std::string new_description;
    std::string new_due_date;
    int priority_selected = 1;
    std::string status_message;

    // Sort/filter state
    int sort_selected = 0;
    int filter_selected = 0;
    std::vector<::Task> filtered_tasks;
    bool showing_filtered = false;

    // Task action state
    std::string action_id_str;
    int action_selected = 0;

    // ── Tab 0: Task List ──
    auto input_action_id = Input(&action_id_str, "Task ID");
    auto action_entries = std::vector<std::string>{"Mark Completed", "Remove Task"};
    auto action_toggle = Toggle(&action_entries, &action_selected);
    auto action_button = Button("Execute", [&] {
        try {
            int id = std::stoi(action_id_str);
            if (action_selected == 0) {
                if (manager.markTaskCompleted(id))
                    status_message = "Task " + std::to_string(id) + " marked completed.";
                else
                    status_message = "Task not found.";
            } else {
                if (manager.removeTask(id))
                    status_message = "Task " + std::to_string(id) + " removed.";
                else
                    status_message = "Task not found.";
            }
            showing_filtered = false;
        } catch (...) {
            status_message = "Invalid ID.";
        }
        action_id_str.clear();
    });
    auto save_button = Button("Save to File", [&] {
        if (Storage::saveToFile(manager.getTasks(), DATA_FILE))
            status_message = "Tasks saved to " + DATA_FILE;
        else
            status_message = "Error: Failed to save tasks!";
    });

    auto task_list_container = Container::Vertical(Components{
        Container::Horizontal(Components{action_toggle, input_action_id, action_button}),
        save_button,
    });

    auto task_list_renderer = Renderer(task_list_container, [&] {
        const auto& display_tasks = showing_filtered
            ? static_cast<const std::vector<::Task>&>(filtered_tasks)
            : manager.getTasks();
        Elements content;
        content.push_back(text(showing_filtered ? " Filtered Tasks" : " All Tasks") | bold);
        content.push_back(separator());
        content.push_back(TaskTable(display_tasks));
        content.push_back(separator());

        Elements action_bar;
        action_bar.push_back(text(" Action: "));
        action_bar.push_back(action_toggle->Render());
        action_bar.push_back(text("  ID: "));
        action_bar.push_back(input_action_id->Render() | size(WIDTH, EQUAL, 8));
        action_bar.push_back(text("  "));
        action_bar.push_back(action_button->Render());
        action_bar.push_back(text("    "));
        action_bar.push_back(save_button->Render());
        content.push_back(hbox(std::move(action_bar)));

        return vbox(std::move(content));
    });

    // ── Tab 1: Add Task ──
    auto input_title = Input(&new_title, "Enter title...");
    auto input_desc  = Input(&new_description, "Enter description...");
    auto input_date  = Input(&new_due_date, "YYYY-MM-DD");
    auto priority_entries = std::vector<std::string>{"Low", "Medium", "High"};
    auto priority_toggle = Toggle(&priority_entries, &priority_selected);
    auto add_button = Button("Add Task", [&] {
        if (new_title.empty()) {
            status_message = "Title cannot be empty.";
            return;
        }
        ::Priority p = ::Priority::MEDIUM;
        if (priority_selected == 0) p = ::Priority::LOW;
        else if (priority_selected == 2) p = ::Priority::HIGH;
        manager.addTask(new_title, new_description, p,
                        new_due_date.empty() ? "N/A" : new_due_date);
        status_message = "Task '" + new_title + "' added! (ID: " +
                         std::to_string(manager.getNextId() - 1) + ")";
        new_title.clear();
        new_description.clear();
        new_due_date.clear();
        priority_selected = 1;
        showing_filtered = false;
        selected_tab = 0;
    });

    auto add_container = Container::Vertical(Components{
        input_title, input_desc, input_date, priority_toggle, add_button,
    });
    auto add_renderer = Renderer(add_container, [&] {
        Elements form;
        form.push_back(text(" Add New Task") | bold);
        form.push_back(separator());

        Elements row1;
        row1.push_back(text(" Title:       "));
        row1.push_back(input_title->Render() | flex);
        form.push_back(hbox(std::move(row1)));

        Elements row2;
        row2.push_back(text(" Description: "));
        row2.push_back(input_desc->Render() | flex);
        form.push_back(hbox(std::move(row2)));

        Elements row3;
        row3.push_back(text(" Due Date:    "));
        row3.push_back(input_date->Render() | size(WIDTH, EQUAL, 14));
        form.push_back(hbox(std::move(row3)));

        Elements row4;
        row4.push_back(text(" Priority:    "));
        row4.push_back(priority_toggle->Render());
        form.push_back(hbox(std::move(row4)));

        form.push_back(separator());
        form.push_back(add_button->Render());

        return vbox(std::move(form));
    });

    // ── Tab 2: Sort ──
    auto sort_entries = std::vector<std::string>{
        "Priority (High to Low)", "Due Date (Earliest)", "Name (A-Z)"};
    auto sort_menu = Radiobox(&sort_entries, &sort_selected);
    auto sort_button = Button("Sort", [&] {
        switch (sort_selected) {
            case 0: manager.setSortStrategy(std::make_unique<SortByPriority>()); break;
            case 1: manager.setSortStrategy(std::make_unique<SortByDueDate>()); break;
            case 2: manager.setSortStrategy(std::make_unique<SortByName>()); break;
        }
        manager.sortTasks();
        showing_filtered = false;
        status_message = "Tasks sorted by " + sort_entries[sort_selected] + ".";
        selected_tab = 0;
    });

    auto sort_container = Container::Vertical(Components{sort_menu, sort_button});
    auto sort_renderer = Renderer(sort_container, [&] {
        Elements content;
        content.push_back(text(" Sort Tasks") | bold);
        content.push_back(separator());
        content.push_back(sort_menu->Render());
        content.push_back(separator());
        content.push_back(sort_button->Render());
        return vbox(std::move(content));
    });

    // ── Tab 3: Filter ──
    auto filter_entries = std::vector<std::string>{
        "All Tasks", "Completed", "Pending",
        "High Priority", "Medium Priority", "Low Priority"};
    auto filter_menu = Radiobox(&filter_entries, &filter_selected);
    auto filter_button = Button("Apply Filter", [&] {
        if (filter_selected == 0) {
            showing_filtered = false;
            status_message = "Showing all tasks.";
        } else {
            switch (filter_selected) {
                case 1: manager.setFilterStrategy(std::make_unique<FilterByStatus>(true)); break;
                case 2: manager.setFilterStrategy(std::make_unique<FilterByStatus>(false)); break;
                case 3: manager.setFilterStrategy(std::make_unique<FilterByPriority>(::Priority::HIGH)); break;
                case 4: manager.setFilterStrategy(std::make_unique<FilterByPriority>(::Priority::MEDIUM)); break;
                case 5: manager.setFilterStrategy(std::make_unique<FilterByPriority>(::Priority::LOW)); break;
            }
            filtered_tasks = manager.filterTasks();
            showing_filtered = true;
            status_message = "Filtered: " + filter_entries[filter_selected] +
                             " (" + std::to_string(filtered_tasks.size()) + " tasks)";
        }
        selected_tab = 0;
    });

    auto filter_container = Container::Vertical(Components{filter_menu, filter_button});
    auto filter_renderer = Renderer(filter_container, [&] {
        Elements content;
        content.push_back(text(" Filter Tasks") | bold);
        content.push_back(separator());
        content.push_back(filter_menu->Render());
        content.push_back(separator());
        content.push_back(filter_button->Render());
        return vbox(std::move(content));
    });

    // ── Main layout with tabs ──
    auto tab_content = Container::Tab(Components{
        task_list_renderer,
        add_renderer,
        sort_renderer,
        filter_renderer,
    }, &selected_tab);

    auto tab_toggle = Menu(&tab_labels, &selected_tab, MenuOption::HorizontalAnimated());

    auto exit_button = Button(" Quit ", [&] {
        Storage::saveToFile(manager.getTasks(), DATA_FILE);
        screen.Exit();
    });

    auto main_container = Container::Vertical(Components{
        Container::Horizontal(Components{tab_toggle, exit_button}),
        tab_content,
    });

    auto main_renderer = Renderer(main_container, [&] {
        Elements header_left;
        header_left.push_back(text(" Task Manager ") | bold | color(Color::Cyan));
        header_left.push_back(filler());
        header_left.push_back(text(" [Tab/Arrow to navigate] ") | dim);

        Elements tab_bar;
        tab_bar.push_back(tab_toggle->Render() | flex);
        tab_bar.push_back(exit_button->Render());

        Elements status_bar;
        status_bar.push_back(text(" " + status_message + " ") | color(Color::GreenLight));
        status_bar.push_back(filler());
        status_bar.push_back(text(" Tasks: " + std::to_string(manager.getTasks().size()) + " ") | dim);

        Elements layout;
        layout.push_back(hbox(std::move(header_left)));
        layout.push_back(hbox(std::move(tab_bar)) | bgcolor(Color::GrayDark));
        layout.push_back(separator());
        layout.push_back(tab_content->Render() | flex);
        layout.push_back(separator());
        layout.push_back(hbox(std::move(status_bar)));

        return vbox(std::move(layout)) | border;
    });

    screen.Loop(main_renderer);
    return 0;
}
