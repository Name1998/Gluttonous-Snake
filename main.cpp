#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define MAPWIDTH  40
#define MAPHEIGHT 25
#define MAXLENGTH 100
#define MAXSPEED  200

struct {
    int x;
    int y;
}food;

struct Snake {
    int speed;
    int len;
    int x[MAXLENGTH];
    int y[MAXLENGTH];
};// snake;         // NOT SAFE IF REPLAY THE GAME;

char currentDirection;

void initialize();
void locate(int x, int y);
void printText(int x, int y, int color, const char text[]);
void gameOver(Snake snake);
void detectKeyboard();
void generateFood(Snake snake);
bool hitSth(Snake snake);
void snakeMove(Snake snake);

void initialize() {
    //initialize direction
    currentDirection = 'd';         
    system("cls");
    /*  DRAW  FRAME(WALL)  */
    for (int i = 0; i < MAPWIDTH; i += 2) {     //横向画WIDTH/2格？？
        printText(i, 0, 6, "□");
        printText(i, MAPHEIGHT, 6, "□");
    }
    for (int i = 0; i <= MAPHEIGHT; i++) {      //竖向画HEIGHT+1格？？
        printText(0, i, 6, "□");
        printText(MAPWIDTH, i, 6, "□");
    }
    /*  INITIALIZE  AND  DRAW  THE  SNAKE  */
    Snake snake;
    snake.len = 3;
    snake.speed = 15;
    snake.x[0] = MAPWIDTH / 2;
    snake.y[0] = MAPHEIGHT / 2;
    printText(snake.x[0], snake.y[0], 7, "■");      //draw head
    for (int i = 1; i < snake.len ; i++) {
        snake.x[i] = snake.x[i - 1] - 2;            // initialize body from head to left, related to default currentDirection
        snake.y[i] = snake.y[i - 1];
        printText(snake.x[i], snake.y[i], 7, "■");
    }
    printf("\n\n\n");
    /*  GENERATE 1ST FOOD  */
    generateFood(snake);
    snakeMove(snake);
}

void locate(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printText(int x, int y, int color, const char text[]) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    locate(x, y);
    printf(text);
}

void gameOver(Snake snake) {
    system("cls");
    printText(MAPWIDTH / 2 + 10, MAPHEIGHT / 2, 3, "");
    printf("you lose, the score is %d\n\n", snake.len - 3);

    //     CANNOT DO THIS YET, THE SNAKE.X[] AND SNAKE.Y[] YET NOT RESET *************
    printText(MAPWIDTH / 2 + 10, MAPHEIGHT / 2 + 1, 3, "Would you like to play again?[Y/N]: "); 
    rewind(stdin);
    char c;// = _getch();
    scanf_s("%c", &c);
    switch (c) {
    case('Y'):case('y'): initialize();  break;
    case('N'):case('n'): exit(0); break;
    }

}
void detectKeyboard() {
    char cl;
    //rewind(stdin);
    if (_kbhit() && (cl = _getch())) {
        switch (cl) {
        case 'a':case'A':
            if (currentDirection != 'd') currentDirection = 'a'; break;
        case 's':case'S':
            if (currentDirection != 'w') currentDirection = 's'; break;
        case 'd':case'D':
            if (currentDirection != 'a') currentDirection = 'd'; break;
        case 'w':case'W' :
            if (currentDirection != 's') currentDirection = 'w'; break;
        case ' ':       // when get space game pause
            _getch();   break;
        case 27:        // when get ESC   game exit
            exit(0);    break;
        }
    }
    
}

void generateFood(Snake snake) {
    //if (snake.x[0] == food.x && snake.y[0] == food.y) {     
        bool isOcupied;
        do{
            isOcupied = false;
            srand((unsigned int)time(NULL));
            food.x = rand() % (MAPWIDTH - 4) + 2;
            food.x -= ((food.x % 2) ? 1 : 0);           //ensure that food.x is not an odd(because □ takes width 2)
            food.y = rand() % (MAPHEIGHT - 2) + 1;

            for (int i = 0; i < snake.len; i++) {
                if (snake.x[i] == food.x && snake.y[i] == food.y) {
                    isOcupied = true;
                    break;
                } 
            }
        } while (isOcupied);

        printText(food.x, food.y, 12, "●");
        /*  LOCATE THE CURSOR TO RIGHT BOTTOM  */
        locate(MAPWIDTH, MAPHEIGHT);
        //score++;
        //snake.speed -= 5;
        //changeFlag = 1;       //if erase tail or not
    //}
}
bool hitSth(Snake snake) {
    if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH)
        return true;
    if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
        return true;
    for (int i = 1; i < snake.len +1 ; i++)
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
            return true;
    return false;
}
void snakeMove(Snake snake) {
    for (; 1; Sleep(300-snake.speed)) {
        detectKeyboard();

        printText(MAPWIDTH + 20, MAPHEIGHT/2, 2, "");
        printf("The Score Is: %d", snake.len - 3);
        printText(MAPWIDTH + 20, MAPHEIGHT / 2 + 1, 2, "");
        printf("The Speed Is: %d", snake.speed);

        //Sleep(100);       //  HEAD  AND  TAIL  MOVE  ASYNCHRONIZELY
        /*  RE-CALCULATE  BODY  POSITION  */
        for (int i = snake.len ; i > 0; i--) {                      //不能从snake.len-1 开始迭代，为什么？**** 需要使用len+1的空间来计算，多出一位是缓存
            snake.x[i] = snake.x[i - 1];
            snake.y[i] = snake.y[i - 1];
        }
        
        /*  RE-CALCULATE  HEAD  POSITION  AND  DRAW  */
        switch (currentDirection) {
            case 'a': snake.x[0] -= 2;  break;
            case 'd': snake.x[0] += 2;  break;
            case 'w': snake.y[0] -= 1;  break;
            case 's': snake.y[0] += 1;  break;
        }
        if (hitSth(snake)) {                            // JUDGE IF HIT THE WALL OR BODY BEFORE  DRAW  HEAD   SO THAT  THE HEAD WON'T  BE  IN  WALL  OR  BODY
            gameOver(snake);
            break;
        }
        printText(snake.x[0], snake.y[0], 7, "■");      //  DRAW  HEAD

        if (snake.x[0] == food.x && snake.y[0] == food.y) {
            generateFood(snake);
            snake.len++;
            snake.speed += snake.speed > MAXSPEED ? 0 : 15;
        }
        /*  USE  "  " REPLACE "■"  AS  TAIL  */
        else 
            printText(snake.x[snake.len], snake.y[snake.len], 7, "  ");

        locate(MAPWIDTH, MAPHEIGHT);
    }
}

int main() {
    //system("mode con cols=110 lines=60");
    initialize();
    //system("pause > nul");  // > nul: 不输出"请按任意键继续"
    return 0;
}