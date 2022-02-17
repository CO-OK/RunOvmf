#include<Assert.h>
#include<Char.h>
FILENUM(6);
    
// const char* fileName[]={
//         "Char.c",
//         "Kernel.c",
//         "Memory32.c",
//         "PageFrameAllocator.c",
//         "Shell.c",
//         "Video.c",
//         "Assert.c"
//     };
// int aFailed(int num,int line){
//     switch (num)
//     {
//     case 0:
//         printf("Assertion failed at line %d in %s\n",line,"Char.c");
//         break;
//     case 1:
//         printf("Assertion failed at line %d in %s\n",line,"Kernel.c");
//         break;
//     case 2:
//         printf("Assertion failed at line %d in %s\n",line,"Memory32.c");
//         break;
//     case 3:
//         printf("Assertion failed at line %d in %s\n",line,"PageFrameAllocator.c");
//         break;
//     case 4:
//         printf("Assertion failed at line %d in %s\n",line,"Shell.c");
//         break;
//     case 5:
//         printf("Assertion failed at line %d in %s\n",line,"Video.c");
//         break;
//     case 6:
//         printf("Assertion failed at line %d in %s\n",line,"Assert.c");
//         break;
//     default:
//         printf("Assertion failed\n");
//         break;
//     }
    
//     while(1){}
// }
int aFailed(char*file, int line){
    printf("Assertion failed at line %d in file: %s\n",line,file+GetFilePointer(file));
    while(1){}
}
int GetFilePointer(char*str){
    int pos=0;
    while(*str!='\0'){
        if(*str=='/'){
            /*向后查找有没有 / */
            str++;
            pos++;
            return pos+GetFilePointer(str);
        }
        else{
            str++;
            pos++;
        }
    }
    return 0;
}