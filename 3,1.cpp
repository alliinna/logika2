#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct node
{
    char inf[256];
    struct node* next;  //ссылка на следующий 
};

struct node* head = NULL; //отсюда забираем
struct node* last = NULL; //сюда добавляем

//сброс остатка строки из буфера ввода
static void drop_tail(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//создание нового узла
struct node* get_struct(void)
{
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL)
    {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }

    printf("Введите название объекта: ");
    if (scanf_s(" %255[^\n]", p->inf, (unsigned)sizeof(p->inf)) != 1)
    {
        p->inf[0] = '\0';
    }
    drop_tail();

    p->next = NULL;
    return p;
}

//постановка объекта в очередь с сохранением алфавитного порядка
void spstore(void)
{
    struct node* p = get_struct();

    if (p->inf[0] == '\0')
    {
        printf("Запись не была произведена (пустое имя)\n");
        free(p);
        return;
    }

    // Если очередь пуста — просто добавляем
    if (head == NULL)
    {
        head = p;
        last = p;
        return;
    }

    struct node* cur = head;
    struct node* prev = NULL;

    while (cur != NULL && strcmp(cur->inf, p->inf) <= 0)
    {
        prev = cur;
        cur = cur->next;
    }

    if (prev == NULL)
    {
        //вставляем в начало
        p->next = head;
        head = p;
    }
    else
    {
        p->next = prev->next;
        prev->next = p;

        if (p->next == NULL) last = p;
    }
}

//извлечение объекта из начала
void serve(void)
{
    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }

    struct node* temp = head;
    head = head->next; // сдвиг в начал

    if (head == NULL) last = NULL; //очередь пустая

    printf("Извлечен объект: %s\n", temp->inf);
    free(temp);
}

//просмотр
void review(void)
{
    struct node* struc = head;

    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }

    while (struc != NULL)
    {
        printf("Объект: %s\n", struc->inf);
        struc = struc->next;
    }
}

//очистка памяти
void clear_list(void)
{
    struct node* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
    last = NULL;
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice;
    do {
        printf("\n--- ОЧЕРЕДЬ, ОТСОРТИРОВАННАЯ ПО ИМЕНАМ (A-Z) ---\n");
        printf("1. Добавить объект (с сортировкой)\n");
        printf("2. Извлечь объект (из начала)\n");
        printf("3. Просмотр очереди\n");
        printf("0. Выход\n");
        printf("Выбор: ");

        if (scanf_s("%d", &choice) != 1)
        {
            drop_tail();
            printf("Неверный ввод! Введите число.\n");
            choice = -1;
            continue;
        }
        drop_tail();

        switch (choice) {
        case 1: spstore(); break;
        case 2: serve();   break;
        case 3: review();  break;
        case 0: clear_list(); break;
        default: printf("Неверный ввод!\n");
        }
    } while (choice != 0);

    return 0;
}
