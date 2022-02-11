/*为每个源文件设置独特的编号并用__dummynum函数来防止出现重复编号*/
#define FILENUM(num) \
    enum { F_NUM=num }; \
    void _dummy##num(void) {}

#define ASSERT(expr) \
    ((expr) || \
        aFailed(F_NUM, __LINE__))



int aFailed(int num, int line);

