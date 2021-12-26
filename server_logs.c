#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
struct tm do_time(char *second)
{
    struct tm tim;
    int time_1 = (int)second[strlen(second) - 1] - 48 + ((int)second[strlen(second) - 2] - 48) * 10;
    int time_2 = (int)second[strlen(second) - 4] - 48 + ((int)second[strlen(second) - 5] - 48) * 10;
    int time_3 = (int)second[strlen(second) - 7] - 48 + ((int)second[strlen(second) - 8] - 48) * 10;
    char time_0[3];
    char symb = second[4];
    for (int i = 0; i < 3; ++i)
    {
        time_0[i] = second[4 + i];
    }
    int time_4 = -1;
    switch (time_0[2])
    {
    case 'n':
        time_4 = 0;
        break;
    case 'b':
        time_4 = 1;
        break;
    case 'r':
        time_4 = 2;
        break;
    case 'y':
        time_4 = 4;
        break;
    case 'l':
        time_4 = 6;
        break;
    case 'g':
        time_4 = 7;
        break;
    case 'p':
        time_4 = 8;
        break;
    case 't':
        time_4 = 9;
        break;
    case 'v':
        time_4 = 10;
        break;
    case 'c':
        time_4 = 11;
        break;
    }
    if (time_4 == 2 && time_0[1] == 'p')
    {
        time_4 = 3;
    }
    if (time_4 == 0 && time_0[1] == 'u')
    {
        time_4 = 5;
    }
    int time_5 = (int)second[11] - 48 + ((int)second[10] - 48) * 10 + ((int)second[9] - 48) * 100 + ((int)second[8] - 48) * 1000 - 1900;
    int time_6 = (int)second[2] - 48 + ((int)second[1] - 48) * 10;
    tim.tm_sec = time_1;
    tim.tm_min = time_2;
    tim.tm_hour = time_3;
    tim.tm_year = time_5;
    tim.tm_mon = time_4;
    tim.tm_mday = time_6;
    return tim;
}
int count_error()
{
    FILE *fin;
    fin = fopen("log_2.txt", "r");
    FILE *fout;
    fout = fopen("leg_2.txt", "w");
    if (fin == NULL)
    {
        return -1;
    }
    char first[1024];
    strcpy(first, "");
    int count_error = 0;
    while (fin != NULL)
    {
        char second[1024];
        char last[1024];
        int state = 0;
        int counter = -1;
        for (int i = 0; i < 12; ++i)
        {
            strcpy(second, "");
            fscanf(fin, "%s", second);
            --counter;
            if (i == 3)
            {
                struct tm tim = do_time(second);
                char str_time[256];
                strftime(str_time, 256, "%d/%b/%Y:%H:%M:%S", &tim);
                fprintf(fout, "%s", "[");
                fprintf(fout, "%s\n", str_time);
            }
            if (strcmp(second, "") == 0)
            {
                break;
            }
            if (last[strlen(last) - 1] == '"')
            {
                if (second[0] == '5')
                {
                    state = 1;
                }
                counter = 1;
                strcat(first, " ");
                strcat(first, second);
            }
            else
            {
                strcat(first, " ");
                strcat(first, second);
            }
            if (state == 1 && counter == 0)
            {
                printf("%s", first);
                printf("\n");
                ++count_error;
                state = 0;
            }
            if (counter == 0)
            {
                break;
            }
            strcpy(last, second);
            strcpy(second, "");
        }

        if (strcmp(first, "") == 0)
        {
            printf("%d", count_error);
            printf("\n");
            fclose(fin);
            fclose(fout);
            return count_error;
        }
        strcpy(first, "");
    }
    fclose(fin);
    fclose(fout);
    return count_error;
}
int time_window(int time_period)
{
    FILE *fout_time;
    fout_time = fopen("leg_2.txt", "r");
    FILE *fout_time_2;
    fout_time_2 = fopen("leg_2.txt", "r");

    if (time_period == 0)
    {
        return -1;
    }
    double timing = 0;
    int max = 0;
    int count_max = 0;
    char result[256];
    strcpy(result, "!");
    char third[1024];
    strcpy(third, "");
    fscanf(fout_time_2, "%s", third);
    while (fout_time != NULL)
    {
        char second[1024];
        strcpy(second, "");
        fscanf(fout_time, "%s", second);
        if (strcmp(second, "") == 0)
        {
            fclose(fout_time);
            fclose(fout_time_2);
            printf("%s %d", result, max);
            return max;
        }
        struct tm tim = do_time(second);
        struct tm tim_2 = do_time(third);
        time_t timer;
        time_t timer_2;
        timer = mktime(&tim);
        timer_2 = mktime(&tim_2);
        timing = difftime(timer, timer_2);
        ++count_max;
        while (timing >= time_period)
        {
            --count_max;
            strcpy(third, "");
            fscanf(fout_time_2, "%s", third);
            struct tm tim_2 = do_time(third);
            time_t timer_2;
            timer_2 = mktime(&tim_2);
            timing = difftime(timer, timer_2);
        }
        if (count_max >= max)
        {
            max = count_max;
            if (strcmp(second, "") != 0)
            {
                char help_str[256];
                strcpy(help_str, "");
                second[0] = ' ';
                strcat(help_str, third);
                strcat(help_str, " ;");
                strcat(help_str, second);
                strcat(help_str, "]");
                strcpy(result, help_str);
            }
        }
    }
}
int main()
{
    int time_period = 0;
    scanf("%d", &time_period);
    int result = count_error();
    int window = time_window(time_period);
    return 0;
}
