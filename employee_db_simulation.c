#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 800
#define NAME_LEN 20

// Structure to represent a database record
typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float salary;
} Employee;

// Function to generate random employee data
void generate_records(Employee *db) {
    for (int i = 0; i < NUM_RECORDS; i++) {
        db[i].id = i + 1;
        snprintf(db[i].name, NAME_LEN, "Emp_%03d", i + 1);
        db[i].age = 20 + rand() % 31;         // Age between 20 and 50
        db[i].salary = 30000 + rand() % 70000; // Salary between 30,000 and 100,000
    }
}

// Function to filter employees by given conditions
void filter_records(Employee *db, int count, int min_age, float max_salary) {
    printf("\nFiltered Records (age > %d && salary < %.2f):\n", min_age, max_salary);
    printf("--------------------------------------------------\n");
    printf("%-5s %-10s %-5s %-10s\n", "ID", "Name", "Age", "Salary");

    int matches = 0;
    for (int i = 0; i < count; i++) {
        if (db[i].age > min_age && db[i].salary < max_salary) {
            printf("%-5d %-10s %-5d %-10.2f\n", db[i].id, db[i].name, db[i].age, db[i].salary);
            matches++;
        }
    }

    if (matches == 0)
        printf("No records match the given conditions.\n");
    else
        printf("\nTotal Matches: %d\n", matches);
}

// Function to group employees into age ranges
void group_by_age(Employee *db, int count) {
    int group_20_30 = 0, group_31_40 = 0, group_41_50 = 0;

    for (int i = 0; i < count; i++) {
        if (db[i].age >= 20 && db[i].age <= 30)
            group_20_30++;
        else if (db[i].age >= 31 && db[i].age <= 40)
            group_31_40++;
        else if (db[i].age >= 41 && db[i].age <= 50)
            group_41_50++;
    }

    printf("\nEmployee Count by Age Range:\n");
    printf("-----------------------------\n");
    printf("20 - 30 : %d\n", group_20_30);
    printf("31 - 40 : %d\n", group_31_40);
    printf("41 - 50 : %d\n", group_41_50);
}

// Main function
int main() {
    Employee db[NUM_RECORDS];
    srand((unsigned int)time(NULL));

    // Measure start time
    clock_t start = clock();

    // Generate random data
    generate_records(db);

    // Perform conditional filtering
    filter_records(db, NUM_RECORDS, 25, 50000.0);

    // Perform grouping
    group_by_age(db, NUM_RECORDS);

    // Measure end time
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nExecution Time: %.6f seconds\n", execution_time);

    return 0;
}
