/**
 *  interpreter.c main controller for a recursive decent parser in parser.c
 *  Date: April 21, 2023
 */
#include "tokenizer.h"

#include "parser.h"

#include "interpreter.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

#include <math.h>

#define ERROR -999999


extern char * line;
int line_index;
extern char curr_cat[OPSIZE];
extern char lex_error[OPSIZE];
extern int syntax_error;
FILE * out_file = NULL;


/**
 * The main function for the program
 * @param argc the argument count
 * @param argv the argument list
 * @return 0 if successful
 */
int main(int argc, char
    const * argv[]) {
    char token[TSIZE]; /* Spot to hold a token, fixed size */
    char input_line[LINE]; /* Line of input, fixed size        */
    FILE * in_file = NULL; /* File pointer                     */

    if (argc != 3) {
        printf("Usage: interpreter inputFile outputFile\n");
        exit(1);
    }

    in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
        exit(1);
    }

    out_file = fopen(argv[2], "w");
    if (out_file == NULL) {
        fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
        exit(1);
    }


    while (fgets(input_line, LINE, in_file) != NULL) {
        // Sets a global pointer to the memory location
        // where the input line resides.
        line = input_line;
        line_index = 0;

        fprintf(out_file, "%s", line);

        //goes all the way to the end of the line
        while (line[line_index] != '\0' &&
            line[line_index] != '\n') {
            int result;

            get_token(token);

            result = bexpr(token);
            if (result != ERROR) {
            fprintf(out_file, "Syntax OK\nValue is %d\n", result);
            } else {
                while(get_token(token));
                if(syntax_error && strcmp(token, ";") != 0) {
                    fprintf(out_file, "===> '%s' expected\nSyntax Error\n", lex_error);
                }
                break;
            }

        }

    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}
