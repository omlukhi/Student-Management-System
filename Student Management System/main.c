/*
    Student Management System
    -------------------------
    A simple console-based program to manage student records.
    Uses structures, file handling, arrays, and basic sorting/searching.

    Compiled with: gcc -Wall main.c -o student_mgmt
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define SUBJECTS 5
#define DATA_FILE "students.dat"

// enum for pass/fail status
typedef enum {
    FAIL,
    PASS
} StudentStatus;

// main student structure
typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
    char course[50];
    int semester;
    float marks[SUBJECTS];
    float total;
    float percentage;
    char grade;
    StudentStatus status;
} Student;

// global array to hold all students
Student students[MAX_STUDENTS];
int studentCount = 0;

// subject names for display purposes
const char *subjectNames[SUBJECTS] = {
    "Subject 1", "Subject 2", "Subject 3",
    "Subject 4", "Subject 5"
};

/* ---- Function Prototypes ---- */

// menu operations
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
void sortStudents(void);
void studentStatistics(void);
void saveStudents(void);
void loadStudents(void);

// result calculation
void calculateResult(Student *student);
char calculateGrade(float percentage);

// searching
int findStudentById(int id);
int findStudentByName(char name[]);

// display helpers
void displaySingleStudent(const Student *s);
void displayStudentBrief(const Student *s);

// input and utility
void clearInputBuffer(void);
int readInt(const char *prompt);
float readFloat(const char *prompt);
void readString(const char *prompt, char *dest, int maxLen);
int isDuplicateId(int id);
void displayMenu(void);
void pressEnterToContinue(void);
void printSeparator(void);
void printHeader(const char *title);


// ==================== MAIN ====================

int main(void)
{
    int choice;

    // try loading saved data when program starts
    loadStudents();

    do {
        displayMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            updateStudent();
            break;
        case 5:
            deleteStudent();
            break;
        case 6: {
            // calculate result for one student
            int id, idx;
            printHeader("CALCULATE RESULT");
            id = readInt("Enter Student ID: ");
            idx = findStudentById(id);
            if (idx == -1) {
                printf("\n  Student not found.\n");
            } else {
                calculateResult(&students[idx]);
                printf("\n  Result calculated successfully.\n");
                displaySingleStudent(&students[idx]);
            }
            pressEnterToContinue();
            break;
        }
        case 7:
            sortStudents();
            break;
        case 8:
            studentStatistics();
            break;
        case 9:
            saveStudents();
            break;
        case 10:
            loadStudents();
            break;
        case 11:
            printSeparator();
            printf("  Exiting... Goodbye!\n");
            printSeparator();
            break;
        default:
            printf("\n  Error: Invalid choice. Please enter 1-11.\n");
            pressEnterToContinue();
            break;
        }

    } while (choice != 11);

    return 0;
}


// ==================== MENU & UI STUFF ====================

void printSeparator(void)
{
    printf("========================================\n");
}

void printHeader(const char *title)
{
    printf("\n");
    printSeparator();
    printf("       %s\n", title);
    printSeparator();
}

void displayMenu(void)
{
    printHeader("STUDENT MANAGEMENT SYSTEM");
    printf("\n");
    printf("  1.  Add Student\n");
    printf("  2.  Display All Students\n");
    printf("  3.  Search Student\n");
    printf("  4.  Update Student\n");
    printf("  5.  Delete Student\n");
    printf("  6.  Calculate Result\n");
    printf("  7.  Sort Students\n");
    printf("  8.  Student Statistics\n");
    printf("  9.  Save Records\n");
    printf("  10. Load Records\n");
    printf("  11. Exit\n\n");
}

void pressEnterToContinue(void)
{
    printf("\n  Press Enter to continue...");
    clearInputBuffer();
    getchar();
}


// ==================== INPUT FUNCTIONS ====================

// clears any leftover characters in the input buffer
void clearInputBuffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ; // just throw them away
}

