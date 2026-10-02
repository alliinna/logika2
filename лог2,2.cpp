#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

void shell(int* items, int count)
{
    int i, j, gap, k;
    int x, a[5] = { 9, 5, 3, 2, 1 };

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (x < items[j]) && (j >= 0); j -= gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right)
{
    int i = left, j = right;
    int x = items[left + rand() % (right - left + 1)];

    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left))  j--;
        if (i <= j) {
            int y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++; j--;
        }
    } while (i <= j);

    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

int cmp(const void* a, const void* b)
{
    return (*(int*)a) - (*(int*)b);
}

void fill_random(int* a, int n)
{
    srand(1);
    for (int i = 0; i < n; i++) a[i] = rand();
}
void fill_ascending(int* a, int n)
{
    for (int i = 0; i < n; i++) a[i] = i;
}
void fill_descending(int* a, int n)
{
    for (int i = 0; i < n; i++) a[i] = n - i;
}
void fill_half(int* a, int n)
{
    int h = n / 2;
    for (int i = 0; i < h; i++) a[i] = i;
    for (int i = h; i < n; i++) a[i] = n - i;
}

typedef void (*fill_fn)(int*, int);

void test(const char* name, fill_fn fill, int N)
{
    int* base = (int*)malloc(N * sizeof(int));
    int* arr = (int*)malloc(N * sizeof(int));
    fill(base, N);

    clock_t s, e;
    double t;

    memcpy(arr, base, N * sizeof(int));
    s = clock(); shell(arr, N); e = clock();
    t = (double)(e - s) / CLOCKS_PER_SEC;
    printf("  %-14s Шелл:   %10.6f с\n", name, t);

    memcpy(arr, base, N * sizeof(int));
    s = clock(); qs(arr, 0, N - 1); e = clock();
    t = (double)(e - s) / CLOCKS_PER_SEC;
    printf("  %-14s qs:     %10.6f с\n", name, t);

    memcpy(arr, base, N * sizeof(int));
    s = clock(); qsort(arr, N, sizeof(int), cmp); e = clock();
    t = (double)(e - s) / CLOCKS_PER_SEC;
    printf("  %-14s qsort:  %10.6f с\n\n", name, t);

    free(base); free(arr);
}

int main(void)
{
    setlocale(LC_ALL, "ru");
    int N = 1000;
    printf("=== N = %d ===\n\n", N);

    test("Случайный", fill_random, N);
    test("Возрастание", fill_ascending, N);
    test("Убывание", fill_descending, N);
    test("Половина", fill_half, N);

    return 0;
}

