#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 50
#define MAX_STAFF 20
#define SUBJECTS 5
#define DAYS 5

typedef struct {
    int id;
    char name[50];
    int age;
    char grade[10];
    float feesPaid;
    int attendance;
    float marks[SUBJECTS];
    char timetable[DAYS][20];
    char signature[50];

    char borrowedBook[50];
    int hasBook;
} Student;

typedef struct {
    int id;
    char name[50];
    float salary;
    float bonus;
    float deductions;
    char role[50];
    char department[30];
} Staff;


void registerStudent(Student *s, int id, char name[], int age, char grade[]);
void borrowBook(Student *s, char bookName[]);
void showStudentInfo(Student s);
void analyzePerformance(Student s);
float calculateNetSalary(Staff st);
void recordFee(Student *s, float amount);
void markAttendance(Student *s, int percent);

int main() {
    Student students[MAX_STUDENTS];
    Staff staff[MAX_STAFF];
    int studentCount = 0, staffCount = 0;
    int choice, searchId;

    do {
        printf("\n=======| SCHOOL MANAGEMENT SYSTEM |=======\n");
        printf("1. Register Student   ||    5. Show Student Info\n");
        printf("2. Record Fee         ||    6. Register Staff\n");
        printf("3. Mark Attendance    ||    7. Show Staff Info\n");
        printf("4. Record Marks       ||    8. Borrow Library Book\n");
        printf("0. Exit               ||\n");          
         printf("==================================================\n");
        printf("| Enter choice: ");
        scanf("%d", &choice);
       

        switch(choice) {

            case 1:
                if(studentCount < MAX_STUDENTS) {
                    int id, age;
                    char name[50], grade[10];

                    printf("Enter ID, Name, Age, Grade: ");
                    scanf("%d %s %d %s", &id, name, &age, grade);

                    registerStudent(&students[studentCount], id, name, age, grade);
                    studentCount++;
                }
                break;

            case 2:
                printf("Enter Student ID & Fee: ");
                float fee;
                scanf("%d %f", &searchId, &fee);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].id == searchId) {
                        recordFee(&students[i], fee);
                    }
                }
                break;

            case 3: 
                printf("Enter Student ID & Attendance %%: ");
                int att;
                scanf("%d %d", &searchId, &att);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].id == searchId) {
                        markAttendance(&students[i], att);
                    }
                }
                break;

            case 4:
                printf("Enter Student ID: ");
                scanf("%d", &searchId);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].id == searchId) {
                        printf("Enter 5 marks: ");
                        for(int j=0; j<5; j++) scanf("%f", &students[i].marks[j]);
                        analyzePerformance(students[i]);
                    }
                }
                break;

            case 5:
                printf("Enter Student ID: ");
                scanf("%d", &searchId);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].id == searchId) {
                        showStudentInfo(students[i]);
                    }
                }
                break;

            case 6:
                if(staffCount < MAX_STAFF) {
                    printf("Enter ID, Name, Salary, Dept: ");
                    scanf("%d %s %f %s", &staff[staffCount].id,
                          staff[staffCount].name,
                          &staff[staffCount].salary,
                          staff[staffCount].department);

                    staff[staffCount].bonus = 1500;
                    staff[staffCount].deductions = 300;
                    strcpy(staff[staffCount].role, "Faculty");

                    staffCount++;
                }
                break;

            case 7:
                for (int i = 0; i < staffCount; i++) {
                    printf("\nStaff: %s | Dept: %s | Net: %.2f",
                           staff[i].name,
                           staff[i].department,
                           calculateNetSalary(staff[i]));
                }
                printf("\n");
                break;

            case 8: {
                char bName[50];
                printf("Enter Student ID & Book Name: ");
                scanf("%d %s", &searchId, bName);

                for (int i = 0; i < studentCount; i++) {
                    if (students[i].id == searchId) {
                        borrowBook(&students[i], bName);
                    }
                }
                break;
            }
        }

    } while (choice != 0);

    return 0;
}



void registerStudent(Student *s, int id, char name[], int age, char grade[]) {
    s->id = id;
    strcpy(s->name, name);
    s->age = age;
    strcpy(s->grade, grade);
    s->feesPaid = 0;
    s->attendance = 0;
    s->hasBook = 0;

    for(int i=0;i<5;i++) s->marks[i] = 0;

    strcpy(s->signature, "Authorized");

    printf("Student %s registered successfully!\n", name);
}

void recordFee(Student *s, float amount) {
    s->feesPaid += amount;
    printf("Fee updated. Total: %.2f\n", s->feesPaid);
}

void markAttendance(Student *s, int percent) {
    s->attendance = percent;
    printf("Attendance updated: %d%%\n", percent);
}

void borrowBook(Student *s, char bookName[]) {
    if(s->hasBook) {
        printf("%s already has a book. Return first!\n", s->name);
        return;
    }
    strcpy(s->borrowedBook, bookName);
    s->hasBook = 1;
    printf("Book '%s' issued to %s\n", bookName, s->name);
}

void analyzePerformance(Student s) {
    float sum = 0;
    for(int i=0; i<5; i++) sum += s.marks[i];

    float avg = sum/5;

    printf("Result: %s (Avg: %.2f)\n",
           (avg >= 40) ? "PASS" : "FAIL", avg);
}

void showStudentInfo(Student s) {
    printf("\n---| Profile: %s |---\n", s.name);
    printf("ID: %d | Grade: %s | Attendance: %d%%\n",
           s.id, s.grade, s.attendance);

    printf("Fees Paid: %.2f\n", s.feesPaid);

    if(s.hasBook)
        printf("Library: '%s'\n", s.borrowedBook);
    else
        printf("Library: No books\n");

    printf("Status: %s\n", s.signature);
}

float calculateNetSalary(Staff st) {
    return st.salary + st.bonus - st.deductions;
}