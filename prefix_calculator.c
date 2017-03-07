#include <stdio.h>
#include <stdlib.h>

#define TYPE_OPER 1
#define TYPE_NUM  2

typedef struct item_s
{
    union
    {
        long double num;
        char        oper;
    };
    char          type;
    struct item_s *next;
    struct item_s *prev;
} ITEM_T;

ITEM_T *item_head = NULL;
ITEM_T *item_tail = NULL;

void add_item(ITEM_T *new, char type)
{
    if (item_tail == NULL) item_head = item_tail = new;
    else
    {
        item_tail->next = new;
        new->prev = item_tail;
        item_tail = new;
    }
    new->type = type;
}

void remove_item(ITEM_T *trash)
{
    if (trash->prev != NULL) trash->prev->next = trash->next;
    if (trash->next != NULL) trash->next->prev = trash->prev;
    free(trash);
}

void parse(char *argv, char *oper_stack)
{
    int idx = 0;
    int stack_count = 0;
    ITEM_T *new = NULL;

    while (argv[idx] != 0)
    {
        switch (argv[idx])
        {
            /* if 0~9, to push number in item list */
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
                new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                sscanf(argv + idx, "%Lf", &new->num);
                while (((argv[idx+1] >= '0') && (argv[idx+1] <= '9')) || (argv[idx+1] == '.'))
                {
                    ++idx;
                }
                add_item(new, TYPE_NUM);
                break;

            /* if '(', to push operator in operator template stack */
            case '(':
                oper_stack[stack_count++] = argv[idx];
                break;

            /* if ')', loop back for operator template stack and push into item list until '(' searched */
            case ')':
                while (oper_stack[--stack_count] != '(')
                {
                    new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                    new->oper = oper_stack[stack_count];
                    add_item(new, TYPE_OPER);
                }
                break;

            /* if operator, 
               loop to check the priority in top of operator template stack is higher,
               pop higher operator from stack and push into item list, 
               then push the lower operator into operator template stack */
            case '+':
                while ((stack_count > 0) &&
                    ((oper_stack[stack_count-1] == '-') ||
                     (oper_stack[stack_count-1] == '*') ||
                     (oper_stack[stack_count-1] == '/') ||
                     (oper_stack[stack_count-1] == '^')))
                {
                    new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                    new->oper = oper_stack[--stack_count];
                    add_item(new, TYPE_OPER);
                }
                oper_stack[stack_count++] = argv[idx];
                break;
            case '-':
                while ((stack_count > 0) && 
                    ((oper_stack[stack_count-1] == '+') ||
                     (oper_stack[stack_count-1] == '*') ||
                     (oper_stack[stack_count-1] == '/') ||
                     (oper_stack[stack_count-1] == '^')))
                {
                    new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                    new->oper = oper_stack[--stack_count];
                    add_item(new, TYPE_OPER);
                }
                oper_stack[stack_count++] = argv[idx];
                break;
            case '*':
                while ((stack_count > 0) &&
                    ((oper_stack[stack_count-1] == '/') ||
                     (oper_stack[stack_count-1] == '^')))
                {
                    new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                    new->oper = oper_stack[--stack_count];
                    add_item(new, TYPE_OPER);
                }
                oper_stack[stack_count++] = argv[idx];
                break;
            case '/':
                while ((stack_count > 0) &&
                    ((oper_stack[stack_count-1] == '*') ||
                     (oper_stack[stack_count-1] == '^')))
                {
                    new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
                    new->oper = oper_stack[--stack_count];
                    add_item(new, TYPE_OPER);
                }
                oper_stack[stack_count++] = argv[idx];
                break;
            case '^':
                oper_stack[stack_count++] = argv[idx];
                break;
        }
        idx++;
    }
    /* pop the least operator from operator template stack and push into item list */
    while (stack_count > 0)
    {
        new = (ITEM_T *)calloc(1, sizeof(ITEM_T));
        new->oper = oper_stack[--stack_count];
        add_item(new, TYPE_OPER);
    }
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

/* calculator: need a operator template stack and a item list to store calculate string */
int main(int argc, char **argv)
{
    int    size = str_len(argv[1]);
    char   *oper_stack = (char *)calloc(size, sizeof(char));

    /* parse argv and generate calculate list */
    parse(argv[1], oper_stack);
    free(oper_stack);

#ifdef DEBUG
    printf("Calculate string: ");
    item_tail = item_head;
    while (item_tail != NULL)
    {
        if (item_tail->type == TYPE_NUM) printf("%Lf ", item_tail->num);
        else printf("%c ", item_tail->oper);
        item_tail = item_tail->next;
    }
    printf("\n");
#endif

    /* calculating */
    item_tail = item_head;
    while (item_tail != NULL)
    {
        /* if item is operator, get the previous two number to calculate A operate B */
        if (item_tail->type == TYPE_OPER)
        {
            switch (item_tail->oper)
            {
                case '+':
                    item_tail->prev->prev->num += item_tail->prev->num;
                    break;
                case '-':
                    item_tail->prev->prev->num -= item_tail->prev->num;
                    break;
                case '*':
                    item_tail->prev->prev->num *= item_tail->prev->num;
                    break;
                case '/':
                    item_tail->prev->prev->num /= item_tail->prev->num;
                    break;
                case '^':
                    item_tail->prev->prev->num = pow(item_tail->prev->prev->num, (int)item_tail->prev->num);
                    break;
            }
            remove_item(item_tail->prev);
            remove_item(item_tail);
            item_tail = item_head;
        }
        else
        {
            /* the last one node in list is the result */
            if (item_tail->next == NULL) break;
            item_tail = item_tail->next;
        }
    }
    printf("Result: %Lf\n", item_tail->num);
    remove_item(item_tail);

    return 0;
}

