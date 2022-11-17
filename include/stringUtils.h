#ifndef ONEGIN_H
#define ONEGIN_H


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "fileUtils.h"
#include "stringUtils.h"

//-----------------------------------------------------------------------------

struct Line
{
    char* begin;
    int   length;
};

//-----------------------------------------------------------------------------

struct Text
{
    char *buffer;
    int  symbols_amount;
    int  lines_amount;
    Line *objects;
};

//-----------------------------------------------------------------------------

void GetTreeObjects (Text* MainText, FILE* input_file);

int read_file (FILE* file, Text *MainText);

int separate_lines (Text *MainText);

void print_single_line (Line *cur_line);

void print_lines (Line objects[], int line_amount);

void trim_left (Text *MainText);

void trim_right (Line objects[], int lines_amount);

void write_result_in_file (Text *MainText, FILE* output_file);

void calloc_objects (Text *MainText);

int count_lines (char *buffer, int symbols_read);

void InputDtor(Text *self);
 
void GetTreeObjects (Text* MainText, FILE* input_file);

void ClearBuffer (char stop_sym);

char* GetTextBuffer (FILE* file);

//-----------------------------------------------------------------------------

#endif


