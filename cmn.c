#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int *input = NULL;  /* m input number set array */
    int *location = NULL; /* n index of input number set */
    int m = 0;
    int n = 0;
    int cmn = 0;
    int idx = 0;
    int sub_idx = 0;
    int all_end = 0;

    /* input m, n and number set */
    do
    {
        printf("C m choose n, n can't be large than m, please input as following\n");
        printf("m: ");
        scanf("%d", &m);
        printf("n: ");
        scanf("%d", &n);
    } while (n > m);

    input = (int *)calloc(m, sizeof(int));
    location = (int *)calloc(n + 1, sizeof(int));
    printf("Please input %d number, seperate by space:\n", m);
    for (idx = 0; idx < m; idx++) scanf("%d", input + idx);
    /* initial location */
    for (idx = 0; idx < n; idx++) location[idx] = idx;
    location[n] = m; /* maximum range */

    /* generate all subset */
    printf("============= Result =============\n");
    do
    {
        /* output one of all subset */
        for (idx = 0; idx < n; idx++) printf("%d ", input[location[idx]]);
        printf("\n");
        cmn++;

        /* if all location over maximum range, means action end */
        all_end = 1;
        for (idx = n-1; idx >= 0; idx--)
        {
            /* if over range, increase previous one */
            if (location[idx] + 1 < location[idx+1])
            {
                location[idx]++;
                /* reset number after this node if in valid range */
                for (sub_idx = idx+1; sub_idx < n; sub_idx++)
                    location[sub_idx] = location[sub_idx-1] + 1;
                all_end = 0;
                break;
            }
        }
    } while (!all_end);
    printf("Total: %d\n", cmn);

    free(input);
    free(location);
    return 0;
}
