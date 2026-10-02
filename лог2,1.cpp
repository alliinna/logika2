#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

void shell(int* items, int count) {

    int i, j, gap, k;
    int x, a[5] = { 9, 5, 3, 2, 1 };
    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (x < items[j]) && (j >= 0); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right) {
    int i = left, j = right;
    int x = items[(left + right) / 2];
    do {
        while ((items[i] < x) && (i < right)) i++;
        while ((x < items[j]) && (j > left)) j--;
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

int main(void) {
    int sizes[] = { 1000, 5000, 10000, 50000, 100000 };
    int n_sizes = 5;

    setlocale(LC_ALL, "ru");
    printf("Размер\tШелл (сек)\tБыстрая (сек)\n");
    printf("------\t-----------\t-------------\n");

    for (int s = 0; s < n_sizes; s++) {
        int N = sizes[s];
        int* arr1 = (int*)malloc(N * sizeof(int));
        int* arr2 = (int*)malloc(N * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            arr1[i] = rand() % 100000;
            arr2[i] = arr1[i];
        }

        clock_t start = clock();
        shell(arr1, N);
        clock_t end = clock();
        double t_shell = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock();
        qs(arr2, 0, N - 1);
        end = clock();
        double t_qs = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("%d\t%.6f\t%.6f\n", N, t_shell, t_qs);

        free(arr1);
        free(arr2);
    }

    return 0;
}