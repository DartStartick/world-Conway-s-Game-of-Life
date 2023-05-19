**Conway's Game of Life**

1. Hra "Conway's Game of Life", bunkový automat, ktorý v roku 1970 vynašiel anglický matematik John Conway. Je to hra bez hráčov, v ktorej človek vytvorí počiatočný stav a potom len pozoruje jeho vývoj.

2. Musíte skompilovať hru (gcc -o game game.c world.c main.c -lncurses), Potom spustite hru príkazom ./game, po ktorom sa hra okamžite spustí a bude fungovať.

3. Ovládanie hry je veľmi jednoduché Stlačením klávesu "Enter" sa svet znovu vygeneruje, klávesy "-" a "=" sa používajú na zmenu hustoty vygenerovaných buniek na mape, pričom samotnú hustotu je možné meniť vpravo dole od 0,04 do 0,35. Kláves "SPACE" je zodpovedný za zastavenie času, napr. ak chcete zobraziť konkrétny tvar. Opätovným stlačením tlačidla "SPACE" sa spustí beh času. Vľavo dole sa zobrazuje počet iterácií, aby ste pochopili, koľko času uplynulo od začiatku tejto hry Život.

4. Jednou z najdôležitejších zmien v hre je súbor World.c. Bol drasticky zmenený, aby hra fungovala správne, ale tieto zmeny budú zahrnuté v bode 7. Z dôležitých funkcií v kóde môžem vyzdvihnúť "setCell(World* world, size_t x, size_t y, bool value)" Táto funkcia nastavuje stav bunky na hracej ploche. Preberá ukazovateľ na štruktúru World, súradnice bunky (x, y) a logickú hodnotu, ktorá určuje, či je bunka živá (true) alebo mŕtva (false). 
"(World)" Hracie pole reprezentované dvojrozmerným poľom buniek. Každá bunka môže byť živá (true) alebo mŕtva (false).
"Клетка" Základná stavebná jednotka hry. Bunka môže byť v dvoch stavoch: živá alebo mŕtva. Stav každej bunky sa aktualizuje v každom kroku hry v závislosti od počtu živých susedov v jej okolí.
"getCell(const World* world, size_t x, size_t y)" Táto funkcia vracia stav bunky na hracej ploche. Preberá ukazovateľ na štruktúru World a súradnice bunky (x, y). Vracia logickú hodnotu, kde true označuje živú bunku a false mŕtvu bunku.

5. https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life https://rustwasm.github.io/docs/book/game-of-life/rules.html https://conwaylife.com/wiki/Conway%27s_Game_of_Life

6. Jediná vec, ktorá bola použitá zo ZAP, boli zručnosti na písanie takýchto funkcií, samotnú hru zo ZAP som nijako nepoužil.

7. Súbor world.c bol zásadne zmenený a tu sú hlavné zmeny:
"newWorld(size_t width, size_t height)" Vytvorí nový svet danej veľkosti. Pridelí pamäť pre polia buniek na čítanie a zápis a vráti ukazovateľ na štruktúru World.
"randomizeCells(World* world, float aliveChance)" Náhodne inicializuje stav buniek na hracej ploche s danou pravdepodobnosťou života.
"setCell(World* world, size_t x, size_t y, bool isAlive)" Nastaví stav konkrétneho políčka na hracej ploche podľa zadaných súradníc.
"getCell(const World* world, size_t x, size_t y)" Získa stav konkrétnej bunky na hracej ploche na zadaných súradniciach.
"advanceWorld(World* world)" Posúva svet o krok vpred. Pre každú bunku sa volá funkcia advanceCellAt, ktorá určuje, či má bunka zostať živá, alebo sa má stať živou v nasledujúcej generácii.
"printWorld(World* world, WINDOW* win)" Vypíše stav sveta do okna curses.
"simpleConsoleRoutine()" Jednoduchá implementácia hernej slučky pre výstup na konzolu. Používa sa na demonštráciu hry v textovej podobe.
