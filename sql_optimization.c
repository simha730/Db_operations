#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 800
#define NAME_LEN 20

// Structure to represent an employee record
typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float salary;
} Employee;

// Function declarations
void generate_records(Employee *db);
void filter_records(Employee *db, int count, int min_age, float max_salary);
void group_by_age(Employee *db, int count);
void compute_average_salary(Employee *db, int count);
void sort_by_salary(Employee *db, int count);
int linear_search_salary(Employee *db, int count, float key);
int binary_search_salary(Employee *db, int count, float key);

// Generate random employee records
void generate_records(Employee *db) {
    for (int i = 0; i < NUM_RECORDS; i++) {
        db[i].id = i + 1;
        snprintf(db[i].name, NAME_LEN, "Emp_%03d", i + 1);
        db[i].age = 20 + rand() % 31;         // Age between 20–50
        db[i].salary = 30000 + rand() % 70000; // Salary between 30k–100k
    }
}

// Conditional filtering
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

    printf("\nTotal Matches: %d\n", matches);
}

// Group employees by age range (count)
void group_by_age(Employee *db, int count) {
    int g1 = 0, g2 = 0, g3 = 0;
    for (int i = 0; i < count; i++) {
        if (db[i].age >= 20 && db[i].age <= 30)
            g1++;
        else if (db[i].age >= 31 && db[i].age <= 40)
            g2++;
        else if (db[i].age >= 41 && db[i].age <= 50)
            g3++;
    }

    printf("\nEmployee Count by Age Range:\n");
    printf("-----------------------------\n");
    printf("20 - 30 : %d\n", g1);
    printf("31 - 40 : %d\n", g2);
    printf("41 - 50 : %d\n", g3);
}

// Compute average salary per age group
void compute_average_salary(Employee *db, int count) {
    float sum_20_30 = 0, sum_31_40 = 0, sum_41_50 = 0;
    int c1 = 0, c2 = 0, c3 = 0;

    for (int i = 0; i < count; i++) {
        if (db[i].age >= 20 && db[i].age <= 30) {
            sum_20_30 += db[i].salary;
            c1++;
        } else if (db[i].age >= 31 && db[i].age <= 40) {
            sum_31_40 += db[i].salary;
            c2++;
        } else if (db[i].age >= 41 && db[i].age <= 50) {
            sum_41_50 += db[i].salary;
            c3++;
        }
    }

    printf("\nAverage Salary by Age Range:\n");
    printf("-----------------------------\n");
    if (c1) printf("20 - 30 : %.2f\n", sum_20_30 / c1);
    if (c2) printf("31 - 40 : %.2f\n", sum_31_40 / c2);
    if (c3) printf("41 - 50 : %.2f\n", sum_41_50 / c3);
}

// Sort the database by salary (for binary search)
void sort_by_salary(Employee *db, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (db[j].salary > db[j + 1].salary) {
                Employee temp = db[j];
                db[j] = db[j + 1];
                db[j + 1] = temp;
            }
        }
    }
}

// Linear search by salary
int linear_search_salary(Employee *db, int count, float key) {
    for (int i = 0; i < count; i++) {
        if ((int)db[i].salary == (int)key)
            return i;
    }
    return -1;
}

// Binary search by salary (requires sorted array)
int binary_search_salary(Employee *db, int count, float key) {
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if ((int)db[mid].salary == (int)key)
            return mid;
        else if (db[mid].salary < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Main function
int main() {
    Employee db[NUM_RECORDS];
    srand((unsigned int)time(NULL));

    clock_t start, end;
    double total_time;

    // Data generation
    start = clock();
    generate_records(db);
    end = clock();
    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Data generation completed in %.6f seconds.\n", total_time);

    // Conditional filtering
    filter_records(db, NUM_RECORDS, 25, 50000.0);

    // Grouping operation
    group_by_age(db, NUM_RECORDS);

    // Average salary per age group
    compute_average_salary(db, NUM_RECORDS);

    // Search comparisons
    float target_salary = db[rand() % NUM_RECORDS].salary; // pick random salary

    printf("\nSearching for salary: %.2f\n", target_salary);

    // Linear search timing
    start = clock();
    int index_linear = linear_search_salary(db, NUM_RECORDS, target_salary);
    end = clock();
    double linear_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Sort for binary search
    sort_by_salary(db, NUM_RECORDS);

    // Binary search timing
    start = clock();
    int index_binary = binary_search_salary(db, NUM_RECORDS, target_salary);
    end = clock();
    double binary_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print results
    printf("\nSearch Results:\n");
    printf("-------------------------\n");
    if (index_linear != -1)
        printf("Linear Search Found: ID=%d, Name=%s, Salary=%.2f\n",
               db[index_linear].id, db[index_linear].name, db[index_linear].salary);
    else
        printf("Linear Search: Record not found.\n");

    if (index_binary != -1)
        printf("Binary Search Found: ID=%d, Name=%s, Salary=%.2f\n",
               db[index_binary].id, db[index_binary].name, db[index_binary].salary);
    else
        printf("Binary Search: Record not found.\n");

    printf("\nExecution Time Comparison:\n");
    printf("----------------------------\n");
    printf("Linear Search Time : %.8f sec\n", linear_time);
    printf("Binary Search Time : %.8f sec\n", binary_time);

    return 0;
}
