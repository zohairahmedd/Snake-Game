#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>

// global variables
int headX, headY, fruitX, fruitY, width = 30, height = 30, gameOver = 0, direction = 2, speeds = 0.8i, size = 0, score = 0, speed = 100000;
int bodyX[30], bodyY[30];

// function prototypes
void render(void);
void setupGame(void);
void inputCheck(void);
void moveSnake(void);
void gameOverCheck(void);
int kbhit(void);
char getch(void);
void placeFruit(void);
void fruitCheck(void);
void usleep(int);

int main() {

    setupGame(); // sets up the first or initial position of snake
    placeFruit(); // places fruit at random 

    while (!gameOver) { // game loop, continues until gameOver equals 1
        render(); // renders the game
        inputCheck(); // checks for user input
        moveSnake(); // moves the snake
        fruitCheck(); // checks if the snake has eaten the fruit
        gameOverCheck(); // checks if the game is over
        usleep(speed); // pauses the game for 0.1 seconds (100000 microseconds)
    }

    usleep(speed); // pauses the game for 0.1 seconds (100000 microseconds)
    system("clear"); // clears the console
    printf("\n\n\n\n\t\t\tYour Final Score: %5d\n\n\t\t\t (press any key to exit)\n\n\n\n", score);
    while (!kbhit()); // waits for the user to press a key
    system("clear"); // clears the console again
}

void render(void) { // prints board including boundaries, fruit, snake, and score

    system("clear");
    int i, j, k, p;
    for (j = 0; j <= height; j++) {
        for (i = 0; i <= width; i++) {
            p = 1;
            if (i == 0 || j == 0 || j == height || i == width) {
                printf("*");
            }
            else if (i == fruitX && j == fruitY) {
                printf("0");
            }
            else if (i == headX && j == headY) {
                printf("o");
            }
            else {
                for (k = 0;k < size;k++) {
                    if (i == bodyX[k] && j == bodyY[k]) {
                        printf("+");
                        p = 0;
                    }
                }
                if (p) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %5d", score);
}

void setupGame(void) { // sets initial position of snake

    headX = height / 2;
    headY = width / 2;
}

void placeFruit(void) { // places fruit at a random position

resetfruitX:fruitX = rand() % 20;
    if (fruitX == 0 || fruitX == width) {
        goto resetfruitX;
    }
resetfruitY:fruitY = rand() % 20;
    if (fruitY == 0 || fruitY == height) {
        goto resetfruitY;
    }
}

void fruitCheck(void) { // checks if fruit has been "eaten", if so, increase score, snake size, decrease speed if speed > 50000, and place fruit at new random position

    if (headX == fruitX && headY == fruitY) {
        score += 10;
        size++;
        if (speed > 50000)
            speed -= 500;
        placeFruit();
    }
}

void inputCheck(void) { // checks for user input to control direction of snake

    if (kbhit()) {
        char key = getch();

        // direction 1 = upwards, direction 2 = right, direction 3 = downwards, direction 4 = left
        if (key == 'w') {
            if (direction != 3) {
                direction = 1;
            }
        }
        else if (key == 'd') {
            if (direction != 4) {
                direction = 2;
            }
        }
        else if (key == 's') {
            if (direction != 1) {
                direction = 3;
            }
        }
        else if (key == 'a') {
            if (direction != 2) {
                direction = 4;
            }
        }
    }
}

void moveSnake(void) { // moves snake in current direction it is travelling in

    int x1, x2, y1, y2, i;
    if (size == 1) {
        bodyX[0] = headX;
        bodyY[0] = headY;
    }
    else {
        x1 = headX;
        y1 = headY;
        for (i = 0;i < size;i++) {
            x2 = bodyX[i];
            y2 = bodyY[i];
            bodyX[i] = x1;
            bodyY[i] = y1;
            x1 = x2;
            y1 = y2;
        }
    }
    if (direction == 1) {
        headY--;
    }
    else if (direction == 2) {
        headX++;
    }
    else if (direction == 3) {
        headY++;
    }
    else if (direction == 4) {
        headX--;
    }
}

void gameOverCheck(void) { // checks if game is over, which happens if snake collides with itself or a boundary

    int i;
    for (i = 0;i < size;i++) {
        if (headX == bodyX[i] && headY == bodyY[i])
            gameOver = 1;
    }

    if (headX == width || headX == 0 || headY == height || headY == 0) {
        gameOver = 1;
    }
}

int kbhit(void) { // checks if a key has been pressed in console - responds to user input without blocking the execution of the program

    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char getch(void) {

    char c;
    system("stty raw"); // raw terminal does not interpret any special characters, and does not display characters that are typed
    c = getchar(); // reads character input
    system("stty sane"); // returns terminal back to original state after character has been read

    return(c);
}