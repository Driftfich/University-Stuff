#include <stdlib.h>
#include <stdio.h>

char *crtBitStr(unsigned int x){
    int size=sizeof x * 9-2;
    static char str[sizeof x * 9-1];
    for (int i=size; i>=0; i--) {
        if ((i+1) % 9 == 0) {
            str[i] = '|';
        }
        else {
            str[i] = '0'+(x&1);
            x>>=1;
        }
    }
    return str;
}

char* crtBitStrShort(unsigned int x)
{
    int size = sizeof x * 9;
    static char res[sizeof x * 8 + (sizeof x / 2) - 2];
    int i;

    for (i=size-2; i>=0; i--)
    {   
        if (((i+1)%9==0))
        {
            res[i] = '|';
        }
        else
        {
        res[i] = '0'+(x&1);
        x>>=1;
        }
    }
    return res;
}

int main() {
    char buf[256];
    printf("Bitte eine ganzahlige Zahl eingeben: ");
    fgets(buf, sizeof(buf), stdin);
    unsigned int x = atoi(buf);
    printf("%ld\n", sizeof x * 8);
    printf("***\n");
    printf("dez: %d\n", x);
    printf("hex: %x\n", x);
    printf("bin: %s\n", crtBitStr(x));
    // printf("bin: %s\n", crtBitStrShort(x));
    printf("***\n");
    return 0;
}