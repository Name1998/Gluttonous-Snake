#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#define Width 45    // define map width
int main() {

    srand((unsigned)time(NULL));//malloc(1)); //initialize seed
    do
    {
        int i = 0;
        int hX = Width/2 , hY = Width/2, len =4; //head position, body length
        int map[Width * Width] = {0}; //map,0:nothing,-1: food, >0: body

        for (i = 0; i < Width; i += 2)///新增地图元素：-2:墙
            map[i] = map[Width * Width - 1 - i] = map[i * Width] = map[i*Width + Width - 1] = -2;//使四周的墙都隔一格分布

        char c = 'd', cl = 'd', deaw[Width * Width * 2 + 1] = {0}; // default position, input cache, drawing cache
        //system("mode con: cols=60 lines=30"); //set cmd window size
        sprintf_s(deaw, 32, "mode con: cols=%d lines=%d", Width * 2, Width);
        system(deaw);
        for (int num = 3; num; num--)///生成多个食物，num控制数量
        {
            do i = rand() % (Width * Width);
            while (map[i]);
            map[i] = -1;
        }

        for (system("title Score:0"); 1; Sleep(100)) {     //gen food, delay
            if (_kbhit() && (cl = _getch()))     //detect input
                switch (cl) {
                    case 'a':case 'A':
                        if (c != 'd') c = 'a';  break;
                    case 'd':case 'D':
                        if (c != 'a') c = 'd';  break;
                    case 'w':case 'W':
                        if (c != 's') c = 'w';  break;
                    case 's':case 'S':
                        if (c != 'w') c = 's';  break;
                    case ' ':
                        _getch();   break;
                    case 27:
                        exit(0);    break;
                }
            switch (c) {
                case 'a':
                    hX -= hX > 0 ? 1 : 1 - Width;
                    break;   //move head
                case 'd':
                    hX += hX < Width - 1 ? 1 : 1 - Width;
                    break;
                case 's':
                    hY += hY < Width - 1 ? 1 : 1 - Width;
                    break;
                case 'w':
                    hY -= hY > 0 ? 1 : 1 - Width;
                    break;
            }
            if (map[hY * Width + hX] > 1 || map[hY * Width + hX] == -2)                   //hY*30+hX is the position in 2d view
                break;      //head hit body or wall
                //exit(!_getch());
            if (map[hY * Width + hX] == -1) {    //when ate food
                len++;
                do i = rand() % (Width * Width);
                while (map[i]);
                map[i] = -1;            //judge if mao[i] is body, if not gen food;
                sprintf_s(deaw, 32, "title Score:%d", len - 4);///新增：计分板
                system(deaw);
            } else
                for (i = 0; i < Width * Width; i++)
                    if (map[i] > 0) map[i] -= 1;   //body value -1

            map[hY * Width + hX] = len;      // refresh head value
            // draw in deaw[]
            for (i = 0; i < Width * Width * 2; i++) {
                if (map[i / 2] == 0)
                    deaw[i] = ' ';
                else if (map[i / 2] > 0)
                    deaw[i] = (i % 2) ? ')' : '(';
                else if (map[i / 2] == -2)deaw[i] = (i & 1) ? ']' : '['; ///新增：墙
                else deaw[i] = '0';
            }
            system("cls");
            printf(deaw);
        }
        sprintf_s(deaw, 48, "title GameOver!Score:%dPressSpaceBarToStartAgain", len - 4);
        system(deaw);
    }while(_getch() == ' ');




    //printf("Hello, World!\n");
    return 0;
}
