#include <stdio.h>
#include <stdlib.h>

void debug(float *num_stack, int num_idx, char *oper_stack, int oper_idx)
{
    int idx = 0;

    printf("===== DEBUG =====\n");
    printf("NUM_STACK : ");
    for (idx = 0; idx < num_idx; idx++) printf("%f ", num_stack[idx]);
    printf("\n");
    printf("OPER_STACK: ");
    for (idx = 0; idx < oper_idx; idx++) printf("%c ", oper_stack[idx]);
    printf("\n\n");
}

int str_len(char *str)
{
    int len = 0;
    while (*str++ != 0) len++;
    return len;
}

long double pow(long double a, int b)
{
    long double base = a;
    while (--b > 0) base *= a;
    return base;
}

float calc(char oper, float num1, float num2)
{
    switch (oper)
    {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
        case '^': return pow(num1, (int)num2);
    }
    return 0;
}

/* calculator: need a operator stack and a number stack to calculate */
int main(int argc, char **argv)
{
    char   *str = argv[1];
    int    size = str_len(str);
    char   *oper_stack = (char *)calloc(size, sizeof(char));
    float  *num_stack = (float *)calloc(size, sizeof(float));
    int    oper_idx = 0;
    int    num_idx = 0;
    int    idx = 0;
    int    flag = 1;

    while (str[idx] != 0)
    {
        switch (str[idx])
        {
            /* 0~9, to load until a completed number and push into number stack */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                sscanf(str + idx, "%f", &num_stack[num_idx]);
                num_stack[num_idx++] *= flag;
                flag = 1;
                while (((str[idx+1] >= '0') && (str[idx+1] <= '9')) || (str[idx+1] == '.'))
                {
                    ++idx;
                }
                flag = 1;
                break;

            /* ( push into operator stack */
            case '(':
                oper_stack[oper_idx++] = str[idx];
                break;

            /* ) pop and calc (second final number) (final oper) (final number), push result into number stack, 
                 repeat pop until touch ( */
            case ')':
                while (oper_stack[--oper_idx] != '(')
                {
                    num_stack[num_idx-2] = calc(oper_stack[oper_idx], num_stack[num_idx-2], num_stack[num_idx-1]);
                    num_idx--;
                }
                break;

            // +-*/^, check if the priority of final operator in stack is equal or higher than now reading, 
            /*        if true, calc (second final number) (final oper) (final number), push result into number stack, 
                      repeat pop until touch ( */
            case '-':
                /* to check if negative operator */
                if ((idx == 0) || (str[idx-1] == '('))
                {
                    flag = -1;
                    break;
                }
            case '+':
                while ((oper_idx > 0) &&
                    ((oper_stack[oper_idx-1] == '+') ||
                     (oper_stack[oper_idx-1] == '-') ||
                     (oper_stack[oper_idx-1] == '*') ||
                     (oper_stack[oper_idx-1] == '/') ||
                     (oper_stack[oper_idx-1] == '^')))
                {
                    num_stack[num_idx-2] = calc(oper_stack[oper_idx-1], num_stack[num_idx-2], num_stack[num_idx-1]);
                    num_idx--;
                    oper_idx--;
                }
                oper_stack[oper_idx++] = str[idx];
                break;
            case '*':
            case '/':
                while ((oper_idx > 0) &&
                    ((oper_stack[oper_idx-1] == '*') ||
                     (oper_stack[oper_idx-1] == '/') ||
                     (oper_stack[oper_idx-1] == '^')))
                {
                    num_stack[num_idx-2] = calc(oper_stack[oper_idx-1], num_stack[num_idx-2], num_stack[num_idx-1]);
                    num_idx--;
                    oper_idx--;
                }
                oper_stack[oper_idx++] = str[idx];
                break;
            case '^':
                oper_stack[oper_idx++] = str[idx];
                break;
        }
        idx++;
#ifdef DEBUG
        debug(num_stack, num_idx, oper_stack, oper_idx);
#endif
    }

    /* calculate the least stack, the final result will stored in num_stack[0] */
    while (oper_idx > 0)
    {
        num_stack[num_idx-2] = calc(oper_stack[oper_idx-1], num_stack[num_idx-2], num_stack[num_idx-1]);
        num_idx--;
        oper_idx--;
    }
    printf("%f\n", num_stack[0]);

    free(num_stack);
    free(oper_stack);
    return 0;
}

