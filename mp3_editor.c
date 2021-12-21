#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <inttypes.h>
#define it_is_the_needed_frame frame_id[0] == needed_frame[0] && frame_id[1] == needed_frame[1] && frame_id[2] == needed_frame[2] && frame_id[3] == needed_frame[3]
#define show_command (strcmp(argv[i], "--show") == 0) 
#define get_command (strcmp(argv[i], "--get") == 0)
#define set_command (strcmp(argv[i], "--set") == 0) 

uint32_t convert(uint32_t n)
{
	return ((n >> 24) & 0x000000ff) | ((n >> 8) & 0x0000ff00) | ((n << 8) & 0x00ff0000) | ((n << 24) & 0xff000000);
}

void show(char* filepath)
{
	FILE* fin;
	fin = fopen(filepath, "rb");
	if (fin == NULL)
	{
		printf("%s", "Error, empty file");
		return;
	}
	fseek(fin, 0, SEEK_SET);
	unsigned char tag_heading[6];
	fread(tag_heading, sizeof(char), 6, fin);
	uint32_t tag_size;
	fread(&tag_size, sizeof(char), 4, fin);
    printf("%d\n", tag_size);
	tag_size = convert(tag_size) + 10;
    printf("%d\n", tag_size);
	while (ftell(fin) < tag_size)
	{
		unsigned char frame_id[3];
		fread(frame_id, sizeof(char), 4, fin);
		uint32_t frame_size;
		fread(&frame_size, sizeof(char), 4, fin);
		frame_size = convert(frame_size);
		uint16_t flags;
		fread(&flags, sizeof(char), 2, fin);
		if (frame_id[0] == 0)
		{
            break;
		}
		unsigned char* info;
		info = malloc(sizeof(char) * frame_size);
		fread(info, sizeof(char), frame_size, fin);
		printf("%c%c%c%c: ", frame_id[0], frame_id[1], frame_id[2], frame_id[3]);
		for (int i = 0; i < frame_size; i++)
		{
			printf("%c", info[i]);
		}
		printf("\n");
		
		free(info);
	}
	fclose(fin);
}

void get(char* filepath, char needed_frame[4])
{
	FILE* fin;
	fin = fopen(filepath, "rb");
	if (fin == NULL)
	{
		printf("%s", "Error, empty file");
		return;
	}
	fseek(fin, 0, SEEK_SET);
	unsigned char tag_heading[6];
	fread(tag_heading, sizeof(char), 6, fin);
	uint32_t tag_size;
	fread(&tag_size, sizeof(char), 4, fin);
	tag_size = convert(tag_size) + 10;
	while (ftell(fin) < tag_size)
	{
		unsigned char frame_id[3];
		fread(frame_id, sizeof(char), 4, fin);
		uint32_t frame_size;
		fread(&frame_size, sizeof(char), 4, fin);
		frame_size = convert(frame_size);
		uint16_t flags;
		fread(&flags, sizeof(char), 2, fin);
		unsigned char* info;
		info = malloc(sizeof(char) * frame_size);
		fread(info, sizeof(char), frame_size, fin);
		if (it_is_the_needed_frame)
		{
			printf("%s: ", frame_id);
			for (int i = 0; i < frame_size; i++)
			{
				printf("%c", info[i]);
			}
			printf("\n");
			free(info);
			return;
		}
		free(info);
		if (frame_id[0] == 0)
		{
			break;
		}
	}
	fclose(fin);
	printf("%s", "Error, no such frame");
}
		
void set(char* filepath, char needed_frame[4], char* frame_value)
{
	FILE* fin;
	fin = fopen(filepath, "rb");
    if (fin == NULL)
	{
		printf("%s", "Error, empty file");
		return;
	}
	FILE* fin_copy;
	fin_copy = fopen("file_copy.mp3", "wb");
	unsigned char tag_heading[6];
	fread(tag_heading, sizeof(char), 6, fin);
    fwrite(tag_heading, sizeof(char), 6, fin_copy);
	uint32_t tag_size;
	fread(&tag_size, sizeof(char), 4, fin);
    uint32_t tag_size_new_pred = convert(tag_size - 10 + strlen(frame_value));
    fwrite(&tag_size_new_pred, sizeof(char), 4, fin_copy);
	tag_size = convert(tag_size) + 10;
 	uint32_t frame_size_old = 0;
 	uint32_t frame_size_new = 0;

	while (ftell(fin) < tag_size)
	{
		unsigned char frame_id[4];
		fread(frame_id, sizeof(char), 4, fin);
		uint32_t frame_size;
		fread(&frame_size, sizeof(char), 4, fin);
		frame_size = convert(frame_size);
		uint16_t flags;
		fread(&flags, sizeof(char), 2, fin);
		if (frame_id[0] == 0)
		{
			break;
		}
		unsigned char* info;
		info = malloc(sizeof(char) * frame_size);
		fread(info, sizeof(char), frame_size, fin);
		if (it_is_the_needed_frame)
		{
			printf("%s", "This frame had not existed here before, i swear");
            frame_size_old = frame_size;
		}
        else
        {
            fwrite(frame_id, sizeof(char), 4, fin_copy);
            frame_size = convert(frame_size);
            fwrite(&frame_size, sizeof(char), 4, fin_copy);
            fwrite(&flags, sizeof(char), 2, fin_copy);
            fwrite(info, sizeof(char), convert(frame_size), fin_copy);
        }
		free(info);
	}
    uint32_t frame_size = convert(strlen(frame_value));
    fwrite(needed_frame, sizeof(char), 4, fin_copy);
    fwrite(&frame_size, sizeof(char), 4, fin_copy);
    uint16_t flags = 0;
    fwrite(&flags, sizeof(char), 2, fin_copy);
    fwrite(frame_value, sizeof(char), convert(frame_size), fin_copy);
    int c;
	while ((c = getc(fin)) != EOF)
	{
		putc(c, fin_copy);
	}
	uint32_t tag_size_new = convert(tag_size - 10 - frame_size_old + strlen(frame_value));
 	
	fclose(fin);
	fclose(fin_copy);
    //переписывание обратно в исходный файл
	fin_copy = fopen("file_copy.mp3", "rb");
    fin = fopen(filepath, "wb");
    fread(tag_heading, sizeof(char), 6, fin);
    fwrite(tag_heading, sizeof(char), 6, fin_copy);
	fread(&tag_size, sizeof(char), 4, fin);
    fwrite(&tag_size_new, sizeof(char), 4, fin_copy);
	while ((c = getc(fin_copy)) != EOF)
	{
		putc(c, fin);
	}
 	fclose(fin);
 	fclose(fin_copy);
}

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		if show_command{
			show(argv[3]);
			continue;
		}
		if get_command{
			get(argv[3], argv[i + 2]);
			continue;
		}
		if set_command{
			set(argv[3], argv[i + 2], argv[i + 5]);
			continue;
		}
	}
	return 0;
}
