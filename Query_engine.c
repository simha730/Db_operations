#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* -------------------------------
   Data model: one table row
---------------------------------*/
typedef struct {
    int id;
    const char* name;
    int age;
    int salary;
} Row;

/* -------------------------------
   Iterator interface
---------------------------------*/

typedef struct Iterator Iterator;

struct Iterator {
    void   (*open)(Iterator* self);
    bool   (*next)(Iterator* self, Row* out);
    void   (*close)(Iterator* self);
    void*   impl;             // internal state
};

/* -------------------------------
   SeqScan operator
---------------------------------*/
typedef struct {
    Row* table;
    size_t size;
    size_t cursor;
} SeqScanState;

void seqscan_open(Iterator* it) {
    ((SeqScanState*) it->impl)->cursor = 0;
}

bool seqscan_next(Iterator* it, Row* out) {
    SeqScanState* st = it->impl;
    if (st->cursor >= st->size)
        return false;
    *out = st->table[st->cursor++];
    return true;
}

void seqscan_close(Iterator* it) {}

Iterator SeqScan(Row* table, size_t size) {
    SeqScanState* st = malloc(sizeof(SeqScanState));
    st->table = table;
    st->size  = size;
    st->cursor = 0;

    return (Iterator){
        .open  = seqscan_open,
        .next  = seqscan_next,
        .close = seqscan_close,
        .impl  = st
    };
}

/* -------------------------------
   Filter operator
---------------------------------*/
typedef bool (*Predicate)(const Row*);

typedef struct {
    Iterator* child;
    Predicate pred;
} FilterState;

void filter_open(Iterator* it) {
    ((FilterState*) it->impl)->child->open(((FilterState*) it->impl)->child);
}

bool filter_next(Iterator* it, Row* out) {
    FilterState* st = it->impl;
    Row tmp;
    while (st->child->next(st->child, &tmp)) {
        if (st->pred(&tmp)) {
            *out = tmp;
            return true;
        }
    }
    return false;
}

void filter_close(Iterator* it) {
    ((FilterState*) it->impl)->child->close(((FilterState*) it->impl)->child);
}

Iterator Filter(Iterator* child, Predicate p) {
    FilterState* st = malloc(sizeof(FilterState));
    st->child = child;
    st->pred  = p;

    return (Iterator){
        .open  = filter_open,
        .next  = filter_next,
        .close = filter_close,
        .impl  = st
    };
}

/* -------------------------------
   Project operator
---------------------------------*/
typedef void (*ProjectFunc)(Row* dst, const Row* src);

typedef struct {
    Iterator* child;
    ProjectFunc func;
} ProjectState;

void project_open(Iterator* it) {
    ((ProjectState*) it->impl)->child->open(((ProjectState*) it->impl)->child);
}

bool project_next(Iterator* it, Row* out) {
    ProjectState* st = it->impl;
    Row src;
    if (!st->child->next(st->child, &src))
        return false;

    st->func(out, &src);
    return true;
}

void project_close(Iterator* it) {
    ((ProjectState*) it->impl)->child->close(((ProjectState*) it->impl)->child);
}

Iterator Project(Iterator* child, ProjectFunc f) {
    ProjectState* st = malloc(sizeof(ProjectState));
    st->child = child;
    st->func  = f;

    return (Iterator){
        .open  = project_open,
        .next  = project_next,
        .close = project_close,
        .impl  = st
    };
}

/* -------------------------------
   Demo usage
---------------------------------*/

// Filter predicate: age > 30
bool pred_age_gt_30(const Row* r) {
    return r->age > 30;
}

// Projection: keep id and name, bump salary 10%
void project_bump_salary(Row* dst, const Row* src) {
    dst->id = src->id;
    dst->name = src->name;
    dst->age = src->age;
    dst->salary = src->salary * 1.10;
}

/* -------------------------------
   Main / Query Example
---------------------------------*/

int main() {
    Row table[] = {
        {1, "Alice", 34,  90000},
        {2, "Bob",   28,  60000},
        {3, "Carol", 41, 120000},
        {4, "Derek", 23,  45000},
        {5, "Eve",   37,  98000},
    };

    Iterator scan   = SeqScan(table, 5);
    Iterator filter = Filter(&scan, pred_age_gt_30);
    Iterator proj   = Project(&filter, project_bump_salary);

    proj.open(&proj);
    Row r;

    printf("Query results (age > 30):\n");
    while (proj.next(&proj, &r)) {
        printf("  id=%d  name=%s  new_salary=%d\n", r.id, r.name, r.salary);
    }
    proj.close(&proj);

    return 0;
}
