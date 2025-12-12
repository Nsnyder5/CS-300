// Nicole Snyder
// CS-300 Project 2
// 12- 12- 2025
// Professor Philip Grabowski

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>

// Simple helper to trim whitespace from both ends of a string
static inline std::string Trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    if (start == s.size()) return "";
    size_t end = s.size() - 1;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end]))) --end;
    return s.substr(start, end - start + 1);
}

// Split a line by commas, returning trimmed tokens. Empty tokens (after trim) are included.
static std::vector<std::string> SplitByComma(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream ss(line);
    while (std::getline(ss, token, ',')) {
        tokens.push_back(Trim(token));
    }
    return tokens;
}

struct Course {
    std::string courseNumber;             // key
    std::string courseName;
    std::vector<std::string> prerequisites;
};

// Load courses from file with two-pass validation (first pass collects all course numbers,
// second pass builds Course objects and validates that each prerequisite exists).
static std::map<std::string, Course> LoadCoursesFromFile(const std::string& filename) {
    std::map<std::string, Course> allCourses;
    std::set<std::string> courseNumbers;

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return allCourses;
    }

    std::string line;
    // First pass: gather course numbers
    while (std::getline(infile, line)) {
        std::string trimmedLine = Trim(line);
        if (trimmedLine.empty()) continue; // skip blank lines

        auto tokens = SplitByComma(trimmedLine);
        if (tokens.size() < 2) {
            std::cerr << "File Format Error: Line does not have enough parameters: " << line << std::endl;
            continue;
        }
        std::string courseNumber = tokens[0];
        if (courseNumber.empty()) {
            std::cerr << "File Format Error: Empty course number on line: " << line << std::endl;
            continue;
        }
        courseNumbers.insert(courseNumber);
    }

    // Re-open file for second pass
    infile.clear();
    infile.seekg(0);
    if (!infile.good()) {
        // some implementations of ifstream don't support seek on certain streams; reopen instead
        infile.close();
        infile.open(filename);
        if (!infile.is_open()) {
            std::cerr << "Error: Could not reopen file " << filename << " for second pass." << std::endl;
            return allCourses;
        }
    }

    // Second pass: parse and validate prerequisites, create Course objects
    while (std::getline(infile, line)) {
        std::string trimmedLine = Trim(line);
        if (trimmedLine.empty()) continue;

        auto tokens = SplitByComma(trimmedLine);
        if (tokens.size() < 2) {
            // Already reported in first pass; skip
            continue;
        }

        Course newCourse;
        newCourse.courseNumber = tokens[0];
        newCourse.courseName = tokens[1];

        if (newCourse.courseNumber.empty() || newCourse.courseName.empty()) {
            std::cerr << "File Format Error: Course number or name empty: " << line << std::endl;
            continue;
        }

        bool validPrereqs = true;
        for (size_t i = 2; i < tokens.size(); ++i) {
            const std::string& prereqNumber = tokens[i];
            if (prereqNumber.empty()) continue; // ignore empty prereq tokens
            if (courseNumbers.find(prereqNumber) == courseNumbers.end()) {
                std::cerr << "Prerequisite Error: Course " << newCourse.courseNumber
                    << " lists an unknown prerequisite: " << prereqNumber << std::endl;
                validPrereqs = false;
                break; // stop processing prereqs for this course
            }
            else {
                newCourse.prerequisites.push_back(prereqNumber);
            }
        }

        if (validPrereqs) {
            // Insert into map (std::map keeps keys sorted)
            allCourses[newCourse.courseNumber] = std::move(newCourse);
        }
    }

    infile.close();
    return allCourses;
}

// Print the entire catalog in alphanumeric order (std::map is already ordered by key)
static void PrintAllCourses(const std::map<std::string, Course>& catalog) {
    if (catalog.empty()) {
        std::cout << "Catalog is empty. Load data first (Option 1)." << std::endl;
        return;
    }

    std::cout << "\n--- Full Course Catalog ---\n";
    for (const auto& kv : catalog) {
        const Course& c = kv.second;
        std::cout << "Course: " << c.courseNumber << ", " << c.courseName << "\n";
        if (c.prerequisites.empty()) {
            std::cout << "    Prerequisites: None\n";
        }
        else {
            std::cout << "    Prerequisites:\n";
            for (const auto& p : c.prerequisites) {
                // Try to look up the title for the prerequisite
                auto it = catalog.find(p);
                if (it != catalog.end()) {
                    std::cout << "        - " << p << " : " << it->second.courseName << "\n";
                }
                else {
                    // Shouldn't happen due to validation, but handle defensively
                    std::cout << "        - " << p << " : [Title Not Found]\n";
                }
            }
        }
        std::cout << "\n";
    }
}

