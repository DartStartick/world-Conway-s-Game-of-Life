#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#include "world.h"

#define MOORE_SEARCH_DEPTH 1

#define WORLD_WIDTH 230
#define WORLD_HEIGHT 50

#define LOOP_ADVANCE_DELAY 100

bool* newDeadCells(size_t width, size_t height) {
    // Создание нового массива клеток, изначально все мертвы (Vytvorenie nového poľa buniek, ktoré sú pôvodne všetky mŕtve)
    bool* cells = malloc(sizeof(bool) * width * height);

    for (size_t i = 0; i < (width * height); i++) {
        cells[i] = false;
    }

    return cells;
}

World* newWorld(size_t width, size_t height) {
    // Создание нового мира заданных размеров (Vytvorenie nového sveta definovaných rozmerov)
    World* w = malloc(sizeof(World));

    w->width = width;
    w->height = height;

    // Создание массивов для чтения и записи клеток (Vytvorenie polí na čítanie a zápis buniek)
    w->readCells = newDeadCells(width, height);
    w->writeCells = newDeadCells(width, height);

    return w;
}

void randomizeCells(World* world, float aliveChance) {
    // Случайная инициализация клеток с заданной вероятностью жизни (Náhodná inicializácia buniek s danou pravdepodobnosťou života)
    size_t width = world->width;
    size_t height = world->height;

    for (size_t i = 0; i < (width * height); i++) {
        world->readCells[i] = (rand() % 100) < aliveChance * 100;
    }
}

void copyWritableToReadable(World* world) {
    // Копирование клеток из массива записи в массив чтения (Kopírovanie buniek z poľa na zápis do poľa na čítanie)
    memcpy(world->readCells, world->writeCells,
           sizeof(bool) * world->width * world->height);
}

void freeWorld(World* world) {
    // Освобождение памяти, занятой миром и его клетками (Uvoľnenie pamäte obsadenej svetom a jeho bunkami)
    free(world->readCells);
    free(world->writeCells);
    free(world);
}

size_t indexOf(size_t width, size_t x, size_t y) {
    // Получение индекса клетки в одномерном массиве по координатам (Získanie indexu bunky v jednorozmernom poli pomocou súradníc)
    return width * y + x;
}

void setCell(World* world, size_t x, size_t y, bool isAlive) {
    // Установка состояния клетки по заданным координатам (Nastavenie stavu bunky podľa zadaných súradníc)
    size_t index = indexOf(world->width, x, y);

    world->writeCells[index] = isAlive;
}

bool getCell(const World* world, size_t x, size_t y) {
    // Получение состояния клетки по заданным координатам (Získanie stavu bunky zo zadaných súradníc)
    int index = indexOf(world->width, x, y);

    return world->readCells[index];
}

size_t min(size_t a, size_t b) {
    // Возвращает минимальное значение из двух (Vracia minimálnu hodnotu z dvoch)
    return (a < b) ? a : b;
}

size_t max(size_t a, size_t b) {
    // Возвращает максимальное значение из двух (Vracia maximálnu hodnotu dvoch)
    return (a > b) ? a : b;
}

size_t nAliveNeighborsSurrounding(World* world, size_t x, size_t y, size_t depth) {
    // Подсчет количества живых соседей в заданной области вокруг клетки (Počítanie počtu žijúcich susedov v danej oblasti okolo bunky)
    size_t xBound = min(x + depth + 1, world->width);
    size_t yBound = min(y + depth + 1, world->height);

    size_t aliveCount = 0;
    for (size_t ny = max(0, y - depth); ny < yBound; ny++) {
        for (size_t nx = max(0, x - depth); nx < xBound; nx++) {

            if (getCell(world, nx, ny) && !(nx == x && ny == y)) {
                aliveCount++;
            }
        }
    }

    return aliveCount;
}

bool cellShouldLive(bool isAlive, size_t nNeighbors) {
    // Определяет, должна ли клетка остаться живой или стать живой в следующем поколении (Určuje, či bunka zostane živá alebo sa stane živou v ďalšej generácii.)
    return (isAlive && nNeighbors >= 2 && nNeighbors <= 3)
        || (!isAlive && nNeighbors == 3);
}

void advanceCellAt(World* world, size_t x, size_t y) {
    // Продвигает клетку на один шаг во времени (posunie bunku o jeden krok v čase)
    size_t nNeighbors = nAliveNeighborsSurrounding(world, x, y, MOORE_SEARCH_DEPTH);
    bool isAlive = getCell(world, x, y);

    setCell(world, x, y, cellShouldLive(isAlive, nNeighbors));
}

void advanceWorld(World* world) {
    // Продвигает весь мир на один шаг во времени (Posúvanie celého sveta o krok ďalej v čase)
    size_t width = world->width;
    size_t height = world->height;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            advanceCellAt(world, x, y);
        }
    }

    copyWritableToReadable(world);
}

void formatWorld(World* world, WINDOW* win) {
    // Форматирование и вывод состояния мира в окно curses (Formátovanie a zobrazenie stavu sveta v okne curses)
    size_t width = world->width;
    size_t height = world->height;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            bool isAlive = getCell(world, x, y);
            char rep = isAlive ? '#' : ' ';
            mvwaddch(win, y, x, rep);
        }
    }
}

void printWorld(World* world, WINDOW* win) {
    // Вывод состояния мира в окно curses (Zobrazenie stavu sveta v okne curses)
    formatWorld(world, win);
    wrefresh(win);
}

void simpleConsoleRoutine() {
    // Простая реализация игрового цикла для консольного вывода (Jednoduchá implementácia hernej slučky pre výstup na konzolu)
    srand((unsigned int)time(NULL));

    World* world = newWorld(WORLD_WIDTH, WORLD_HEIGHT);
    randomizeCells(world, 0.1);

    for (size_t i = 0;; i++) {
        printWorld(world, stdscr);
        napms(LOOP_ADVANCE_DELAY);
    }
} 
