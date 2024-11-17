#include <stdio.h>
#include <stdlib.h>

void sort_ar(int ar[], int length)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length - 1; j++)
        {
            if (ar[j] > ar[j + 1])
            {
                int temp = ar[j];
                ar[j] = ar[j + 1];
                ar[j + 1] = temp;
            }
        }
    }
}

int binary_search(int ar[], int length, int search_item)
{
    int index = length / 2;
    sort_ar(ar, length);
    while (index != 0 && index != length)
    {
        if (ar[index] == search_item)
        {
            return 1;
        }
        else if (ar[index] < search_item)
        {
            index++;
        }
        else
        {
            index--;
        }
    }

    return 0;
}

int main()
{
    int ar[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int length = sizeof(ar) / sizeof(int);

    int search_item = 8;
    if (binary_search(ar, length, search_item))
    {
        printf("Item found\n");
    }
    else
    {
        printf("Item not found\n");
    }
}