// Print information for a single course; includes the title and prerequisites (with titles)
static void PrintCourseInformation(const std::map<std::string, Course>& catalog, const std::string& searchCourseNumber) {
    auto it = catalog.find(searchCourseNumber);
    if (it == catalog.end()) {
        std::cout << "Error: Course " << searchCourseNumber << " not found in the catalog." << std::endl;
        return;
    }

    const Course& c = it->second;
    std::cout << "\n--- Course Information for " << searchCourseNumber << " ---\n";
    std::cout << "Course Number: " << c.courseNumber << "\n";
    std::cout << "Title: " << c.courseName << "\n";

    if (c.prerequisites.empty()) {
        std::cout << "Prerequisites: None\n";
    }
    else {
        std::cout << "Prerequisites:\n";
        for (const auto& p : c.prerequisites) {
            auto pit = catalog.find(p);
            if (pit != catalog.end()) {
                std::cout << "    - " << p << " : " << pit->second.courseName << "\n";
            }
            else {
                std::cout << "    - " << p << " : [Title Not Found]\n";
            }
        }
    }
}

// Simple function to display the menu options
static void PrintMenu() {
    std::cout << "\n===== ABCU CS Advising Menu =====\n";
    std::cout << "1. Load course data from file\n";
    std::cout << "2. Print an alphanumeric list of all courses\n";
    std::cout << "3. Print the course title and prerequisites for an individual course\n";
    std::cout << "9. Exit the program\n";
    std::cout << "Enter selection: ";
}

int main() {
    std::map<std::string, Course> coursesCatalog;
    bool running = true;

    while (running) {
        PrintMenu();
        std::string selectionLine;
        if (!std::getline(std::cin, selectionLine)) {
            // EOF or input error: exit
            std::cout << "\nInput closed. Exiting.\n";
            break;
        }

        std::string selection = Trim(selectionLine);
        if (selection.empty()) {
            std::cout << "Please enter a menu option number." << std::endl;
            continue;
        }

        if (selection == "1") {
            std::cout << "Enter the file name containing course data: ";
            std::string filename;
            if (!std::getline(std::cin, filename)) {
                std::cout << "Input error. Returning to menu." << std::endl;
                continue;
            }
            filename = Trim(filename);
            if (filename.empty()) {
                std::cout << "Filename cannot be empty." << std::endl;
                continue;
            }
            auto loaded = LoadCoursesFromFile(filename);
            if (loaded.empty()) {
                std::cout << "Program: No valid courses loaded. Catalog remains empty." << std::endl;
            }
            else {
                coursesCatalog = std::move(loaded);
                std::cout << "Successfully loaded " << coursesCatalog.size() << " courses into the catalog." << std::endl;
            }
        }
        else if (selection == "2") {
            if (coursesCatalog.empty()) {
                std::cout << "Catalog is empty. Please load data first with option 1." << std::endl;
            }
            else {
                PrintAllCourses(coursesCatalog);
            }
        }
        else if (selection == "3") {
            if (coursesCatalog.empty()) {
                std::cout << "Catalog is empty. Please load data first with option 1." << std::endl;
                continue;
            }
            std::cout << "Enter the course number to look up (e.g., CSCI300): ";
            std::string courseNumber;
            if (!std::getline(std::cin, courseNumber)) {
                std::cout << "Input error. Returning to menu." << std::endl;
                continue;
            }
            courseNumber = Trim(courseNumber);
            if (courseNumber.empty()) {
                std::cout << "Course number cannot be empty." << std::endl;
                continue;
            }
            PrintCourseInformation(coursesCatalog, courseNumber);
        }
        else if (selection == "9") {
            std::cout << "Exiting program. Goodbye." << std::endl;
            running = false;
        }
        else {
            std::cout << "Invalid selection: " << selection << ". Please choose 1, 2, 3, or 9." << std::endl;
        }
    }

    return 0;
}