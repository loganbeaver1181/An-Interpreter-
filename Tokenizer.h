/**
 * tokenizer.h - Header file for tokenizer.c.
 *
 * @version 4/21/2023
 */

/* Constants */
#define LINE 100
#define TSIZE 20
#define OPSIZE 35
#define TRUE 1
#define FALSE 0
#define LOOP 1

/* operators */
#define ADD_OP +
#define SUB_OP -
#define MULTI_OP *
#define DIV_OP /
#define LEFT_PAREN (
#define RIGHT_PAREN )
#define EXPON_OP ^
#define ASSIGN_OP =
#define LESS_THEN_OP <
#define LES_THEN_OR_EQUAL_OP <=
#define GREATER_THEN_OP >
#define GREATER_THEN_OR_EQUAL_OP >=
#define EQUALS_OP ==
#define NOP_OP !
#define NOT_EQUALS_OP !=
#define SEMI_COLON

/* libraries*/
#include <ctype.h>
#include <stdio.h>

/*function headers*/
int get_token(char *token);
void classify_token(char *token);
void single_token(char *token, char token_char);
void two_token(char token_ptr[TSIZE], char token_char1,char token_char2);
int is_int(char pos_int);
void make_int(char token_ptr[TSIZE]);
void print_to_file(FILE *output, char *token, char *category);
void construct_lex_error(char * token, char * error, int* error_indicator);
char * get_lex_error(char * token, int* error_indicator);
