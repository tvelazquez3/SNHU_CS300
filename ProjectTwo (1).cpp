// Name: ProjectTwo.cpp
// Author: Tessa Velazquez
// Version: 1.0
// Description: ABCU Advising Assistance Program

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// course structure

struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};

// binary search tree

class BinarySearchTree {
private:
	struct Node {
		Course course;
		Node* left;
		Node* right;

		Node(Course aCourse) {
			course = aCourse;
			left = nullptr;
			right = nullptr;
		}
	};
	Node* root;
	int treeSize;

	void addNode(Node* node, Course course) {
		if (course.courseNumber < node->course.courseNumber) {
			if (node->left == nullptr) {
				node->left = new Node(course);
			}
			else {
				addNode(node->left, course);
			}

		}
		else if (course.courseNumber > node->course.courseNumber) {
			if (node->right == nullptr) {
				node->right = new Node(course);
			}
			else {
				addNode(node->right, course);
			}
		}
		else {
			// duplicate course numbers are not inserted
			cout << "Warning: Duplicate course " << course.courseNumber << " was skipped." << endl;

		}
	}

	void inOrder(Node* node) const {
		if (node != nullptr) {
			inOrder(node->left);
			cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
			inOrder(node->right);
		}
	}

	void clear(Node* node) {
		if (node != nullptr) {
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}
public:
	BinarySearchTree() {
		root = nullptr;
		treeSize = 0;
	}

	~BinarySearchTree() {
		clear(root);
	}

	void clearTree() {
		clear(root);
		root = nullptr;
		treeSize = 0;
	}

	void insert(Course course) {
		if (root == nullptr) {
			root = new Node(course);
		}
		else {
			addNode(root, course);
		}
		treeSize++;
	}

	Course* search(string courseNumber) const {
		Node* current = root;
		while (current != nullptr) {
			if (current->course.courseNumber == courseNumber) {
				return &(current->course);
			}
			else if (courseNumber < current->course.courseNumber) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return nullptr;
	}

	void printCourseList() const {
		inOrder(root);
	}

	int size() const {
		return treeSize;
	}
};

// helper functions

string trim(const string& value) {
	size_t start = value.find_first_not_of(" \t\r\n");
	size_t end = value.find_last_not_of(" \t\r\n");

	if (start == string::npos) {
		return "";
	}
	return value.substr(start, end - start + 1);
}

string toUpperCase(string value) {
	for (char& letter : value) {
		letter = static_cast<char>(toupper(static_cast<unsigned char>(letter)));
	}
	return value;
}

vector<string> splitLine(const string& line) {
	vector<string> tokens;
	string token;
	stringstream stream(line);

	while (getline(stream, token, ',')) {
		tokens.push_back(trim(token));
	}
	return tokens;
}

bool courseNumberExists(const vector<string>& courseNumbers, const string& courseNumber) {
	return find(courseNumbers.begin(), courseNumbers.end(), courseNumber) != courseNumbers.end();
}

// load course data

bool loadCourses(const string& filename, BinarySearchTree& courses) {
	ifstream file(filename);
	string line;
	vector<string> courseNumbers;
	vector<Course> tempCourses;

	if (!file.is_open()) {
		cout << "Error: File could not be opened." << endl;
		return false;
	}

	while (getline(file, line)) {
		line = trim(line);

		if (line.empty()) {
			continue;
		}
		vector<string> tokens = splitLine(line);

		if (tokens.size() < 2) {
			cout << "Error: Each line must include at least a course number and course title." << endl;
			return false;
		}
		Course course;
		course.courseNumber = toUpperCase(tokens.at(0));
		course.courseTitle = tokens.at(1);

		if (course.courseNumber.empty() || course.courseTitle.empty()) {
			cout << "Error: Course number and course title cannot be blank." << endl;
			return false;
		}

		for (unsigned int i = 2; i < tokens.size(); ++i) {
			string prerequisite = toUpperCase(tokens.at(i));
			if (!prerequisite.empty()) {
				course.prerequisites.push_back(prerequisite);
			}
		}
		courseNumbers.push_back(course.courseNumber);
		tempCourses.push_back(course);
	}
	file.close();

	// validating that every prerequisite exists as a course in file
	for (const Course& course : tempCourses) {
		for (const string& prerequisite : course.prerequisites) {
			if (!courseNumberExists(courseNumbers, prerequisite)) {
				cout << "Error: Prerequiste " << prerequisite << " does not exist as a course." << endl;
				return false;
			}
		}
	}
	courses.clearTree();
	for (const Course& course : tempCourses) {
		courses.insert(course);
	}

	cout << "Course data loaded successfully." << endl;
	return true;
}

// printing one course and its prerequisites
void printCourseInformation(const BinarySearchTree& courses, string courseNumber) {
	courseNumber = toUpperCase(trim(courseNumber));
	Course* course = courses.search(courseNumber);

	if (course == nullptr) {
		cout << "Course " << courseNumber << " was not found." << endl;
		return;
	}

	cout << course->courseNumber << ", " << course->courseTitle << endl;

	if (course->prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
	}
	else {
		cout << "Prerequisites:" << endl;

		for (const string& prerequisiteNumber : course->prerequisites) {
			Course* prerequisite = courses.search(prerequisiteNumber);
			if (prerequisite != nullptr) {
				cout << prerequisite->courseNumber << ", " << prerequisite->courseTitle << endl;
			}
			else {
				cout << prerequisiteNumber << " was not found in the course list." << endl;
			}
		}
	}
}

// main menu program

int main() {
	BinarySearchTree courses;
	bool dataLoaded = false;
	int choice = 0;
	string filename;
	string courseNumber;

	cout << "Welcome to the ABCU Advising Assistance Program." << endl;

	while (choice != 9) {
		cout << endl;
		cout << "Menu:" << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;
		cout << "Enter choice: ";

		if (!(cin >> choice)) {
			cout << "Error: Please enter a number from the menu." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		cin.ignore(10000, '\n');
		switch (choice) {
		case 1:
			cout << "Enter the course data file name: ";
			getline(cin, filename);
			dataLoaded = loadCourses(filename, courses);
			break;
		case 2:
			if (!dataLoaded) {
				cout << "Please load course data first." << endl;
			}
			else {
				cout << "Here is a sample schedule:" << endl;
			}
			break;
		case 3:
			if (!dataLoaded) {
				cout << "Please load course data first." << endl;
			}
			else {
				cout << "What courses do you want to know about? ";
				getline(cin, courseNumber);
				printCourseInformation(courses, courseNumber);
			}
			break;
		case 9:
			cout << "Thank you for using the course planner!" << endl;
			break;
		default:
			cout << choice << " is not a valid option." << endl;
			break;
		}
	}
	return 0;
}