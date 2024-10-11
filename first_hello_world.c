

#include <stdio.h>
#include <stdlib.h>

#define MAX_NOTEN 20 // This is a macro
int Noten[] = {1,2,3,4,5,6,3,2,1,3};

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
    }

int arithmetischer_mittelwert(int *arr)
{
    float Akku = 0.0;
    int Counter = 0;
    int local_Noten[] = Noten;
    int Noten_count = sizeof(local_Noten)/sizeof(Noten[0]);
    Noten_count = min(Noten_count, MAX_NOTEN);
    
    for (int i = 0; i < sizeof(Noten)/sizeof(Noten[0]); i++) {
        Akku += Noten[i];
        Counter++;
    }
    
    if (Counter > 0)
    {
        Akku = Akku / Counter;
        printf("Durschnitt: %f", Akku);
    }
    else
    {
        printf("Keine Noten vorhanden");
    }
}



int main()
{
    arithmetischer_mittelwert(Noten);
    return 0;
}




