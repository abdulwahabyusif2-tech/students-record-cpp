// ============================================================
// Student Records and Grade Application
// Data structure used: ARRAYS
//
//   - students[]      : array of Student structs (fixed capacity)
//   - Student.scores[]: array of scores, one per course
//   - COURSE_NAMES[]  : array of course names
//
// Demonstrates: indexed access, linear search, insertion at end,
// deletion by shifting elements, selection sort, and file I/O.
// ============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

// ---------- Constants ----------
const int MAX_STUDENTS = 100;   // capacity of the students array
const int NUM_COURSES  = 5;     // size of each scores array

const string COURSE_NAMES[NUM_COURSES] = {
    "Programming", "Mathematics", "Networking", "Databases", "English"
};
const string SHORT_NAMES[NUM_COURSES] = {"Prog", "Math", "Netw", "DB", "Eng"};
const string DATA_FILE = "students.txt";

// ---------- Data ----------
struct Student {
    int    id;
    string name;
    double scores[NUM_COURSES];   // array inside the struct
};

Student students[MAX_STUDENTS];   // the main array of records
int studentCount = 0;             // how many slots are actually used

// ---------- Input helpers ----------
void exitOnEof() {
    if (cin.eof()) {
        cout << "\nInput ended. Goodbye!\n";
        exit(0);
    }
}

int readInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        exitOnEof();
        cout << "Invalid input. Enter a whole number from " << minVal
             << " to " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readScore(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0.0 && value <= 100.0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        exitOnEof();
        cout << "Invalid score. Enter a number from 0 to 100.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readName() {
    string name;
    while (true) {
        cout << "Enter student name: ";
        getline(cin, name);
        exitOnEof();
        if (!name.empty() && name.find(',') == string::npos) {
            return name;
        }
        cout << "Name cannot be empty or contain commas.\n";
    }
}

// ---------- Core logic ----------
double calculateAverage(const Student& s) {
    double sum = 0;
    for (int i = 0; i < NUM_COURSES; i++) {
        sum += s.scores[i];
    }
    return sum / NUM_COURSES;
}

char getGrade(double avg) {
    if (avg >= 80) return 'A';
    if (avg >= 70) return 'B';
    if (avg >= 60) return 'C';
    if (avg >= 50) return 'D';
    return 'F';
}

// Linear search: returns array index, or -1 if not found
int findStudentIndex(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) return i;
    }
    return -1;
}

void printHeader() {
    cout << left << setw(8) << "ID" << setw(20) << "Name";
    for (int i = 0; i < NUM_COURSES; i++) cout << setw(7) << SHORT_NAMES[i];
    cout << setw(9) << "Average" << "Grade\n";
    cout << string(8 + 20 + NUM_COURSES * 7 + 9 + 5, '-') << "\n";
}

void printStudent(const Student& s) {
    double avg = calculateAverage(s);
    string shown = s.name.substr(0, 18);
    cout << left << setw(8) << s.id << setw(20) << shown;
    cout << fixed << setprecision(1);
    for (int i = 0; i < NUM_COURSES; i++) cout << setw(7) << s.scores[i];
    cout << setw(9) << avg << getGrade(avg) << "\n";
}

// ---------- Menu operations ----------
void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "The array is full (" << MAX_STUDENTS << " students). Cannot add more.\n";
        return;
    }
    int id = readInt("Enter student ID (1-999999): ", 1, 999999);
    if (findStudentIndex(id) != -1) {
        cout << "A student with ID " << id << " already exists.\n";
        return;
    }
    Student s;
    s.id = id;
    s.name = readName();
    for (int i = 0; i < NUM_COURSES; i++) {
        s.scores[i] = readScore("  " + COURSE_NAMES[i] + " score (0-100): ");
    }
    students[studentCount] = s;   // insert at the end: O(1)
    studentCount++;
    cout << "Student added.\n";
}

void displayAll() {
    if (studentCount == 0) {
        cout << "No records yet.\n";
        return;
    }
    printHeader();
    for (int i = 0; i < studentCount; i++) printStudent(students[i]);
    cout << "\nTotal students: " << studentCount << " / " << MAX_STUDENTS << "\n";
}

void searchStudent() {
    int id = readInt("Enter student ID to search: ", 1, 999999);
    int idx = findStudentIndex(id);
    if (idx == -1) {
        cout << "No student found with ID " << id << ".\n";
        return;
    }
    printHeader();
    printStudent(students[idx]);
}

void updateScore() {
    int id = readInt("Enter student ID: ", 1, 999999);
    int idx = findStudentIndex(id);
    if (idx == -1) {
        cout << "No student found with ID " << id << ".\n";
        return;
    }
    cout << "Courses:\n";
    for (int i = 0; i < NUM_COURSES; i++) {
        cout << "  " << (i + 1) << ". " << COURSE_NAMES[i]
             << " (current: " << students[idx].scores[i] << ")\n";
    }
    int c = readInt("Choose course (1-" + to_string(NUM_COURSES) + "): ", 1, NUM_COURSES);
    students[idx].scores[c - 1] = readScore("New score (0-100): ");
    cout << "Score updated.\n";
}

