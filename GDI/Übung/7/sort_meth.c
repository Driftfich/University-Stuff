#include <stdio.h>
#include <stdlib.h>



// c)
// merge method declaration for Mergesort
void merge(int *arr1, int *arr2, int length_1, int length_2, int *res)
{
    int length;
    if (length_1 > length_2)
    {
        length = length_2;
        res[length_1 + length_2] = arr1[length_1];
    }
    else if (length_1 < length_2)
    {
        length = length_1;
        res[length_1 + length_2] = arr2[length_2];
    }
    else length = length_1-1;

    for (length; length>=0; length--)
    {
        printf("arr1[%d\n]: %d\n", length, arr1[length]);
        printf("arr2[%d\n]: %d\n", length, arr2[length]);
        if (arr1[length] > arr2[length])
        {
            printf("first\n");
            res[length*2 +1] = arr1[length];
            res[length*2] = arr2[length];
        }
        else
        {
            res[length*2 +1] = arr1[length];
            res[length*2] = arr2[length];
        }
    }
}

// Quicksort partition method declaration
int partition(int ar[], int length, int low[], int high[])
{
    int pivot = ar[0];
    for (int i = 1; i < length; i++)
    {
        if (ar[i] < pivot)
        {
            low[i] = ar[i];
        }
        else
        {
            high[i] = ar[i];
        }
    }
}

int main()
{
    int arr1[] = {1, 3, 5, 7, 9};
    int arr2[] = {2, 4, 6, 8, 10};
    int length_1 = sizeof(arr1) / sizeof(arr1[0]);
    int length_2 = sizeof(arr2) / sizeof(arr2[0]);
    int res[length_1 + length_2];
    merge(arr1, arr2, length_1, length_2, res);
    for (int i = 0; i < length_1 + length_2; i++)
    {
        printf("%d ", res[i]);
    }
}
