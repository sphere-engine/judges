#include "spoj.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define NOMATCH -1

int strcmp_n(const char *a, const char *b, int n)
{
    int i;
    int result = 1;

    for(i=0; i < n; i++)
        if (a[i] != b[i])
            result = 0;
    return result;
}

int isvariablechar(char a)
{
    if (isalnum(a) || a == '_')
        return 1;
    return 0;
}

int match_separated(char *line, const char *word)
{
    int start_pos;
    int line_length = strlen(line);
    int word_length = strlen(word);

    if (word_length > line_length)
        return NOMATCH;
    for (start_pos = 0; start_pos <= line_length - word_length; start_pos++)
        if (strcmp_n(line+start_pos, word, word_length) && (start_pos == 0 || !isvariablechar(line[start_pos-1])) && (line_length - start_pos == word_length || !isvariablechar(line[start_pos + word_length])))
                return start_pos;
    
    return NOMATCH;
}

void clean_strings(char *line)
{
    int i, len = strlen(line);
    int qopen = 0;

    for (i = 0; i < len; i++) {
        if (line[i] == '"')
            qopen = 1 - qopen;
        else if (qopen == 1) {
            line[i] = ' ';
        }
    }
}

void clean_simple_comments(char *line)
{
    int i, len = strlen(line);
    int copen = 0;

    for (i = 0; i < len - 1; i++) {
        if (copen) {
            line[i] = ' ';
            continue;
        }
        if (line[i] == '/' && line [i+1] == '/')
            copen = 1;
    }
}

int clean_multiline_comments(char *line, int comment_opened)
{
    int i, len = strlen(line);
    int copen = comment_opened;

    for (i = 0; i < len - 1; i++) {
        if (line[i] == '*' && line[i+1] == '/') {
            copen = 0;
            i++;
        } else if (copen)
            line[i] = ' ';
        else if (line[i] == '/' && line[i+1] == '*') {
            copen = 1;
            i++;
        }
    }
    return copen;
}

int check_loops(FILE *source_file)
{
    char line[10000];
    int for_pos, while_pos, goto_pos;
    int comment_opened = 0;
    int loops = 0;

    while (fgets(line, sizeof(line), source_file))
    {
        comment_opened = clean_multiline_comments(line, comment_opened);
        clean_simple_comments(line);
        clean_strings(line);
        for_pos = match_separated(line, "for");
        while_pos = match_separated(line, "while");
        goto_pos = match_separated(line, "goto");
        if (for_pos != -1 || while_pos != -1 || goto_pos != -1)
            loops = 1;
    }
    return loops;
}

// <0-255> normal, - 1 white, -2 eof
int getChar(FILE *f, bool ignWhite) {
    bool white;
    int ch;
    do {
        if ((ch = getc(f))==EOF)
            return -2;
        if (ch==' ' || ch=='\n' || ch=='\t' || ch=='\r')
            white=true;
        else white=false;
    } while (ignWhite && white);
    if (white)
        return -1;
    return ch;
}

int main(void) 
{
    spoj_init();
    int ch1 = getChar(spoj_t_out, true);
    int ch2 = getChar(spoj_p_out, true);
    if (check_loops(spoj_t_src)) {
        fprintf(spoj_p_info, "Forbidden loop construction\n");
        return SPOJ_RV_NEGATIVE;
    }
    while (ch1 == ch2) {
        if (ch1==-2)
            exit(SPOJ_RV_POSITIVE);
        bool ignWhite;
        if (ch1==-1)
            ignWhite =true;
        else
            ignWhite=false;
        ch1 = getChar(spoj_t_out, ignWhite);
        ch2 = getChar(spoj_p_out, ignWhite);
    };
    if (ch1 == -2 && ch2 == -1 && getChar(spoj_p_out, true)==-2 )
        return SPOJ_RV_POSITIVE;
    if (ch2 == -2 && ch1 == -1 && getChar(spoj_t_out, true)==-2)
        return SPOJ_RV_POSITIVE;
    return SPOJ_RV_NEGATIVE;
}
