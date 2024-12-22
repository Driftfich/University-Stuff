#include <stdio.h>
#include <stdlib.h>



// c)
// merge method declaration for Mergesort
void merge(int *arr1, int *arr2, int length_1, int length_2, int *res)
{
    int i = 0, j = 0, k = 0;

    // Merge the arrays until one of them is exhausted
    while (i < length_1 && j < length_2)
    {
        if (arr1[i] <= arr2[j])
        {
            res[k++] = arr1[i++];
        }
        else
        {
            res[k++] = arr2[j++];
        }
    }

    // Copy the remaining elements of arr1, if any
    while (i < length_1)
    {
        res[k++] = arr1[i++];
    }

    // Copy the remaining elements of arr2, if any
    while (j < length_2)
    {
        res[k++] = arr2[j++];
    }
}

// Quicksort partition method declaration
int partition(int ar[], int length, int low[], int high[])
{
    int pivot = ar[0];
    int low_size = 0;
    int high_size = 0;
    for (int i = 1; i < length; i++)
    {
        if (ar[i] < pivot)
        {
            low[low_size] = ar[i];
            low_size++;
        }
        else
        {
            high[high_size] = ar[i];
            high_size++;
        }
    }
    ar[low_size] = pivot;
    return low_size;
}

int qmerge(int ar1[], int ar2[], int length1, int length2, int res[])
{
    int i;
    for (i = 0; i<length1; i++)
    {
        res[i] = ar1[i];
    }
    for (int j=0; j<length2; j++)
    {
        res[i+j+1] = ar2[j];
    }
}

// mergesort divide method
void divide(int ar[], int length, int low[], int high[], int mid)
{
    for (int i = 0; i < mid; i++)
    {
        low[i] = ar[i];
    }
    for (int i = mid; i < length; i++)
    {
        high[(i - mid)] = ar[i];
    }

    return;
}

void merge_sort(int ar[], int length)
{
    if (length > 1)
    {
        int mid = length / 2;
        int *low = (int *)malloc(mid * sizeof(int));
        int *high = (int *)malloc((length - mid) * sizeof(int));
        divide(ar, length, low, high, mid);
        // for (int i = 0; i < mid; i++)
        // {
        //     printf("%d ", low[i]);
        // }
        // printf("\n");
        // for (int i = 0; i < (length - mid); i++)
        // {
        //     printf("%d ", high[i]);
        // }
        // printf("\n");
        merge_sort(low, length / 2);
        merge_sort(high, length - mid);
        merge(low, high, (length / 2), (length - mid), ar);
    }
}

void quicksort(int ar[], int length)
{
    if (length > 1)
    {
        int *low = (int *)malloc(length * sizeof(int));
        int *high = (int *)malloc(length * sizeof(int));
        int low_size = partition(ar, length, low, high);
        for (int i = 0; i < low_size; i++)
        {
            printf("%d ", low[i]);
        }
        printf("\n");
        printf("%d\n", low_size);
        // quicksort(low, low_size);
        // quicksort(high, length - low_size - 1);

        for (int i = 0; i < low_size; i++)
        {
            printf("%d ", low[i]);
        }
        printf("\n");
        for (int i = 0; i < length - low_size - 1; i++)
        {
            printf("%d ", high[i]);
        }
        qmerge(low, high, low_size, length - low_size, ar);
    }
}

int main()
{
    // create random array with 20 elements in random order
    int arr[] =  {18, 5, 7, 0, 10, 40, 55, 3, 5, 6, 7, 2, 9};
    quicksort(arr, sizeof(arr) / sizeof(int));
    for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
    {
        printf("%d ", arr[i]);
    }
}
