#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include "world.h"
#include <time.h>
#include <unistd.h>

// Размеры игрового поля (Rozmery hracej plochy)
#define GAME_WIDTH 230
#define GAME_HEIGHT 50

bool paused = false; // Флаг паузы (Vlajka pauzy)
int iteration = 0; // Счетчик итераций (Počítadlo iterácií)
double random_count = 0.12; // Счетчик псевдослучайных клеток (Pseudonáhodné počítadlo buniek)

// Функция обработки пользовательского ввода (Funkcia spracovania používateľských vstupov)
void setCell(World* world, size_t x, size_t y, bool value);
bool getCell(const World* world, size_t x, size_t y);

// Функция обработки событий (Funkcia spracovania udalostí)
void handleInput(World* world) {
    int ch = getch();
    if (ch == '\n') {
        iteration = 0;
        randomizeCells(world, random_count);
        clear();
        refresh();
    } else if (ch == ' ') {
        paused = !paused; // Инвертируем флаг паузы (Invertovanie príznaku pauzy)
    } else if (ch == '=' && random_count < 0.35) {
        random_count += 0.01; // Увеличиваем счетчик псевдослучайных клеток (Zvýšenie počítadla pseudonáhodných buniek)
    } else if (ch == '-' && random_count > 0.05) {
        random_count -= 0.01; // Уменьшаем счетчик псевдослучайных клеток (Zníženie počítadla pseudonáhodných buniek)
    }
}

// Основная игровая функция (Hlavná funkcia hry)
void playGame() {
    srand(time(NULL));

    World* world = newWorld(WORLD_WIDTH, WORLD_HEIGHT);
    randomizeCells(world, random_count);

    initscr(); // Инициализация curses (Inicializácia kliatby)
    noecho();  // Отключение эхо ввода (Deaktivácia vstupu echo)
    cbreak();  // Чтение символов без буферизации (Čítanie znakov bez vyrovnávacej pamäte)
    nodelay(stdscr, TRUE); // Неблокирующее чтение ввода (Neblokované čítanie vstupov)
    curs_set(0); // Скрыть курсор (Skryť kurzor)

    while (true) {
        printWorld(world, stdscr);

        // Весь присутствующий текст (Všetok prítomný text)
        mvprintw(GAME_HEIGHT + 1, 0, "Iterations: %d", iteration);
        mvprintw(0, 100, "Conway's Game of Life");
        mvprintw(GAME_HEIGHT + 1, 200, "Cell density: %'g", random_count);

        refresh();
        handleInput(world);
        
        if (!paused) {
            advanceWorld(world);
            iteration++;
        }
        
        usleep(100000); // Задержка в 100 миллисекунд (0.1 секунды) (Oneskorenie 100 milisekúnd (0,1 sekundy))
    }

    endwin(); // Завершение работы с curses (Dokončenie kliatby)

    freeWorld(world);
}

