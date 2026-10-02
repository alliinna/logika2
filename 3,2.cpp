#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct node
{
    char inf[256];      //назв объект
    int priority;       //приоритет
    struct node* next;  //ссылка на следующий
};

//указ на 1 и последний элемент
struct node* head = NULL;
struct node* last = NULL;

//очистка буфера от остатков строк
static void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//создание элемента списка
struct node* get_struct(void)
{
    struct node* p = NULL;
    char s[256];

    if ((p = (struct node*)malloc(sizeof(struct node))) == NULL)
    {
        printf("Ошибка при распределении памяти\n");
        exit(1);
    }

    printf("Введите название объекта: \n");
    if (scanf_s("%255s", s, (unsigned)sizeof(s)) != 1)
    {
        clear_input();
        printf("Запись не была произведена\n");
        free(p);
        return NULL;
    }
    clear_input();

    if (*s == 0)
    {
        printf("Запись не была произведена\n");
        free(p);
        return NULL;
    }
    strcpy_s(p->inf, sizeof(p->inf), s);

    printf("Введите приоритет: \n");
    if (scanf_s("%d", &p->priority) != 1)
    {
        clear_input();
        printf("Приоритет введён неверно, принят 0\n");
        p->priority = 0;
    }
    clear_input();

    p->next = NULL;
    return p;
}

//проверка, есть ли уже объект с таким названием
static int name_exists(const char* name)
{
    struct node* cur = head;
    while (cur != NULL)
    {
        if (strcmp(cur->inf, name) == 0)
            return 1;
        cur = cur->next;
    }
    return 0;
}

//добавление элемента в список в соответствии с приоритетом
//с большим перед с меньшим
void spstore(void)
{
    struct node* p = NULL;
    struct node* struc = head;
    struct node* prev = NULL;

    p = get_struct();
    if (p == NULL) return;

    while (name_exists(p->inf))
    {
        printf("Объект с названием \"%s\" уже есть в очереди!\n", p->inf);
        printf("Введите новый приоритет для этого объекта (или 0 для отмены): \n");

        if (scanf_s("%d", &p->priority) != 1)
        {
            clear_input();
            printf("Приоритет введён неверно. Добавление отменено.\n");
            free(p);
            return;
        }
        clear_input();

        if (p->priority == 0)
        {
            printf("Добавление отменено.\n");
            free(p);
            return;
        }

        break;
    }

    if (head == NULL)
    {
        head = p;
        last = p;
        return;
    }

    //ищем позицию пропускаем элементы с приоритетом >= приоритета нового
    while (struc != NULL && struc->priority >= p->priority)
    {
        prev = struc;
        struc = struc->next;
    }

    if (prev == NULL)
    {
        //вставляем в нач (приоритет нового >, чем у головы)
        p->next = head;
        head = p;
    }
    else
    {
        //вставляем после prev
        p->next = prev->next;
        prev->next = p;

        if (p->next == NULL) last = p;
    }
    return;
}

void serve(void)
{
    struct node* temp = head;

    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }

    head = head->next;
    if (head == NULL) last = NULL;

    printf("Извлечён объект: %s (Приоритет: %d)\n", temp->inf, temp->priority);
    free(temp);
    return;
}

//Просмотр содержимого
void review(void)
{
    struct node* struc = head;

    if (head == NULL)
    {
        printf("Очередь пуста\n");
        return;
    }

    while (struc)
    {
        printf("Имя - %s, Приоритет - %d\n", struc->inf, struc->priority);
        struc = struc->next;
    }
    return;
}

//Очистка всей памяти при выходе
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
        printf("\n--- ПРИОРИТЕТНАЯ ОЧЕРЕДЬ ---\n");
        printf("1. Добавить объект (по приоритету)\n");
        printf("2. Извлечь объект (первый)\n");
        printf("3. Просмотр очереди\n");
        printf("0. Выход\n");
        printf("Выбор: ");

        if (scanf_s("%d", &choice) != 1)
        {
            clear_input();
            printf("Неверный ввод! Введите число.\n");
            choice = -1;
            continue;
        }
        clear_input();

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

