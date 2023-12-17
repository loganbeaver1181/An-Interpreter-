#ifndef PARSER_H
   #define PARSER_H
#define ERROR -999999 // value to represent an error


extern char * line; // the current line from the input file

extern char curr_cat[OPSIZE]; //current token category

extern FILE* out_file; // the file to write to
/*
 * Purpose: Function Prototypes for parser.c
 * Date:    April 21, 2023
 */
int bexpr(char *);	// bexpr is short for boolean_expression
int expr(char *);     // expr is short for expression
int term(char *);
int ttail(char *, int);       // ttail is short for term_tail
int stmt(char *);
int stail(char *, int);      // stail is short for statement_tail
int factor(char *);
int ftail(char *, int);	// ftail is short for factor_tail
int expp(char *);     // expp is short for exponentiation

void add_sub_tok(char *);
void mul_div_tok(char *);
void compare_tok(char *);
void expon_tok(char *); // helper function
int num(char *);


#endif
