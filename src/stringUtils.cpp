
#include "stringUtils.h"


void GetTreeObjects (Text* MainText, FILE* input_file)
{
    read_file (input_file, MainText);

    calloc_objects (MainText);

    separate_lines (MainText);

    trim_left (MainText);    
}


void calloc_objects (Text *MainText)
{
    assert (MainText->buffer != nullptr);

    MainText->objects = (Line*) calloc (sizeof(Line), count_lines (MainText->buffer, MainText->symbols_amount));

    return;
}


int separate_lines (Text *MainText)
{
    assert (MainText->buffer != nullptr && MainText->objects != NULL && MainText->symbols_amount > 0);

    int lines_indx = 0, cur_len = 0;

    char *cur_ptr = MainText->buffer;

    char *end_ptr = cur_ptr + MainText->symbols_amount;

    for (; cur_ptr != end_ptr; cur_ptr++)
    {
        cur_len++;
                                                                                             //01234
        if (*cur_ptr == '\n')                                                                //abcd\ndef\n
        {                                                                                    //\n
            if (cur_len > 1) // ignore "\n"
            {
                MainText->objects[lines_indx].begin = cur_ptr - cur_len + 1;

                MainText->objects[lines_indx].length    = cur_len;
                *cur_ptr = '\0';
                lines_indx++;
            }

            cur_len = 0;
        }
    }

    MainText->lines_amount = lines_indx;

    return 1;
}



int count_lines (char *buffer, int symbols_read)
{
    char *cur_ptr = buffer;
    char *end_ptr = cur_ptr + symbols_read;

    int line_counter = 0;

    for (; cur_ptr != end_ptr; cur_ptr++)
    {
        if (*cur_ptr == '\n') line_counter++;
        // printf("%c", *cur_ptr);
    }

    return line_counter;
}


void write_result_in_file (Text *MainText, FILE* output_file)
{
    for (int i = 0; i < MainText->lines_amount; i++)
    {
        fputs (MainText->objects[i].begin, output_file);
        fputc ('\n', output_file);
    }
}


void print_lines (Line objects[], int lines_amount)
{
    assert (objects != NULL && lines_amount > 0);
    for (int i = 0; i < lines_amount; i++)
    {
        //printf("%.*s", objects[i].length, objects[i].begin);
        //printf ("Line length %d\n", objects[i].length);
        puts (objects[i].begin);

    }
}


void trim_left (Text *MainText)
{
    // printf ("Lines amount: %d\n", MainText->lines_amount);

    for (int i = 0; i < (MainText->lines_amount); i++)
    {
        while (*(MainText->objects[i].begin) == ' ' || *(MainText->objects[i].begin) == '\n')
        {
            MainText->objects[i].begin++;
        }
    }
}


char* GetTextBuffer (FILE* file)
{
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = (char*) calloc (size, sizeof (char));
    fgets (buffer, size, file);

    return buffer;
}


void InputDtor (Text *self)
{
    free (self->buffer);
    free (self->objects);
}


void ClearBuffer (char stop_sym)
{
    while (getchar() != stop_sym) ;
}


int read_file (FILE* file, Text *MainText)
{
    assert (file != nullptr);

    fseek (file, 0L, SEEK_END);
    int file_len = ftell (file);
    fseek (file, 0L, SEEK_SET);

    MainText->buffer = (char*) calloc(sizeof(char), file_len);
    MainText->symbols_amount = fread (MainText->buffer, sizeof(char), file_len, file);

    return 1;
}
