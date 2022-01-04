#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define nine_sumbols 1000000000

typedef struct uint1024_t
{
    uint32_t array[35];
} uint1024_t;

uint1024_t from_uint(unsigned int x, int i)
{
    uint1024_t array;
    for (int j = 0; j < 35; ++j)
    {
        array.array[j] = 0;
    }
    array.array[i] = x%nine_sumbols;
    array.array[i + 1] = x/nine_sumbols;
    return array;
}
uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t array;
    for (int j = 0; j < 35; ++j)
    {
        array.array[j] = 0;
    }
    uint32_t transport = 0;
    for (int i = 0; i < 35; ++i)
    {
        if ((int)x.array[i] + y.array[i] < nine_sumbols)
        {
            array.array[i] = x.array[i] + y.array[i] + transport;
            transport = 0;
        }
        else
        {
            uint64_t value = x.array[i] + y.array[i];
            array.array[i] = value%nine_sumbols;
            transport = value/nine_sumbols;
        }
    }
    return array;
}
uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t array;
    for (int j = 0; j < 35; ++j)
    {
        array.array[j] = 0;
    }
    for (int i = 0; i < 35; ++i)
    {
        if (x.array[i] < y.array[i])
        {
            array.array[i] = nine_sumbols + x.array[i] - y.array[i];
            x.array[i + 1] -= 1;
        }
        else
        {
            array.array[i] = x.array[i] - y.array[i];
        }
    }
    return array;
}
uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t array;
    for (int j = 0; j < 35; ++j)
    {
        array.array[j] = 0;
    }
    uint32_t transport = 0;
    for (int i = 0; i < 35; ++i)
    {
        for (int j = 0; j < 35; ++j)
        {
            if (i + j < 35)
            {
            uint64_t new = array.array[j+i] + ((uint64_t) x.array[i]) * y.array[j] + transport;
            array.array[j+i] = (new)%nine_sumbols;
            transport = new/nine_sumbols;
            }
        }
    }
    return array;
}
void printf_value(uint1024_t x)
{
    uint8_t state_global = 0;
    uint8_t state_in = 0;
    for (int i = 34; i >= 0; --i)
    {
        if (x.array[i] != 0)
        {
            state_global = 1;
        }
        if (state_global == 1)
        {
            uint32_t value = x.array[i];
            uint32_t unit = nine_sumbols;
            for (int j = 0; j < 10; j++)
            {
                if (value/unit != 0)
                {
                    state_in = 1;
                }
                if (state_in == 1 && j != 0)
                {
                    printf("%d",(int) value/unit);
                }
                value = value%unit;
                unit /= 10;
            }
        }
    }
    printf("\n");
}
void scanf_value(uint1024_t *x)
{
    char str[1024];
    scanf("%s", str);
    uint32_t value = 0;
    for (int j = 0; j < 35; ++j)
    {
        x->array[j] = 0;
    }
    int set[9];
    for (int h = 0; h < 9; h++)
    {
        set[h] = 0;
    }
    int array_count = 0;
    int i = strlen(str) - 1;
    int j = 0;
    while (i >= 0)
    {
        j = 0;
        while (i >= 0 && j < 9)
        {
            set[j] = str[i] - '0';
            j++;
            i--;
        }
        for (int h = 8; h > 0; --h)
        {    
            x->array[array_count] += set[h];
            x->array[array_count] *= 10;
            
        }
        x->array[array_count] += set[0];
        for (int h = 0; h < 9; h++)
        {
            set[h] = 0;
        }
        array_count++;
    }
}
int main()
{
    uint1024_t something = from_uint(1023443444, 0);
    uint1024_t anything = from_uint(99998, 0);
    uint1024_t summ = add_op(something, anything);
    uint1024_t diff = subtr_op(something, anything);
    uint1024_t mult = mult_op(something, anything);
    printf_value(something);
    printf_value(anything);
    printf_value(summ);
    printf_value(diff);
    printf_value(mult);
    uint1024_t array;
    uint1024_t *x = &array;
    scanf_value(x);
    printf_value(array);
    uint1024_t mult_2 = mult_op(array, array);
    printf_value(mult_2);
    return 0;
}