void deleteStudent() {
    int id = readInt("Enter student ID to delete: ", 1, 999999);
    int idx = findStudentIndex(id);
    if (idx == -1) {
        cout << "No student found with ID " << id << ".\n";
        return;
    }
    // Shift every later element one position left: O(n)
    for (int i = idx; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;
    cout << "Student deleted.\n";
}

// Selection sort by average, highest first
void sortByAverage() {
    for (int i = 0; i < studentCount - 1; i++) {
        int best = i;
        for (int j = i + 1; j < studentCount; j++) {
            if (calculateAverage(students[j]) > calculateAverage(students[best])) {
                best = j;
            }
        }
        if (best != i) {
            Student temp = students[i];
            students[i] = students[best];
            students[best] = temp;
        }
    }
}

void showRanking() {
    if (studentCount == 0) {
        cout << "No records yet.\n";
        return;
    }
    sortByAverage();
    cout << "Class ranking (highest average first):\n";
    printHeader();
    for (int i = 0; i < studentCount; i++) printStudent(students[i]);
}

void classStatistics() {
    if (studentCount == 0) {
        cout << "No records yet.\n";
        return;
    }
    double courseTotals[NUM_COURSES] = {0};
    double courseHigh[NUM_COURSES];
    double courseLow[NUM_COURSES];
    for (int c = 0; c < NUM_COURSES; c++) {
        courseHigh[c] = 0;
        courseLow[c] = 100;
    }
    int gradeCounts[5] = {0};   // A, B, C, D, F
    double classTotal = 0;
    int topIdx = 0, lowIdx = 0;

    for (int i = 0; i < studentCount; i++) {
        double avg = calculateAverage(students[i]);
        classTotal += avg;
        if (avg > calculateAverage(students[topIdx])) topIdx = i;
        if (avg < calculateAverage(students[lowIdx])) lowIdx = i;

        char g = getGrade(avg);
        int slot = (g == 'F') ? 4 : (g - 'A');
        gradeCounts[slot]++;

        for (int c = 0; c < NUM_COURSES; c++) {
            double sc = students[i].scores[c];
            courseTotals[c] += sc;
            if (sc > courseHigh[c]) courseHigh[c] = sc;
            if (sc < courseLow[c])  courseLow[c]  = sc;
        }
    }

    cout << fixed << setprecision(1);
    cout << "\n=== Class Statistics ===\n";
    cout << "Students: " << studentCount << "\n";
    cout << "Class average: " << classTotal / studentCount << "\n";
    cout << "Top student: " << students[topIdx].name << " ("
         << calculateAverage(students[topIdx]) << ")\n";
    cout << "Lowest student: " << students[lowIdx].name << " ("
         << calculateAverage(students[lowIdx]) << ")\n\n";

    cout << left << setw(14) << "Course" << setw(10) << "Average"
         << setw(10) << "Highest" << "Lowest\n";
    cout << string(44, '-') << "\n";
    for (int c = 0; c < NUM_COURSES; c++) {
        cout << setw(14) << COURSE_NAMES[c]
             << setw(10) << courseTotals[c] / studentCount
             << setw(10) << courseHigh[c] << courseLow[c] << "\n";
    }

    const char labels[5] = {'A', 'B', 'C', 'D', 'F'};
    cout << "\nGrade distribution:\n";
    for (int i = 0; i < 5; i++) {
        cout << "  " << labels[i] << ": " << string(gradeCounts[i], '*')
             << " (" << gradeCounts[i] << ")\n";
    }
}

// ---------- File storage ----------
bool saveToFile() {
    ofstream out(DATA_FILE);
    if (!out) {
        cout << "Could not write " << DATA_FILE << ".\n";
        return false;
    }
    for (int i = 0; i < studentCount; i++) {
        out << students[i].id << "," << students[i].name;
        for (int c = 0; c < NUM_COURSES; c++) out << "," << students[i].scores[c];
        out << "\n";
    }
    return true;
}

void loadFromFile() {
    ifstream in(DATA_FILE);
    if (!in) return;   // first run: no file yet
    string line;
    while (studentCount < MAX_STUDENTS && getline(in, line)) {
        stringstream ss(line);
        string idText, name, scoreText;
        if (!getline(ss, idText, ',') || !getline(ss, name, ',')) continue;
        Student s;
        s.id = atoi(idText.c_str());
        s.name = name;
        bool ok = true;
        for (int c = 0; c < NUM_COURSES; c++) {
            if (!getline(ss, scoreText, ',')) { ok = false; break; }
            s.scores[c] = atof(scoreText.c_str());
        }
        if (ok && s.id > 0) {
            students[studentCount] = s;
            studentCount++;
        }
    }
    cout << "Loaded " << studentCount << " student(s) from " << DATA_FILE << ".\n";
}

// ---------- Main ----------
void showMenu() {
    cout << "\n===== Student Records & Grades =====\n"
         << " 1. Add student\n"
         << " 2. View all students\n"
         << " 3. Search by ID\n"
         << " 4. Update a score\n"
         << " 5. Delete student\n"
         << " 6. Class ranking (sorted)\n"
         << " 7. Class statistics\n"
         << " 8. Save to file\n"
         << " 0. Save and exit\n";
}

int main() {
    loadFromFile();
    while (true) {
        showMenu();
        int choice = readInt("Choose an option: ", 0, 8);
        cout << "\n";
        switch (choice) {
            case 1: addStudent();     break;
            case 2: displayAll();     break;
            case 3: searchStudent();  break;
            case 4: updateScore();    break;
            case 5: deleteStudent();  break;
            case 6: showRanking();    break;
            case 7: classStatistics();break;
            case 8:
                if (saveToFile()) cout << "Saved to " << DATA_FILE << ".\n";
                break;
            case 0:
                if (saveToFile()) cout << "Saved. Goodbye!\n";
                return 0;
        }
    }
}
