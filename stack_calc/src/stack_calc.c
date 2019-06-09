/*
 ============================================================================
 Name        : stack_calc.c
 Author      : D.Nest
 Version     : 1.0
 Description : Калькулятор с использованием стека и получением задач из файла для использования обратной польской нотации.
 В Input.txt задачи заполняются по форме:
 Первое_Число Второе_Число Оператор & ...
 Пример:
 2 5 * & 5 3 + & 9 ! &
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
    char ch[10];
    struct list* next;
} list;

list* last = NULL;
list* head = NULL;
int bool = 1;
typedef struct stek {
    float data;
    char ch;
    struct stek* next_stek;
} stek;

stek* lastFstek = NULL;
stek* headFstek = NULL;

//Функция начала и конца чтения входящего файла

void InitFile(FILE* input)
{

    list* futureLast = (list*)calloc(1, sizeof(list)); // Выделяем память
    futureLast->next = NULL;
    fscanf(input, "%c", &futureLast->ch[0]);
    int size = 0;
    if (futureLast->ch[0] == '\0') // Если файл заканчивается
    {
        free(futureLast); //Освобождаем память
        bool = 0;
    }
    else {
        while (futureLast->ch[size] != ' ') {
            size++;
            fscanf(input, "%c", &futureLast->ch[size]);
        }

        if (last != NULL) {

            last->next = futureLast;
            futureLast->next = NULL;
        }
        // Инициализация стека при первом запуске
        else {
            head = futureLast;
        }
    }
    last = futureLast;
}
float number_calc(char operation, float value1, float value2)
{
    float result_elem; //Возвращаем указатель
    float res; //Локальная переменная для факториала
    if (operation == '+') {
        result_elem = value1 + value2;
    }
    if (operation == '-') {
        result_elem = value1 - value2;
    }
    if (operation == '*') {
        result_elem = value1 * value2;
    }
    if (operation == '/') {
        if (value2 == 0)
            result_elem = 0;
        else
            result_elem = value1 / value2;
    }
    if (operation == '^') {
        result_elem = value1;
        if (value2 != 0) {
            for (int i = 1; i < value2; i++) {
                value1 = value1 + result_elem;
                result_elem = value1;
            }
        }
        else {
            result_elem = 1;
        }
    }
    if (operation == '!') {
        res = 1;
        for (int i = 1; i <= value1; i++) {
            res = res * i;
        }
        result_elem = res;
    }
    return result_elem;
}
int main(void)
{
    FILE *input, *output; //Файлы на ввод и вывод
    input = fopen("input.txt", "r"); //Открываем файл на ввод
    while (bool) {
        InitFile(input);
    }
    //Когда файл прочтён
    fclose(input); //Закрываем его
    // Открываем файл для вывода
    output = fopen("output.txt", "w");
    while (head != NULL) // Перебор стека с элементами
    {
        if (head->ch[0] != '&') {
            if (head->ch[0] == '+' || head->ch[0] == '*' || head->ch[0] == '/' || head->ch[0] == '!' || (head->ch[0] == '-' && head->ch[1] == ' ')) {
                if (head->ch[0] != '!') {
                    headFstek->next_stek->data = number_calc(head->ch[0], headFstek->next_stek->data, headFstek->data);
                }
                else {
                    headFstek->next_stek->data = number_calc(head->ch[0], headFstek->data, '0');
                }
                headFstek->next_stek->ch = head->ch[0];
                //Очистка стека
                lastFstek = headFstek;
                headFstek = headFstek->next_stek;
                free(lastFstek);
            }
            else { // Если элемент - число
                stek* tmp = (stek*)malloc(sizeof(stek));
                tmp->data = atof(head->ch);
                tmp->next_stek = headFstek;
                headFstek = tmp;
            }
            // Переход на следующй элемент
        }
        else {
            // Запись результата в файл
            switch (headFstek->ch) {
            case '+':
                fprintf(output, "Результат сложения: %.2f\n", headFstek->data);
                break;
            case '-':
                fprintf(output, "Результат вычитания: %.2f\n", headFstek->data);
                break;
            case '*':
                fprintf(output, "Результат умножения: %.2f\n", headFstek->data);
                break;
            case '/':
                fprintf(output, "Результат деления: %.2f\n", headFstek->data);
                break;
            case '!':
                fprintf(output, "Факториал: %.2f\n", headFstek->data);
                break;
            case '^':
                fprintf(output, "Результат возведения в степень: %.2f\n", headFstek->data);
                break;
            }
            free(headFstek); // Удаление последнего элемента из стека
        }
        head = head->next;
    }
    fclose(output); //Закрытие файла на вывод
    return 0;
}
