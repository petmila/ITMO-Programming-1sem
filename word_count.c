#include <stdio.h>
#include <string.h>
#define IN 1  /* в слове */
#define OUT 0 /* не в слове */
#define counting_lines (strcmp(argv[1], option[0]) == 0) || (strcmp(argv[1], option[3]) == 0) /* подсчет количества строк */
#define counting_words (strcmp(argv[1], option[2]) == 0) || (strcmp(argv[1], option[5]) == 0) /* подсчет количества слов */
#define counting_bytes (strcmp(argv[1], option[1]) == 0) || (strcmp(argv[1], option[4]) == 0)  /* подсчет количества байт */

int solve(int argc, char *argv[])
{
    char *option[6] = {"--lines", "--bytes", "--words", "-l", "-c", "-w"}; //список опций
    FILE *fin;
    fin = fopen(argv[2], "r");
    if (fin == NULL)
    {
        return -1;
    }
    int count, state;
    char symb;
    state = OUT; // изначальное состояние - не в слове
    count = 0;

    while ((symb = getc(fin)) != EOF)
    {
        if (counting_lines)
        {
            if (symb == '\n' || symb == '\t')
            {
                ++count;
            }
        }
        else if (counting_words)
        {
            if (symb == ' ' || symb == '\n' || symb == '\t')
            {
                state = OUT;
            }
            else if (state == OUT)
            {
                state = IN;
                ++count;
            }
        }
    }
    if (counting_bytes) 
    {
        count = ftell(fin);
    }
    fclose(fin);
    return count;
}
int main(int argc, char *argv[])
{
    printf("%d", solve(argc, argv));
    return 0;
}

