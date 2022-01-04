#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#define input (strcmp(argv[i], "--input") == 0) 
#define output (strcmp(argv[i], "--output") == 0)
#define max_iter (strcmp(argv[i], "--max_iter") == 0) 
#define dump_freq (strcmp(argv[i], "--dump_freq") == 0) 
#define all_neighbours array_old[i + 1][j - 1] + array_old[i + 1][j] + array_old[i + 1][j + 1] + array_old[i][j - 1] + array_old[i][j + 1] + array_old[i - 1][j - 1] + array_old[i - 1][j] + array_old[i - 1][j + 1] 

long convert(char *str)
{
    long n = 0;
    int i = 0;
    while(i < strlen(str))
    {
        n += ((int)str[i] - '0')*(pow(10,strlen(str) - i - 1));
        ++i;
    }
    return n;
}
void game_of_life(int** array_old, int height, int width)
{
    int neighbour[height*width];
    int array_new[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array_new[i][j] = array_old[i][j];
        }
    }
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            array_new[i][j] = array_old[i][j];
            int choice = all_neighbours;
            if (array_new[i][j] == 1)
            {
                switch(choice)
                {
                    case 2: 
                        array_new[i][j] = array_old[i][j];
                        break;
                    case 3:
                        array_new[i][j] = array_old[i][j];
                        break;
                    default:
                        array_new[i][j] = 0;
                }
            }
            else
            {
                if (choice == 3)
                {
                    array_new[i][j] = 1;
                }
            }
            choice = 0;
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array_old[i][j] = array_new[i][j];  
        }
    }

}
int main(int argc, char *argv[])
{
    FILE *fin;
    int maxiter = 1, dumpfreq = 1;
    char* dirname;
    for (int i = 0; i < argc; i++)
    {
        if input
        {
            fin = fopen(argv[i + 1], "rb");
        }
        if output
        {
            dirname = argv[i + 1];
        }
        if max_iter
        {
            maxiter = convert(argv[i + 2]);
        }
        if dump_freq
        {
            dumpfreq = convert(argv[i + 2]);
        }
    }
    unsigned char bmp_header[14];
    fread(bmp_header, sizeof(char), 14, fin);
    int bmp_size = bmp_header[10]+ bmp_header[11]*256 + bmp_header[12]*256*256 + bmp_header[13]*256*256*256;
    int image_size = bmp_header[2]+ bmp_header[3]*256 + bmp_header[4]*256*256 + bmp_header[5]*256*256*256;

    unsigned char dib_header[bmp_size - 14];
    fread(&dib_header, sizeof(char), bmp_size - 14, fin);
    int image_width = dib_header[4] + dib_header[5]*256 + dib_header[6]*256*256 + dib_header[7]*256*256*256;
    int image_height = dib_header[8] + dib_header[9]*256 + dib_header[10]*256*256 + dib_header[11]*256*256*256;
    int data_size = dib_header[20]+ dib_header[21]*256 + dib_header[22]*256*256 + dib_header[23]*256*256*256;

    unsigned char image_data[data_size];
    fread(image_data, sizeof(char), data_size, fin);
    fclose(fin);

    int** image_array = (int**)malloc(image_height * sizeof(int*));
    for(int i = 0; i < image_height; i++)
        image_array[i] = (int*)malloc(image_width * sizeof(int));

    int index = -(image_width % 4);
    for(int i = image_height - 1; i >= 0; i--)
    {
        index += image_width % 4;
        for(int j = 0; j < image_width; j++)
        {
            if(image_data[index] == 255)
            {
                image_array[i][j] = 0;
            }
            else
            {
                image_array[i][j] = 1;
            }
            index += 3;
        }
    }
    char way_last[256];
	for (int generation_number = 0; generation_number <= maxiter; generation_number++)
    {
        if (generation_number % dumpfreq == 0)
        {
            char way[256] = "";
            int n = 0;
            int count = maxiter/(dumpfreq);
            while (count > 0)
            {
                count /= 10;
                n += 1;
            }
            char number_str[10000] = "";
            int iterator = generation_number/dumpfreq;
            for (int i = 0 ; i < n; i++)
            {
                number_str[i] = (int) (iterator/(pow(10, n - i - 1)))%10 + '0';
                //printf("%c\n", number_str[i]);
            }
            strcat(way, dirname);
            strcat(number_str, ".bmp");
            strcat(way, number_str);
            FILE* generation = fopen(way, "wb");
            
            fwrite(bmp_header, sizeof(char), 14, generation);
            fwrite(&dib_header, sizeof(char), bmp_size - 14, generation);
            int index = 0;
            for (int i = image_height - 1; i >= 0; i--)
            {
                for (int j = 0; j < image_width; j++)
                {
                    if (image_array[i][j] == 1)
                    {
                        image_data[index] = 0;
                        image_data[++index] = 0;
                        image_data[++index] = 0;
                        index++;
                    }
                    else
                    {
                        image_data[index] = 255;
                        image_data[++index] = 255;
                        image_data[++index] = 255;
                        index++;
                    }
                }
                while (index % 4 != 0) {
                    image_data[index++] = 0;
                }
            }
 			fwrite(image_data, sizeof(char), data_size, generation);
            strcpy(way_last, way);
            fclose(generation);
        }
        game_of_life(image_array, image_height, image_width);
    }
    printf("%s: %s\n", "Last generation way", way_last);
    strcpy(way_last, "");
    free(image_array);
    return 0;
}
