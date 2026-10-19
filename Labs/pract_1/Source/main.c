#include <pic.h>

CONFIG(XT & WDTEN & PWRTEN & UNPROTECT);

unsigned char TMP1;
unsigned char TMP2;
unsigned char RESULT;

unsigned char SUM(void);

unsigned char SUM(void)
{
    return TMP1 + TMP2;
}

void main(void)
{
    TMP1 = 5;
    TMP2 = 3;
    RESULT = 0;

    while (1)
    {
        RESULT = SUM();
    }
}
