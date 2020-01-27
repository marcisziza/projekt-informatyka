#include <ncurses.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>       
#include <sstream>

bool koniec_gry = false;

int wysokosc_planszy = 20;
int szerokosc_planszy = 40;

int pozycja_x;
int pozycja_y;

int poprzednia_pozycja_x;
int poprzednia_pozycja_y;

int ogon_x[100];
int ogon_y[100];
int dlugosc_ogona;

int owoc_x;   
int owoc_y;

int zdobyte_punkty = 0;

int szybkosc_weza = 0;

char kierunek;

int znak;

void EkranStartowy() {
  printw("            * * * * * * * * * * * * * * * * * * * * * * *\n");
  printw("            *            Witaj! Oto gra w Snejka.       *\n");
  printw("            * Wcisnij 's', aby rozpoczac gre.           *\n");
  printw("            * Wcisnij 'i', aby zobaczyc instrukcje gry. *\n");
  printw("            * Wcisnij 'q', aby wyjsc z programu.        *\n");
  printw("            * * * * * * * * * * * * * * * * * * * * * * *\n\n");
}

void Instrukcja() {
  printw("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
  printw("*                         Sterowanie wezem:                         *\n");
  printw("* W - gora, D - prawo, S - dol, A - lewo                            *\n");
  printw("*                                                                   *\n");
  printw("* Celem gry jest zebranie jak najwiekszej liczby punktow            *\n");
  printw("* Punkty sa dodawane, za zjedzenie owockow: symbol *                *\n");
  printw("*                                                                   *\n");
  printw("* Gra sie konczy, gdy waz wjedzie na scianke lub polknie swoj ogon  *\n");
  printw("*                                                                   *\n");
  printw("* Nacisnij 's', aby rozpoczac gre.                                  *\n");
  printw("* Nacisnij 'q', aby wyjsc z programu.                               *\n");
  printw("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
}

void WylosujOwoca() {
  owoc_x = (rand() % (szerokosc_planszy - 1) + 1);
  owoc_y = (rand() % (wysokosc_planszy - 1) + 1);
}

void UstawieniaGry() {
  pozycja_x = (szerokosc_planszy / 2);
  pozycja_y = (wysokosc_planszy / 2);
 
  WylosujOwoca();

  int punkty = 0;
}

void TrudnoscGry() {

  clear();

  printw("            * * * * * * * * * * * * * * * * * * * * * * *\n");
  printw("            *          Wybierz poziom trudnosci:        *\n");
  printw("            *      5 - latwy                            *\n");
  printw("            *      3 - sredni                           *\n");
  printw("            *      1 - trudny                           *\n");
  printw("            * * * * * * * * * * * * * * * * * * * * * * *\n\n");
  
  szybkosc_weza = getch();
  
  clear();

}

void WyrysowaniePlanszy() {
  
  clear();

  printw( "\n" );

  for (int i = 0; i < wysokosc_planszy + 1; i++) {
    for (int j = 0; j < szerokosc_planszy + 1; j++) {
      if(j == 0 || j == szerokosc_planszy  || i == 0 || i == wysokosc_planszy  ) {
        mvprintw(i, j, "+");
      }
      else {
        mvprintw(i, j, " ");
      }
    }  
    printw( "\n" );
  }
  printw( "\n" );
}

void GrajwSnejka() {

  TrudnoscGry();

  do {
    halfdelay(szybkosc_weza - 48);

    znak = getch();
    clear();

    //klawisz w = symbol n(orth)
    //klawisz s = symbol s(outh)
    //klawisz d = symbol e(ast)
    //klawisz a = symbol w(est)

    if(znak == 'w') {
      kierunek = 'n';
    }
    else if(znak == 'a') {
      kierunek = 'e';
    }
    else if(znak == 's') {
      kierunek = 's';
    }
    else if(znak == 'd') {
      kierunek = 'w';
    }
    else if(znak == 'q') {
      koniec_gry = true;
    }

    poprzednia_pozycja_x = pozycja_x;
    poprzednia_pozycja_y = pozycja_y;

    switch(kierunek) {
      case 'n':
        pozycja_y--;
        break;
            
      case 'e':
        pozycja_x--;
        break;

      case 's':
        pozycja_y++;  
        break;

      case 'w':
        pozycja_x++;
        break;
    }

    if(pozycja_x == owoc_x && pozycja_y == owoc_y) {
      WylosujOwoca();
      zdobyte_punkty += 10;
      dlugosc_ogona++;
    }

    //logika zwiększania się weza po zjedzeniu owoca
    if(dlugosc_ogona > 0) {
      for (int i = dlugosc_ogona - 1; i > 0; i--) {
        ogon_x[i] = ogon_x[i - 1];
        ogon_y[i] = ogon_y[i - 1];
      }
          
      ogon_x[0] = poprzednia_pozycja_x;
      ogon_y[0] = poprzednia_pozycja_y;
    }

    WyrysowaniePlanszy();

    //rysowanie owoca na planszy
    mvprintw(owoc_y, owoc_x, "*");

    //rysowanie glowy weza na planszy
    mvprintw(pozycja_y, pozycja_x, "0");

    //rysowanie ogona weza na planszy
    if(dlugosc_ogona >= 1) {
      for (int i = 0; i < dlugosc_ogona; i++) {
        mvprintw(ogon_y[i], ogon_x[i], "o");
      }
    }

    //wyswietlanie ilosci punktow
    std::stringstream wynik;
    wynik << "Punkty: " << zdobyte_punkty;

    mvprintw(wysokosc_planszy + 2, 0, wynik.str().c_str());

    //warunki konczace gre
    if(pozycja_x <= 0 || pozycja_y <= 0 || pozycja_x >= szerokosc_planszy || pozycja_y >= wysokosc_planszy ) {
      koniec_gry = true;
    }

    for (int i = 0; i < dlugosc_ogona; i++) {
      if(pozycja_x == ogon_x[i] && pozycja_y == ogon_y[i]) {
        koniec_gry = true;
      }
    }

  } while(!koniec_gry);
}

int main() {
  srand(time(NULL));

  initscr();
 
  EkranStartowy();

  noecho();
  
  znak = getch();
  switch(znak) {
    case 's':
      UstawieniaGry();
      GrajwSnejka();
    break;

    case 'i':
      Instrukcja();

      znak = getch();
      switch(znak) {
        case 's':

          clear();

          UstawieniaGry();
          GrajwSnejka();
        break;

        case 'q':
          koniec_gry = true;
        break;      
      }
    break;

    case 'q':
      koniec_gry = true;
    break;
  }
     
  printw("Koniec programu, przycisnij przycisk...");
  getch();
  endwin();   
}

