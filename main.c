#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <locale.h>

#define BIG 999999999

// Функция для вывода матрицы смежности
void printMatrix(int a, int** graph) {
    printf("    ");
    // Вывод заголовка с номерами столбцов
    for (int i = 0; i < a; i++) {
        printf("%-4d", i + 1);
    }
    printf("\n");

    // Вывод матрицы смежности
    for (int i = 0; i < a; i++) {
        printf("%-4d", i + 1);
        for (int j = 0; j < a; j++) {
            if (graph[i][j] > BIG) {
                printf("big");
            }
            else {
                printf("%-4d", graph[i][j]);
            }
        }
        printf("\n");
    }
}

// Функция для генерации случайного графа
void generateRandomGraph(int a, int** graph) {
    srand(time(NULL));

    // Заполнение матрицы случайными весами, 0 по диагонали
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {
            if (i == j) {
                graph[i][j] = 0;
            }
            else {
                graph[i][j] = rand() % 9 + 1;
            }
        }
    }
}

// Функция для ручного ввода матрицы смежности
void manualInputGraph(int a, int** graph) {
    printf("Введите матрицу смежности(%dx%d):\n", a, a);

    // Ввод элементов матрицы с проверкой на корректность
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {
            while (1) {
                printf("[%d][%d]: ", i, j);
                if (scanf("%d", &graph[i][j]) == 1 && graph[i][j] <= BIG) {
                    break;
                }
                else {
                    printf("Ошибка ввода. Пожалуйста, введите корректные данные (не больше %d).\n", BIG);
                    while (getchar() != '\n');
                }
            }
        }
    }
}


// Функция для загрузки матрицы смежности из файла
void fileInputGraph(int* a, int*** graph) {
    FILE* file;
    char filename[256];

    do {
        printf("Введите имя файла: ");
        if (scanf("%s", filename) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
            while (getchar() != '\n');
            filename[0] = '\0';
        }
    } while (filename[0] == '\0');

    file = fopen(filename, "r");
    while (file == NULL) {
        printf("Ошибка открытия файла. Пожалуйста, введите корректное имя файла: ");
        if (scanf("%s", filename) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
            while (getchar() != '\n');
            filename[0] = '\0';
        }
        file = fopen(filename, "r");
    }



    // Выделение памяти под матрицу
    *graph = (int**)malloc(*a * sizeof(int*));
    for (int i = 0; i < *a; i++) {
        (*graph)[i] = (int*)malloc(*a * sizeof(int));
        // Чтение элементов матрицы из файла
        for (int j = 0; j < *a; j++) {
            if (fscanf(file, "%d", &(*graph)[i][j]) != 1) {
                printf("Ошибка чтения элементов матрицы из файла.\n");
                exit(1);
            }

            // Пропустить разделители (пробелы, табуляции)
            int delimiter;
            do {
                delimiter = fgetc(file);
            } while (delimiter == ' ' || delimiter == '\t');

            // Вернуть разделитель в поток
            ungetc(delimiter, file);
        }
    }

    // Закрытие файла
    fclose(file);
}

