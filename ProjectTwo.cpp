#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // for transform
#include <cctype>    // for tolower

using namespace std;

// Struct for holding course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Binary Search Tree Node
struct Node {
    Course course;
    Node* left;
    Node* right;
    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Class for Binary Search Tree
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            addNode(node->left, course);
        }
        else {
            addNode(node->right, course);
        }
    }

    void printInOrder(Node* node) {
        if (node != nullptr) {
            printInOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            printInOrder(node->right);
        }
    }

    Node* search(Node* node, const string& courseNumber) {
        if (node == nullptr) return nullptr;

        string nodeCourse = node->course.courseNumber;
        string targetCourse = courseNumber;

        // Convert both to lowercase for case-insensitive comparison
        transform(nodeCourse.begin(), nodeCourse.end(), nodeCourse.begin(), ::tolower);
        transform(targetCourse.begin(), targetCourse.end(), targetCourse.begin(), ::tolower);

        if (nodeCourse == targetCourse) return node;
        if (targetCourse < nodeCourse) return search(node->left, courseNumber);
        else return search(node->right, courseNumber);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void Insert(Course course) {
        addNode(root, course);
    }

    void PrintAllCourses() {
        cout << "\n=== Course List (Alphanumeric Order) ===" << endl;
        printInOrder(root);
    }

    void PrintCourseInformation(const string& courseNumber) {
        Node* node = search(root, courseNumber);
        if (node == nullptr) {
            cout << "\nCourse not found: " << courseNumber << endl;
            return;
        }
        cout << "\nCourse Number: " << node->course.courseNumber << endl;
        cout << "Course Title: " << node->course.courseTitle << endl;
        cout << "Prerequisites: ";
        if (node->course.prerequisites.empty()) {
            cout << "None" << endl;
        }
        else {
            for (size_t i = 0; i < node->course.prerequisites.size(); ++i) {
                cout << node->course.prerequisites[i];
                if (i < node->course.prerequisites.size() - 1)
                    cout << ", ";
            }
            cout << endl;
        }
    }
};

// Function to load courses from a CSV file
void loadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'. Please check the file path and name.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) continue; // Skip malformed lines

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        bst.Insert(course);
    }

    file.close();
    cout << "File '" << filename << "' loaded successfully!\n";
}

// Menu display
void displayMenu() {
    cout << "\n=== ABCU Advising Assistance Program ===\n";
    cout << "1. Load Data Structure\n";
    cout << "2. Print Course List\n";
    cout << "3. Print Course Information\n";
    cout << "9. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

int main() {
    BinarySearchTree bst;
    string filename = "CS 300 ABCU_Advising_Program_Input.csv";
    bool dataLoaded = false;
    string userInput;
    int choice = 0;

    while (choice != 9) {
        displayMenu();
        getline(cin, userInput);

        // Check if input is numeric
        bool isNumber = !userInput.empty() && all_of(userInput.begin(), userInput.end(), ::isdigit);

        if (isNumber) {
            choice = stoi(userInput);
        }
        else {
            cout << "Invalid selection: '" << userInput << "'. Please enter a number (1, 2, 3, or 9).\n";
            continue;
        }

        switch (choice) {
        case 1:
            loadCourses(filename, bst);
            dataLoaded = true;
            break;

        case 2:
            if (dataLoaded) {
                bst.PrintAllCourses();
            }
            else {
                cout << "Please load the data first (Option 1).\n";
            }
            break;

        case 3:
            if (dataLoaded) {
                cout << "Enter course number (e.g., CS200 or cs200): ";
                {
                    string input;
                    getline(cin, input);
                    bst.PrintCourseInformation(input);
                }
            }
            else {
                cout << "Please load the data first (Option 1).\n";
            }
            break;

        case 9:
            cout << "Exiting program. Goodbye!\n";
            break;

        default:
            cout << "Invalid selection: '" << userInput
                << "'. Please enter 1, 2, 3, or 9.\n";
            break;
        }
    }

    return 0;
}

