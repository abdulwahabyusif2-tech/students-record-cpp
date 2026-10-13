# Student Records and Grade Application (C++)

A console application for managing student records and grades, built to demonstrate **arrays as a data structure** in C++.

## Features

- Add, view, search, update and delete student records
- Automatic average and letter grade (A-F) for every student
- Class ranking using selection sort
- Class statistics: class average, top and lowest student, per-course average/highest/lowest, grade distribution
- Records are saved to `students.txt` and loaded again on the next run

## How arrays are used

| Array | Purpose |
|---|---|
| `Student students[MAX_STUDENTS]` | Main array holding all student records (capacity 100) |
| `double scores[NUM_COURSES]` (inside each `Student`) | Scores for the 5 courses |
| `COURSE_NAMES[]` / `SHORT_NAMES[]` | Course labels used for prompts and table headers |
| `courseTotals[]`, `courseHigh[]`, `courseLow[]`, `gradeCounts[]` | Working arrays for the statistics report |

### Array operations demonstrated

| Operation | Where | Cost |
|---|---|---|
| Access by index | `students[i]`, `scores[c]` | O(1) |
| Insert at end | `addStudent()` | O(1) |
| Linear search by ID | `findStudentIndex()` | O(n) |
| Delete (shift elements left) | `deleteStudent()` | O(n) |
| Selection sort | `sortByAverage()` | O(n^2) |
| Traverse with loops | `displayAll()`, `classStatistics()` | O(n) |

## Build and run

Requires a C++11 compiler (g++, clang++ or MSVC).

```bash
g++ -std=c++11 -Wall -Wextra -o student_records main.cpp
./student_records
```

On Windows (MinGW): `student_records.exe`

## Grading scale

| Average | Grade |
|---|---|
| 80 and above | A |
| 70 - 79.9 | B |
| 60 - 69.9 | C |
| 50 - 59.9 | D |
| below 50 | F |

## Data file format

`students.txt` stores one student per line:

```
id,name,score1,score2,score3,score4,score5
```

## Limitations and ideas for improvement

- The array has a fixed capacity of 100 students (a deliberate property of arrays). Replace it with `std::vector` for dynamic sizing.
- Names cannot contain commas because of the CSV-style file format.
- Possible extensions: edit student names, search by name, export a report, unit tests.