// keeps asking until user gives a valid integer
int readInt(const char *prompt)
{
    int value, result;

    while (1) {
        printf("  %s", prompt);
        result = scanf("%d", &value);
        if (result == 1) {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("  Error: Please enter a valid integer.\n");
    }
}

// same thing but for float values
float readFloat(const char *prompt)
{
    float value;
    int result;

    while (1) {
        printf("  %s", prompt);
        result = scanf("%f", &value);
        if (result == 1) {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("  Error: Please enter a valid number.\n");
    }
}

// reads a string with spaces allowed, wont accept empty input
void readString(const char *prompt, char *dest, int maxLen)
{
    while (1) {
        printf("  %s", prompt);
        if (fgets(dest, maxLen, stdin) != NULL) {
            // get rid of the newline at the end
            int len = (int)strlen(dest);
            if (len > 0 && dest[len - 1] == '\n') {
                dest[len - 1] = '\0';
                len--;
            }
            if (len > 0)
                return;
        }
        printf("  Error: Input cannot be empty.\n");
    }
}


// ==================== HELPER FUNCTIONS ====================

// checks if a student with this ID already exists
int isDuplicateId(int id)
{
    int i;
    for (i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return 1; // yes its a duplicate
    }
    return 0;
}

// calculates total, percentage, grade and pass/fail
void calculateResult(Student *student)
{
    int i;
    float sum = 0.0f;

    for (i = 0; i < SUBJECTS; i++) {
        sum += student->marks[i];
    }

    student->total = sum;
    student->percentage = sum / (float)SUBJECTS;
    student->grade = calculateGrade(student->percentage);

    // 50% or above is pass
    if (student->percentage >= 50.0f)
        student->status = PASS;
    else
        student->status = FAIL;
}

// returns letter grade based on percentage
char calculateGrade(float percentage)
{
    if (percentage >= 90.0f)
        return 'O'; // A+ Outstanding
    else if (percentage >= 80.0f)
        return 'A';
    else if (percentage >= 70.0f)
        return 'B';
    else if (percentage >= 60.0f)
        return 'C';
    else if (percentage >= 50.0f)
        return 'D';
    else
        return 'F';
}


// ==================== DISPLAY FUNCTIONS ====================

// prints all the details of a single student
void displaySingleStudent(const Student *s)
{
    int i;
    char statusStr[10];

    // build the status string using strcat
    statusStr[0] = '\0';
    if (s->status == PASS)
        strcat(statusStr, "PASS");
    else
        strcat(statusStr, "FAIL");

    printf("\n  ---- Student Details ----\n");
    printf("  ID          : %d\n", s->id);
    printf("  Name        : %s\n", s->name);
    printf("  Age         : %d\n", s->age);
    printf("  Gender      : %s\n", s->gender);
    printf("  Course      : %s\n", s->course);
    printf("  Semester    : %d\n", s->semester);

    printf("  Marks       : ");
    for (i = 0; i < SUBJECTS; i++) {
        printf("%.1f", s->marks[i]);
        if (i < SUBJECTS - 1)
            printf(", ");
    }
    printf("\n");

    printf("  Total       : %.1f\n", s->total);
    printf("  Percentage  : %.2f%%\n", s->percentage);

    // show grade with label for A+
    printf("  Grade       : ");
    switch (s->grade) {
    case 'O': printf("A+ (Outstanding)\n"); break;
    case 'A': printf("A\n"); break;
    case 'B': printf("B\n"); break;
    case 'C': printf("C\n"); break;
    case 'D': printf("D\n"); break;
    case 'F': printf("F\n"); break;
    default:  printf("%c\n", s->grade); break;
    }

    printf("  Status      : %s\n", statusStr);
}

// one-line summary for the table view
void displayStudentBrief(const Student *s)
{
    printf("  %-6d %-20s %-18s %-8d %-10.2f %-6c %-6s\n",
        s->id, s->name, s->course, s->semester,
        s->percentage, s->grade,
        (s->status == PASS) ? "PASS" : "FAIL");
}


// ==================== ADD STUDENT ====================

void addStudent(void)
{
    Student newStudent;
    int id, age, semester, i;
    float mark;

    printHeader("ADD STUDENT");

    // check if we have room
    if (studentCount >= MAX_STUDENTS) {
        printf("\n  Error: Maximum student capacity (%d) reached.\n", MAX_STUDENTS);
        pressEnterToContinue();
        return;
    }

    // get student ID (must be positive and unique)
    while (1) {
        id = readInt("Enter Student ID: ");
        if (id <= 0) {
            printf("  Error: Student ID must be a positive integer.\n");
            continue;
        }
        if (isDuplicateId(id)) {
            printf("  Error: Student ID %d already exists.\n", id);
            continue;
        }
        break;
    }
    newStudent.id = id;

    // get name
    readString("Enter Student Name: ", newStudent.name, sizeof(newStudent.name));

    // get age (1-100)
    while (1) {
        age = readInt("Enter Age: ");
        if (age < 1 || age > 100) {
            printf("  Error: Age must be between 1 and 100.\n");
            continue;
        }
        break;
    }
    newStudent.age = age;

    // gender
    readString("Enter Gender (Male/Female/Other): ", newStudent.gender, sizeof(newStudent.gender));

    // course name
    readString("Enter Course: ", newStudent.course, sizeof(newStudent.course));

    // semester (1 to 8)
    while (1) {
        semester = readInt("Enter Semester (1-8): ");
        if (semester < 1 || semester > 8) {
            printf("  Error: Semester must be between 1 and 8.\n");
            continue;
        }
        break;
    }
    newStudent.semester = semester;

    // enter marks for each subject
    printf("\n  Enter marks for %d subjects:\n", SUBJECTS);
    for (i = 0; i < SUBJECTS; i++) {
        while (1) {
            char prompt[40];
            sprintf(prompt, "%s (0-100): ", subjectNames[i]);
            mark = readFloat(prompt);
            if (mark < 0.0f || mark > 100.0f) {
                printf("  Error: Invalid marks. Enter a value between 0 and 100.\n");
                continue;
            }
            break;
        }
        newStudent.marks[i] = mark;
    }

    // calculate the result automatically
    calculateResult(&newStudent);

    // save to array
    students[studentCount] = newStudent;
    studentCount++;

    printf("\n  Student added successfully! (Total students: %d)\n", studentCount);
    pressEnterToContinue();
}


// ==================== DISPLAY ALL STUDENTS ====================

void displayStudents(void)
{
    int i, viewChoice;

    printHeader("DISPLAY ALL STUDENTS");

    if (studentCount == 0) {
        printf("\n  No student records found.\n");
        pressEnterToContinue();
        return;
    }

    // print table header
    printf("\n  %-6s %-20s %-18s %-8s %-10s %-6s %-6s\n",
        "ID", "Name", "Course", "Sem", "Percent", "Grade", "Status");
    printf("  ");
    for (i = 0; i < 76; i++) printf("-");
    printf("\n");

    for (i = 0; i < studentCount; i++) {
        displayStudentBrief(&students[i]);
    }
    printf("\n  Total records: %d\n", studentCount);

    // let user view detailed info if they want
    printf("\n  View full details of a student? (Enter ID, or 0 to skip): ");
    viewChoice = readInt("");
    if (viewChoice > 0) {
        int idx = findStudentById(viewChoice);
        if (idx == -1)
            printf("  Student not found.\n");
        else
            displaySingleStudent(&students[idx]);
    }

    pressEnterToContinue();
}


// ==================== SEARCH STUDENT ====================

// linear search - find student by ID, returns index or -1
int findStudentById(int id)
{
    int i;
    for (i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

// linear search - find student by name using strcmp
int findStudentByName(char name[])
{
    int i;
    for (i = 0; i < studentCount; i++) {
        if (strcmp(students[i].name, name) == 0)
            return i;
    }
    return -1;
}

void searchStudent(void)
{
    int searchChoice, idx;

    printHeader("SEARCH STUDENT");

    if (studentCount == 0) {
        printf("\n  No student records found.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n  1. Search by Student ID\n");
    printf("  2. Search by Student Name\n\n");
    searchChoice = readInt("Enter search option: ");

    switch (searchChoice) {
    case 1: {
        int id = readInt("Enter Student ID: ");
        idx = findStudentById(id);
        if (idx == -1)
            printf("\n  Student not found.\n");
        else
            displaySingleStudent(&students[idx]);
        break;
    }
    case 2: {
        char name[50];
        readString("Enter Student Name: ", name, sizeof(name));
        idx = findStudentByName(name);
        if (idx == -1)
            printf("\n  Student not found.\n");
        else
            displaySingleStudent(&students[idx]);
        break;
    }
    default:
        printf("\n  Error: Invalid search option.\n");
        break;
    }

    pressEnterToContinue();
}


// ==================== UPDATE STUDENT ====================

void updateStudent(void)
{
    int id, idx, i;
    int age, semester;
    float mark;

    printHeader("UPDATE STUDENT");

    if (studentCount == 0) {
        printf("\n  No student records found.\n");
        pressEnterToContinue();
        return;
    }

    id = readInt("Enter Student ID to update: ");
    idx = findStudentById(id);

    if (idx == -1) {
        printf("\n  Student not found.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n  Current details:\n");
    displaySingleStudent(&students[idx]);
    printf("\n  Enter new details:\n\n");

    // update name
    readString("Enter new Name: ", students[idx].name, sizeof(students[idx].name));

    // update age
    while (1) {
        age = readInt("Enter new Age: ");
        if (age < 1 || age > 100) {
            printf("  Error: Age must be between 1 and 100.\n");
            continue;
        }
        break;
    }
    students[idx].age = age;

    // update gender
    readString("Enter new Gender: ", students[idx].gender, sizeof(students[idx].gender));

    // update course
    readString("Enter new Course: ", students[idx].course, sizeof(students[idx].course));

    // update semester
    while (1) {
        semester = readInt("Enter new Semester (1-8): ");
        if (semester < 1 || semester > 8) {
            printf("  Error: Semester must be between 1 and 8.\n");
            continue;
        }
        break;
    }
    students[idx].semester = semester;

    // update marks
    printf("\n  Enter new marks for %d subjects:\n", SUBJECTS);
    for (i = 0; i < SUBJECTS; i++) {
        while (1) {
            char prompt[40];
            sprintf(prompt, "%s (0-100): ", subjectNames[i]);
            mark = readFloat(prompt);
            if (mark < 0.0f || mark > 100.0f) {
                printf("  Error: Invalid marks. Enter a value between 0 and 100.\n");
                continue;
            }
            break;
        }
        students[idx].marks[i] = mark;
    }

    // recalculate everything after update
    calculateResult(&students[idx]);

    printf("\n  Student updated successfully!\n");
    displaySingleStudent(&students[idx]);
    pressEnterToContinue();
}


// ==================== DELETE STUDENT ====================

void deleteStudent(void)
{
    int id, idx, i;
    char confirm[5];

    printHeader("DELETE STUDENT");

    if (studentCount == 0) {
        printf("\n  No student records found.\n");
        pressEnterToContinue();
        return;
    }

    id = readInt("Enter Student ID to delete: ");
    idx = findStudentById(id);

    if (idx == -1) {
        printf("\n  Student not found.\n");
        pressEnterToContinue();
        return;
    }

    displaySingleStudent(&students[idx]);

    readString("\n  Are you sure you want to delete this student? (Y/N): ",
        confirm, sizeof(confirm));

    if (confirm[0] == 'Y' || confirm[0] == 'y') {
        // shift everyone after this index down by one
        for (i = idx; i < studentCount - 1; i++) {
            students[i] = students[i + 1];
        }
        studentCount--;
        printf("\n  Student deleted successfully. (Remaining: %d)\n", studentCount);
    } else {
        printf("\n  Deletion cancelled.\n");
    }

    pressEnterToContinue();
}


// ==================== SORT STUDENTS ====================

void sortStudents(void)
{
    int sortChoice, i, j;
    Student temp;

    printHeader("SORT STUDENTS");

    if (studentCount < 2) {
        printf("\n  Not enough students to sort. (Need at least 2)\n");
        pressEnterToContinue();
        return;
    }

    printf("\n  1. Sort by Student ID    (Ascending)\n");
    printf("  2. Sort by Name          (Ascending)\n");
    printf("  3. Sort by Percentage    (Highest to Lowest)\n\n");
    sortChoice = readInt("Enter sort option: ");

    switch (sortChoice) {
    case 1:
        // bubble sort by ID - ascending order
        for (i = 0; i < studentCount - 1; i++) {
            for (j = 0; j < studentCount - 1 - i; j++) {
                if (students[j].id > students[j + 1].id) {
                    temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\n  Students sorted by ID (Ascending).\n");
        break;

    case 2:
        // bubble sort by Name - ascending, using strcmp
        for (i = 0; i < studentCount - 1; i++) {
            for (j = 0; j < studentCount - 1 - i; j++) {
                if (strcmp(students[j].name, students[j + 1].name) > 0) {
                    temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\n  Students sorted by Name (Ascending).\n");
        break;

    case 3:
        // bubble sort by percentage - highest first
        for (i = 0; i < studentCount - 1; i++) {
            for (j = 0; j < studentCount - 1 - i; j++) {
                if (students[j].percentage < students[j + 1].percentage) {
                    temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\n  Students sorted by Percentage (Highest to Lowest).\n");
        break;

    default:
        printf("\n  Error: Invalid sort option.\n");
        pressEnterToContinue();
        return;
    }

    // show the sorted list
    printf("\n  %-6s %-20s %-18s %-8s %-10s %-6s %-6s\n",
        "ID", "Name", "Course", "Sem", "Percent", "Grade", "Status");
    printf("  ");
    for (i = 0; i < 76; i++) printf("-");
    printf("\n");
    for (i = 0; i < studentCount; i++) {
        displayStudentBrief(&students[i]);
    }

    pressEnterToContinue();
}


// ==================== STUDENT STATISTICS ====================

void studentStatistics(void)
{
    int i;
    float highest, lowest, sum, average;
    int passedCount = 0, failedCount = 0;

    printHeader("STUDENT STATISTICS");

    if (studentCount == 0) {
        printf("\n  No student records found.\n");
        pressEnterToContinue();
        return;
    }

    highest = students[0].percentage;
    lowest = students[0].percentage;
    sum = 0.0f;

    for (i = 0; i < studentCount; i++) {
        float pct = students[i].percentage;
        sum += pct;

        if (pct > highest)
            highest = pct;
        if (pct < lowest)
            lowest = pct;

        if (students[i].status == PASS)
            passedCount++;
        else
            failedCount++;
    }

    average = sum / (float)studentCount;

    printf("\n");
    printf("  Total Students       : %d\n", studentCount);
    printf("  Highest Percentage   : %.2f%%\n", highest);
    printf("  Lowest Percentage    : %.2f%%\n", lowest);
    printf("  Average Percentage   : %.2f%%\n", average);
    printf("  Passed Students      : %d\n", passedCount);
    printf("  Failed Students      : %d\n", failedCount);

    pressEnterToContinue();
}


// ==================== SAVE RECORDS ====================

void saveStudents(void)
{
    FILE *fp;

    printHeader("SAVE RECORDS");

    fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("\n  Error: Could not open file '%s' for writing.\n", DATA_FILE);
        pressEnterToContinue();
        return;
    }

    // first write how many students we have
    fwrite(&studentCount, sizeof(int), 1, fp);

    // then write all the student data
    if (studentCount > 0) {
        fwrite(students, sizeof(Student), (size_t)studentCount, fp);
    }

    fclose(fp);
    printf("\n  Student records saved successfully. (%d records)\n", studentCount);
    pressEnterToContinue();

    /*
     * Note: We could also use text file functions like this:
     *
     *   FILE *f = fopen("students.txt", "w");
     *   fprintf(f, "%d\n", studentCount);
     *   for (int i = 0; i < studentCount; i++) {
     *       fprintf(f, "%d\n", students[i].id);
     *       fputs(students[i].name, f);
     *       fprintf(f, "\n");
     *   }
     *   fclose(f);
     *
     * And to read back:
     *   FILE *f = fopen("students.txt", "r");
     *   fscanf(f, "%d", &studentCount);
     *   for (int i = 0; i < studentCount; i++) {
     *       fscanf(f, "%d", &students[i].id);
     *       fgets(students[i].name, 50, f);
     *   }
     *   fclose(f);
     *
     * But binary is simpler for this project.
     */
}


// ==================== LOAD RECORDS ====================

void loadStudents(void)
{
    FILE *fp;
    int loadedCount = 0;

    printf("\n  Loading student records...\n");

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        // file doesn't exist yet, thats fine on first run
        printf("  No existing data file found. Starting with empty records.\n");
        return;
    }

    // read student count first
    if (fread(&loadedCount, sizeof(int), 1, fp) != 1) {
        printf("  Warning: Data file is empty or corrupt. Starting fresh.\n");
        fclose(fp);
        return;
    }

    // make sure the count makes sense
    if (loadedCount < 0 || loadedCount > MAX_STUDENTS) {
        printf("  Warning: Invalid record count in file. Starting fresh.\n");
        fclose(fp);
        return;
    }

    // now read all the student records
    if (loadedCount > 0) {
        size_t numRead = fread(students, sizeof(Student), (size_t)loadedCount, fp);
        if ((int)numRead != loadedCount) {
            printf("  Warning: Could only read %d of %d records.\n",
                (int)numRead, loadedCount);
            studentCount = (int)numRead;
        } else {
            studentCount = loadedCount;
        }
    } else {
        studentCount = 0;
    }

    fclose(fp);
    printf("  Student records loaded successfully. (%d records)\n", studentCount);
}