// Функция для выполнения алгоритма Флойда
void floyd(int a, int** graph) {
    int** dist = (int**)malloc(a * sizeof(int*));
    for (int i = 0; i < a; i++) {
        dist[i] = (int*)malloc(a * sizeof(int));
        for (int j = 0; j < a; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    // Алгоритм Флойда
    for (int k = 0; k < a; k++) {
        for (int i = 0; i < a; i++) {
            for (int j = 0; j < a; j++) {
                if (dist[i][k] != 0 && dist[k][j] != 0 && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // Вывод матрицы расстояний после алгоритма Флойда
    printf("\nМатрица расстояний:\n");
    printf("    ");
    for (int i = 0; i < a; i++) {
        printf("%-4d", i + 1);
    }
    printf("\n");

    for (int i = 0; i < a; i++) {
        printf("%-4d", i + 1);
        for (int j = 0; j < a; j++) {
            printf("%-4d", dist[i][j]);
        }
        printf("\n");
    }

    // Освобождение памяти
    for (int i = 0; i < a; i++) {
        free(dist[i]);
    }
    free(dist);
}

// Функция для сохранения результатов в файл
void saveToFile(int a, int** graph) {
    FILE* file;
    char filename[256];

    printf("Введите имя файла для сохранения результатов: ");
    scanf("%s", filename);

    // Открытие файла для записи
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        exit(1);
    }

    // Сначала сохраняем матрицу смежности
    fprintf(file, "%d\n", a);
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < a; j++) {
            fprintf(file, "%d", graph[i][j]);

            // Вставляем пробелы между элементами матрицы
            if (j < a - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    // Затем сохраняем матрицу расстояний (просто копируем код вывода матрицы расстояний)
    int** dist = (int**)malloc(a * sizeof(int*));
    for (int i = 0; i < a; i++) {
        dist[i] = (int*)malloc(a * sizeof(int));
        for (int j = 0; j < a; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    fprintf(file, "\nМатрица расстояний:\n");
    fprintf(file, "    ");
    for (int i = 0; i < a; i++) {
        fprintf(file, "%-4d", i + 1);
    }
    fprintf(file, "\n");

    for (int i = 0; i < a; i++) {
        fprintf(file, "%-4d", i + 1);
        for (int j = 0; j < a; j++) {
            fprintf(file, "%-4d", dist[i][j]);
        }
        fprintf(file, "\n");
    }

    // Освобождение памяти
    for (int i = 0; i < a; i++) {
        free(dist[i]);
    }
    free(dist);

    // Закрытие файла
    fclose(file);
}

// Основная функция программы
int main() {
    int a;
    int** graph = NULL;
    setlocale(LC_ALL, "Rus");  // Установка русской локали
    printf("Курсовой проект по курсу «Логика и основы алгоритмизации в инженерных задачах» на тему «Реализация алгоритма Флойда»\n");
    printf("\n");
    printf("Выполнил студент группы 22ВВВ1:\n");
    printf("Коннов А.Д.\n");
    printf("\n");
    printf("Приняли:\n");
    printf("Акифьев И.В.\n");
    printf("Юрова О.В.\n");
    printf("\n");
    printf("Пенза - 2023\n");
    printf("\n");

    // Основной цикл программы
    while (1) {
        printf("Выберите действие:\n");
        printf("1. Ввести матрицу смежности\n");
        printf("2. Загрузить матрицу смежности из файла\n");
        printf("0. Завершить программу\n");

        int choice;
        int fillChoice;
        do {
            printf("Ваш выбор: ");
            if (scanf("%d", &choice) != 1) {
                printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
                while (getchar() != '\n');
                choice = -1;
            }
        } while (choice < 0);

        switch (choice) {
        case 1:
            do {
                printf("Введите размер матрицы: ");
                if (scanf("%d", &a) != 1 || a <= 0) {
                    printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
                    while (getchar() != '\n');
                    a = -1;
                }
            } while (a < 0);


            // Выделение памяти под матрицу
            graph = (int**)malloc(a * sizeof(int*));
            for (int i = 0; i < a; i++) {
                graph[i] = (int*)malloc(a * sizeof(int));
            }

            do {
                printf("Выберите способ заполнения матрицы:\n");
                printf("1. Заполнить случайными значениями\n");
                printf("2. Заполнить вручную\n");
                printf("Ваш выбор: ");
                if (scanf("%d", &fillChoice) != 1 || (fillChoice != 1 && fillChoice != 2)) {
                    printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
                    while (getchar() != '\n');
                    fillChoice = -1;
                }
            } while (fillChoice < 0);

            switch (fillChoice) {
            case 1:
                generateRandomGraph(a, graph);
                break;
            case 2:
                manualInputGraph(a, graph);
                break;
            default:
                printf("Ошибка: неверный выбор заполнения матрицы.\n");
                return 1;
            }

            // Вывод матрицы
            printf("Матрица смежности:\n");
            printMatrix(a, graph);

            // Вывод матрицы расстояний
            floyd(a, graph);

            // Сохранение результатов в файл
            char saveChoice;
            do {
                printf("Желаете сохранить результаты в файл? (1 - Да/2 - Нет): ");
                if (scanf(" %c", &saveChoice) != 1 || (saveChoice != '1' && saveChoice != '2')) {
                    printf("Ошибка ввода. Пожалуйста, введите корректные данные.\n");
                    while (getchar() != '\n');
                    saveChoice = '0';
                }
            } while (saveChoice == '0');

            if (saveChoice == '1') {
                saveToFile(a, graph);
                printf("Результаты сохранены в файл.\n");
            }


            // Освобождение памяти
            for (int i = 0; i < a; i++) {
                free(graph[i]);
            }
            free(graph);
            break;

        case 2:
            fileInputGraph(&a, &graph);

            // Вывод матрицы
            printf("Матрица смежности:\n");
            printMatrix(a, graph);

            // Вывод матрицы расстояний
            floyd(a, graph);

            // Сохранение результатов в файл
            char saveChoice2;
            printf("Желаете сохранить результаты в файл? (1 - Да/2 - Нет): ");
            scanf(" %c", &saveChoice2);
            if (saveChoice2 == '1') {
                saveToFile(a, graph);
                printf("Результаты сохранены в файл.\n");
            }

            // Освобождение памяти
            for (int i = 0; i < a; i++) {
                free(graph[i]);
            }
            free(graph);
            break;

        case 0:
            return 0;  // Завершение программы

        default:
            printf("Ошибка: неверный выбор.\n");
            return 1;
        }
    }
}
