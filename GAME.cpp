#include <iostream>
#include <string>
#include <sstream>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <termcap.h>

using namespace std;

string lines[13];
int stars[4];
bool game_over = false;

struct termios goldtio;

void no_buffer() {
  //struct termios goldtio, new_tio;
  unsigned char c;

  /* get the terminal settings for stdin */
  tcgetattr(STDIN_FILENO, & goldtio);

  /* we want to keep the old setting to restore them a the end */
  new_tio = goldtio;

  /* disable canonical mode (buffered i/o) and local echo */
  new_tio.c_lflag &= (~ICANON & ~ECHO);

  /* set the new settings immediately */
  tcsetattr(STDIN_FILENO, TCSANOW, & new_tio);
}

void restore() {
  // restore the former settings
  tcsetattr(STDIN_FILENO, TCSANOW, & goldtio);
}

// set string to default value

void set(int x, int y) {
  lines[0] = "...........................|-------------------------...............................................................................";
  lines[1] = ".|.---.--.......|-----.----|........................|...............................................................................";
  lines[2] = ".|.|||.||------.|..........|----------------------|.|...............................................................................";
  lines[3] = ".|.|||.||.......|.|------|.|......................|.|...............................................................................";
  lines[4] = ".|.|||.||.-----.|.|......|.|.---------------------|.|...............................................................................";
  lines[5] = ".|.|||.||.....|.|.|..|.|.|.|......................|.|...............................................................................";
  lines[6] = ".|.|||.||----.|.|.|..|.|.|.|.---------------------|.|......................................................................&&.......";
  lines[7] = ".|.|||........|.|.|..|.|...|......................|.|......................................................................&&.......";
  lines[8] = ".|.|||---------.|.|..|.-------------------------..|.|...............................................................................";
  lines[9] = "................|.|..|............................|.|...............................................................................";
  lines[10] = ".--------------.|.|..----------------------------.|.|...............................................................................";
  lines[11] = "..................|--------------------------------.--------------------------------................................................";
  lines[12] = "-----------------.....................................------------------------------................................................";

  if (stars[0] != 1) lines[6][3] = '*';
  if (stars[1] != 1) lines[7][17] = '*';
  if (stars[2] != 1) lines[12][0] = '*';
  if (stars[3] != 1) lines[1][28] = '*';

  if (stars[0] == 1 and stars[1] == 1 and stars[2] == 1 and stars[3] == 1) lines[6][52] = ')';
}

//prints lines
void print_on_screen() {
  int value = 0;
  while (value < 13) {
    cout << lines[value] << endl;
    value += 1;
  }
}

void chek_stars(int x, int y) {
  if (x == 3 and y == 6) stars[0] = 1;
  if (x == 17 and y == 7) stars[1] = 1;
  if (x == 0 and y == 12) stars[2] = 1;
  if (x == 28 and y == 1) stars[3] = 1;
}

void monster(int x, int y, int & time, int & my) {

  if (time <= 0) {
    lines[my][100] = '.';
    if (y - my < 0) my -= 1;
    else if (y - my > 0) my += 1;
    time = 2;
  } else time -= 1;
  lines[my][100] = '%';
}

void shoot(int my, int & sx, int & sy, int x, int y, int time) {
  if (x > 54 and my == y and time > 0 and sx < 56) {
    sx = 99;
    sy = my;
  }

  if (sx > 1) {
    sx -= 1;
    lines[sy][sx] = '=';
  }
  if (sx < 56) {
    sx = 0;
    sy = 0;
  }
}

//every frame
void update(int x, int y, char av, int & time, int & my, int & sx, int & sy) {

  set(x, y);
  lines[y][x] = av;
  chek_stars(x, y);
  monster(x, y, time, my);
  shoot(my, sx, sy, x, y, time);
  print_on_screen();

  if (lines[y][x] == '%'
    or lines[y][x] == '=') {
    game_over = true;
    lines[6][46] = 'G';
    lines[6][47] = 'a';
    lines[6][48] = 'm';
    lines[6][49] = 'e';
    lines[6][50] = ' ';
    lines[6][51] = 'O';
    lines[6][52] = 'v';
    lines[6][53] = 'e';
    lines[6][54] = 'r';
    lines[6][55] = '!';
  }
  if (lines[y][x] == '&') {
    game_over = true;
    lines[6][46] = 'Y';
    lines[6][47] = 'o';
    lines[6][48] = 'u';
    lines[6][49] = ' ';
    lines[6][50] = 'W';
    lines[6][51] = 'i';
    lines[6][52] = 'n';
    lines[6][53] = '!';
  }
}

//input
void input_(int & x, int & y) {

  char input = getchar();

  if (input == 'w'
    and y > 0 and lines[y - 1][x] != '|'
    and lines[y - 1][x] != '-') y -= 1;

  else if (input == 's'
    and y < 12 and lines[y + 1][x] != '|'
    and lines[y + 1][x] != '-') y += 1;

  else if (input == 'a'
    and x > 0 and lines[y][x - 1] != '|'
    and lines[y][x - 1] != '-') x -= 1;

  else if (input == 'd'
    and x < 131 and lines[y][x + 1] != '|'
    and x < 131 and lines[y][x + 1] != '-') x += 1;
}

int main() {
  no_buffer();
  int x = 0;
  int y = 0;
  char av = '*';
  int my = 6;
  int time = 2;
  int sy;
  int sx;

  while (game_over != true) {
    input_(x, y);
    update(x, y, av, time, my, sx, sy);
  }
}
