#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// -----------------------------
// Course Struct
// -----------------------------
struct Course {
    string courseId;
    string courseName;
    vector<string> prerequisites;
};

// -----------------------------
// Node Struct for BST
// -----------------------------
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// -----------------------------
// Binary Search Tree Class
// -----------------------------
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseId < node->course.courseId) {
            addNode(node->left, course);
        }
        else {
            addNode(node->right, course);
        }
    }

    Node* searchNode(Node* node, const string& courseId) const {
        if (node == nullptr) return nullptr;

        if (courseId == node->course.courseId) {
            return node;
        }
        else if (courseId < node->course.courseId) {
            return searchNode(node->left, courseId);
        }
        else {
            return searchNode(node->right, courseId);
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseId << ", " << node->course.courseName << endl;
            inOrder(node->right);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroyTree(root); }

    void Insert(const string& courseId, const string& courseName, const vector<string>& prereqs) {
        Course c;
        c.courseId = courseId;
        c.courseName = courseName;
        c.prerequisites = prereqs;
        addNode(root, c);
    }

    Course* Search(const string& courseId) {
        Node* result = searchNode(root, courseId);
        if (result != nullptr) return &(result->course);
        return nullptr;
    }

    void InOrder() const {
        inOrder(root);
    }
};

// -----------------------------
// CSV Parsing Helper
// -----------------------------
Course parseCourseLine(const string& line) {
    Course course;
    vector<string> tokens;
    size_t start = 0, end;

    while ((end = line.find(',', start)) != string::npos) {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(line.substr(start));

    if (!tokens.empty()) {
        course.courseId = tokens[0];
    }
    if (tokens.size() > 1) {
        course.courseName = tokens[1];
    }
    if (tokens.size() > 2) {
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }
    }
    return course;
}

// -----------------------------
// Program Functions
// -----------------------------
void loadCourses(BinarySearchTree& bst) {
    ifstream file("ABCU_Advising_Program_Input.csv");
    if (!file.is_open()) {
        cout << "Error: Could not open the file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        Course course = parseCourseLine(line);
        bst.Insert(course.courseId, course.courseName, course.prerequisites);
    }

    file.close();
    cout << "Courses loaded successfully.\n";
}

void printCourseList(BinarySearchTree& bst) {
    cout << "Here is a sample schedule:\n";
    bst.InOrder();
}

void printCourseInfo(BinarySearchTree& bst) {
    string courseId;
    cout << "What course do you want to know about? ";
    cin >> courseId;

    transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);

    Course* course = bst.Search(courseId);

    if (!course) {
        cout << "Course not found.\n";
        return;
    }

    cout << course->courseId << ", " << course->courseName << endl;
    if (!course->prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None\n";
    }
}

// -----------------------------
// Main Program
// -----------------------------
int main() {
    BinarySearchTree bst;
    int choice = 0;

    cout << "Welcome to the course planner.\n";

    while (choice != 9) {
        cout << "\n1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {
        case 1:
            loadCourses(bst);
            break;
        case 2:
            printCourseList(bst);
            break;
        case 3:
            printCourseInfo(bst);
            break;
        case 9:
            cout << "Thank you for using the course planner!\n";
            break;
        default:
            cout << choice << " is not a valid option.\n";
        }
    }

    return 0;
}

