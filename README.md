# Student Management System in C

A console-based student management system built using C programming language. This was made as a semester project to practice intermediate C concepts like structures, file handling, arrays, sorting and searching.

## What it does

Basically its a menu-driven program where you can manage student records. You can add students, view them, search, update, delete, sort and even save everything to a file so the data stays even after you close the program.

### Features

- Add new students with all their info (name, age, course, marks etc.)
- View all students in a table format
- Search students by ID or name
- Update student details
- Delete students
- Automatically calculates total marks, percentage and grade
- Sort students by ID, name or percentage
- View statistics (highest, lowest, average marks, pass/fail count)
- Save and load records from a binary file (`students.dat`)
- Input validation so the program doesnt crash on wrong input


On Windows:
```
gcc main.c -o student_mgmt.exe
student_mgmt.exe
```

Thats it. No external libraries needed, just standard C.

## Menu Options

```
========================================
       STUDENT MANAGEMENT SYSTEM
========================================

  1.  Add Student
  2.  Display All Students
  3.  Search Student
  4.  Update Student
  5.  Delete Student
  6.  Calculate Result
  7.  Sort Students
  8.  Student Statistics
  9.  Save Records
  10. Load Records
  11. Exit
```

## Grading System

| Percentage | Grade |
|-----------|-------|
| 90 - 100 | A+ |
| 80 - 89 | A |
| 70 - 79 | B |
| 60 - 69 | C |
| 50 - 59 | D |
| Below 50 | F |

50% and above = PASS, below 50% = FAIL

## File Structure

```
StudentManagementSystem/
├── main.c           <- all the source code
├── students.dat     <- created automatically when you save records
└── README.md
```

Everything is in a single file (`main.c`). The `students.dat` file gets created when you save records for the first time.

## C Concepts Used

- Structures and typedef
- Arrays (1D and array of structures)
- Strings and string functions (strlen, strcmp, strcat)
- Functions with parameters and return values
- Pointers (passing structs by reference)
- Enum for pass/fail status
- File handling (fopen, fclose, fread, fwrite)
- Linear search
- Bubble sort
- Input validation
- Switch-case, loops, if-else

## Known Limitations

- Maximum 100 students only (can be changed by editing MAX_STUDENTS)
- Name search is case-sensitive so you have to type the exact name
- No multi-file setup, everything is in one file
- Data file is binary so you cant read it in a text editor

## Built With

- C programming language

- VS Code / Turbo C++ (any editor works)

---

Made as a semester project. Feel free to use or modify it.
