#pragma once // Она используется для обозначения того, что заголовочный файл должен быть включен только один раз в программу, независимо от того, сколько раз он был включен.

#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>

#define LOOP_ADVANCE_DELAY 100

#define WORLD_WIDTH 230
#define WORLD_HEIGHT 50

typedef struct {
    
    bool* readCells;    // Массив чтения клеток (Súbor nameraných hodnôt buniek)
    bool* writeCells;   // Массив записи клеток (Záznamové pole buniek)

    size_t width;       // Ширина мира (Šírka sveta)
    size_t height;      // Высота мира (Výška sveta)

} World;

// Создание нового мира заданных размеров (Vytvorenie nového sveta definovaných rozmerov)
World* newWorld(size_t width, size_t height);

// Освобождение памяти, занятой миром и его клетками (Uvoľnenie pamäte obsadenej svetom a jeho bunkami)
void freeWorld(World* world);

// Случайная инициализация клеток с заданной вероятностью жизни (Náhodná inicializácia buniek s danou pravdepodobnosťou života)
void randomizeCells(World* world, float aliveChance);

// Вывод состояния мира в окно curses (Zobrazenie stavu sveta v okne curses)
void printWorld(World* world, WINDOW* win);

// Продвижение всего мира на один шаг во времени (Celý svet o krok ďalej v čase)
void advanceWorld(World* world);

// Получение состояния клетки по заданным координатам (Získanie stavu bunky zo zadaných súradníc)
bool getCell(const World* world, size_t x, size_t y);
