#include<Panic.h>
#include<Char.h>
#include<VideoInit.h>
void Panic(const char* str){
    ClearScreen(Black);
    ChangeCol(0);ChangeRow(0);
    SetBackColor(Black);
    SetForeColor(Red);
    printf("Kernel panic\n%s\n",str);
}