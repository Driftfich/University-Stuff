#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int do_calculation(char input[])
{
    printf("Ihre Eingabe: %s\n", input);

    // extract numbers and operator
    char operator;
    float num1, num2;

    sscanf(input, "%f %c %f", &num1, &operator, &num2);

    // calculate result
    float result;

    switch (operator)
    {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;

        case '/': 
            if (num2 == 0)
            {
                printf("Division durch 0 nicht möglich\n");
                return 1;
            }
            result = num1 / num2;
            break;

        default:
            printf("Ungültiger Operator\n");
            return 1;
    }


    printf("Das Ergebnis ist: %f\n", result);

    return 0;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main()
{
    while (1==1)
    {
        // get input from user
        printf("Bitte geben Sie eine Rechnung ein ('break' zum beenden): \n");
        char input[200] = "";
        scanf("%199[^\n]", input);

        clear_input_buffer();

        if (strstr(input, "break") != NULL)
            {
                break;
            }
        else
            {
                do_calculation(input);
            }
    }

    printf("Programm beendet\n");
    
    return 0;
}