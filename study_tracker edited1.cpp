#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;
class Assignment {
public:
    string subject;
    string name;
    string deadline;

    Assignment(const string& assignmentSubject, const string& assignmentName, const string& assignmentDeadline)
        : subject(assignmentSubject), name(assignmentName), deadline(assignmentDeadline) {}
};

class UniversityJournal {
private:
    Assignment** assignments; // Using an array of pointers
    int assignmentCount;
    int maxAssignments; // Maximum number of assignments

public:
    UniversityJournal() : assignmentCount(0), maxAssignments(50) {
        assignments = new Assignment*[maxAssignments];
        loadFromFile("assignments.txt"); // Load assignments from file on startup
    }

    ~UniversityJournal() {
        for (int i = 0; i < assignmentCount; ++i) {
            delete assignments[i]; // Release individual Assignment objects
        }
        delete[] assignments; // Release the array of pointers
    }

    void addAssignment(const Assignment& assignment) {
        if (assignmentCount < maxAssignments) {
            assignments[assignmentCount++] = new Assignment(assignment);
            saveToFile("assignments.txt"); // Save assignments automatically after adding
        } else {
            cerr << "Maximum assignment limit reached\n";
        }
    }

    void deleteAssignment(int index) {
        if (index >= 0 && index < assignmentCount) {
            delete assignments[index];
            // Shift remaining assignments to fill the gap
            for (int i = index; i < assignmentCount - 1; ++i) {
                assignments[i] = assignments[i + 1];
            }
            --assignmentCount;
            cout << "Assignment deleted successfully.\n";
            saveToFile("assignments.txt"); // Save assignments automatically after deleting
        } else {
            cerr << "Invalid assignment index.\n";
        }
    }

    // Function to compare two assignments based on their deadline
    static bool compareAssignmentsByDeadline(const Assignment* a, const Assignment* b) {
        // Custom comparison function for sorting by deadline
        return a->deadline.substr(6, 2) + a->deadline.substr(3, 2) + a->deadline.substr(0, 2) <
               b->deadline.substr(6, 2) + b->deadline.substr(3, 2) + b->deadline.substr(0, 2);
    }

    // Modify the showAssignments function
    void showAssignments() {
        if (assignmentCount == 0) {
            cout << "No assignments available.\n";
        } else {
            // Sort assignments by date before displaying
            std::sort(assignments, assignments + assignmentCount, compareAssignmentsByDeadline);

            cout << "Current assignments (sorted by deadline):\n";
            for (int i = 0; i < assignmentCount; ++i) {
                cout << "[" << i + 1 << "] Subject: " << assignments[i]->subject << ", Name: " << assignments[i]->name
                     << ", Deadline: " << assignments[i]->deadline << '\n';
            }
        }
    }

    void inputAssignments(int countToAdd) {
        for (int i = 0; i < countToAdd; ++i) {
            string subject, name, deadline;

            // Clear the input buffer before each subject input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter subject for assignment " << i + 1 << ": ";
            getline(cin, subject);
            cout << "Enter name for assignment " << i + 1 << ": ";
            getline(cin, name);

            // Input date in DD.MM.YY format
            cout << "Enter deadline for assignment " << i + 1 << " (DD.MM.YY): ";
            getline(cin, deadline);

            addAssignment(Assignment(subject, name, deadline));
            cout << "Assignment " << i + 1 << " added successfully.\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            while (!file.eof()) {
                string subject, name, deadline;
                getline(file, subject, ',');
                getline(file, name, ',');
                getline(file, deadline, '\n');

                if (!subject.empty() && !name.empty() && !deadline.empty()) {
                    addAssignment(Assignment(subject, name, deadline));
                }
            }
            file.close();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < assignmentCount; ++i) {
                file << assignments[i]->subject << "," << assignments[i]->name << ","
                     << assignments[i]->deadline << "\n";
            }
            file.close();
            cout << "Assignments saved to " << filename << "\n";
        } else {
            cerr << "Error opening file for writing\n";
        }
    }
};

int main() {
    UniversityJournal journal;

    int choice;
    do {
        cout << "\nOptions:\n";
        cout << "1. Show assignments\n";
        cout << "2. Add assignments\n";
        cout << "3. Delete assignment\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                journal.showAssignments();
                break;
            case 2: {
                int countToAdd;
                cout << "Enter the number of assignments to add: ";
                cin >> countToAdd;
                journal.inputAssignments(countToAdd);
                break;
            }
            case 3: {
                journal.showAssignments();
                int indexToDelete;
                cout << "Enter the index of the assignment to delete: ";
                cin >> indexToDelete;
                journal.deleteAssignment(indexToDelete - 1); // Adjust for 0-based indexing
                break;
            }
            case 0:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
