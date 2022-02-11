/*为每个源文件设置独特的编号并用__dummynum函数来防止出现重复编号*/
#define FILENUM(num) \
    enum { F_NUM=num }; \
    void _dummy##num(void) {}

#define ASSERT(expr) \
    ((expr) || \
        aFailed(__FILE__, __LINE__))



int aFailed(char*file, int line);
/*得到完整文件路径中的文件名*/
int GetFilePointer(char*str);

