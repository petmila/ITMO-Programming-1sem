#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#define file (strcmp(argv[i], "--file") == 0) 
#define create (strcmp(argv[i], "--create") == 0)
#define extract (strcmp(argv[i], "--extract") == 0) 
#define list (strcmp(argv[i], "--list") == 0) 

void create_arch(char* archname, int argc, char* argv[])
{
    FILE* archfout;
    FILE* added_file;
    archfout = fopen(archname, "wb");
    fprintf(archfout, "%d\n", argc - 4);
    for (int i = 4; i < argc; i++)
    {
        added_file = fopen(argv[i], "rb");
        fseek(added_file, 0, SEEK_END);
        uint64_t size = ftell(added_file);
        fseek(added_file, 0, SEEK_SET);
        fclose(added_file);
        added_file = fopen(argv[i], "rb");
        fprintf(archfout, "%s %llu\n", argv[i], size);
        
        for (int t = 0; t < size; t++)
        {
            fputc(fgetc(added_file), archfout);
        }
        fprintf(archfout, " ");
        printf("File %s was added\n", argv[i]);
        fclose(added_file);
    }
    printf("Creating the archive ended.\n");
    fclose(archfout);
}
void extract_arch(char *archname)
{
    FILE* archfin;
    archfin = fopen(archname, "rb");
    char file_name[128] = "";
    uint64_t file_size;
    int file_count = 0;
    fscanf(archfin, "%d", &file_count);
    FILE* created_file;
    char nul_str[1];
    for (int i = 0; i < file_count; i++)
    {
        fscanf(archfin, "%s", file_name);
        fscanf(archfin, "%llu", &file_size);
        fread(nul_str, sizeof(char), 1, archfin);
        unsigned char* file_info;
		file_info = malloc(sizeof(char) * file_size);
		fread(file_info, sizeof(char), file_size, archfin);
        created_file = fopen(file_name, "wb");
        fwrite(file_info, sizeof(char), file_size, created_file);
        free(file_info);
        fclose(created_file);
    }
    remove(archname);
}
void list_arch(char *archname)
{
    FILE* archfin;
    archfin = fopen(archname, "rb");
    printf("List of files:\n");
    char file_name[128] = "";
    uint64_t file_size;
    int file_count = 0;
    fscanf(archfin, "%d", &file_count);
    for (int i = 0; i < file_count; i++)
    {
        fscanf(archfin, "%s", file_name);
        fscanf(archfin, "%llu", &file_size);
        unsigned char* file_info;
		file_info = malloc(sizeof(char) * file_size);
		fread(file_info, sizeof(char), file_size, archfin);
        fgetc(archfin);

        printf("%s\n", file_name);
        free(file_info);
    }
    fclose(archfin);
}
int main(int argc, char *argv[])
{
    char* archname;
    for (int i = 0; i < argc; i++)
    {
        if file
        {
            archname = argv[i + 1];
        }
        if create
        {
            create_arch(archname, argc, argv);
        }
        if extract
        {
            extract_arch(archname);
        }
        if list
        {
            list_arch(archname);
        }
    }
    return 0;
}
