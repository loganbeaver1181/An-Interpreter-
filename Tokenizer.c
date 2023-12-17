/**
 * tokenizer.c - A simple token recognizer.
 *
 * NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
 *       program.
 *
 * @version 03/22/2023
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./tokenizer.h"
#include "./parser.h"

// Global pointer to line of input
char *line;

// index of where we are in the line
int line_index;

// The current category
char curr_cat[OPSIZE];

// the file to write to from parser.c
extern FILE* out_file;



/**
 * @brief Function for retrieving individual tokens.
 * Internally calls classify_token to immediately identify each token before the next iteration
 * 
 * @param token a pointer to where the current token will be stored
 */
int get_token(char *token) {
    static int lex_error = 0;
    // printf("getting token\n");
    //skip all whitespace
    while(line[line_index] == ' ' ||
    line[line_index] == '\t' ||
    line[line_index] == '\n' ||
    line[line_index] == '\r'){
        line_index++;
    }

    if (line[line_index] == '\0') {
        return 0;
    }

    classify_token(token);

    //clear white space after a token as well
    while(line[line_index] == ' ' ||
    line[line_index] == '\t' ||
    line[line_index] == '\n' ||
    line[line_index] == '\r'){
        line_index++;
    }

    if(strcmp(curr_cat, "NOT_A_TOKEN") == 0 && lex_error != 1) {
        lex_error = 1;
        char* error;
        while(lex_error) {
            error = get_lex_error(token, &lex_error);
        }
        fprintf(out_file, "===> '%s'\nLexical Error: not a lexeme\n", error);
        return 0;
    }
    return 1;
}


/**
 * @brief Recieves a token pointer and identifies the token
 * Sets the global variable curr_cat to the appropriate name
 * depending on the category of the current token
 * 
 * @param token a pointer to where the current token will be stored
 */
void classify_token(char* token) {
    memset(token, 0, TSIZE);
    memset(curr_cat, 0, OPSIZE);

    //Plus op case
    if (line[line_index] == '+'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "ADD_OP");
    }
        //Minus op case
    else if (line[line_index] == '-'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "SUB_OP");
    }
        //Multiply op case
    else if (line[line_index] == '*'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "MULT_OP");
    }
        //Division op case
    else if (line[line_index] == '/'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "DIV_OPP");
    }
        //Left paren case
    else if (line[line_index] == '('){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "LEFT_PAREN");
    }
        //Right paren case
    else if (line[line_index] == ')'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "RIGHT_PAREN");
    }
        //Exponent op case
    else if (line[line_index] == '^'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "EXPON_OP");
    }
        //2 cases
    else if (line[line_index] == '<'){
        //Less then or equal case
        if(line[line_index + 1] == '='){
            two_token(token, line[line_index], line[line_index + 1]);
            strcpy(curr_cat, "LESS_THEN_OR_EQUAL_OP");
        }
            //Less than case
        else{
            single_token(token, line[line_index]);
            strcpy(curr_cat, "LESS_THEN_OP");
        }
    }
        //2 cases
    else if (line[line_index] == '>'){
        //Greater then or equal case
        if(line[line_index + 1] == '='){
            two_token(token, line[line_index], line[line_index + 1]);
            strcpy(curr_cat, "GREATER_THEN_OR_EQUAL_OP");
        }
            //Greater then case.
        else{
            single_token(token, line[line_index]);
            strcpy(curr_cat, "GREATER_THEN_OP");
        }
    }
        //2 cases
    else if (line[line_index] == '='){
        //Equals op case.
        if(line[line_index + 1] == '='){
            two_token(token, line[line_index], line[line_index + 1]);
            strcpy(curr_cat, "EQUALS_OP");
        }
            //Assignment op case.
        else{
            single_token(token, line[line_index]);
            strcpy(curr_cat, "NOT_A_TOKEN");
        }
    }
        //2 cases
    else if (line[line_index] == '!'){
        //not equals case
        if(line[line_index + 1] == '='){
            two_token(token, line[line_index], line[line_index + 1]);
            strcpy(curr_cat, "NOT_EQUALS_OP");
        }
        //not case
        else{
            single_token(token, line[line_index]);
            strcpy(curr_cat, "NOT_A_TOKEN");
        }
    }
        //Semi Colon Case
    else if (line[line_index] == ';'){
        single_token(token, line[line_index]);
        strcpy(curr_cat, "SEMI_COLON");
    }
        //Int literal case.
    else if (is_int(line[line_index]) == 1){
        make_int(token);//Construct the int
        strcpy(curr_cat, "INT_LITERAL");
    }
        //Not a token case.
    else{
        single_token(token, line[line_index]);
        strcpy(curr_cat, "NOT_A_TOKEN");
    }
}

/**
 * @brief 'Creates' a token of length 1, adding a null terminator to
 * the end of the string
 * 
 * @param token a pointer to where the current token will be stored
 * @param token_char the character to be made into a token
 */
void single_token(char *token, char token_char) {
    token[0] = token_char;
    token[1] = '\0';

    line_index++;
}

/**
 * @brief 'Creates' a token of length 2, adding a null terminator to
 * the end of the string
 * 
 * @param token a pointer to where the current token will be stored
 * @param token_char the first character to be made into a token
 * @param token_char2 the second character to be made into a token
 */
void two_token(char token[TSIZE], char token_char1, char token_char2) {
    token[0] = token_char1;//Assign first character in the token.
    token[1] = token_char2;//Assign second character in the token.
    token[2] = '\0';

    line_index += 2;//Increment the line index by 2, the length of the token.
}

/**
 * @brief Checks if a character could possibly be an int literal
 * 
 * @param pos_int the character to be checked
 */
int is_int(char pos_int) {
    int flag = 0;

    if (pos_int == '0' ||
        pos_int == '1' ||
        pos_int == '2' ||
        pos_int == '3' ||
        pos_int == '4' ||
        pos_int == '5' ||
        pos_int == '6' ||
        pos_int == '7' ||
        pos_int == '8' ||
        pos_int == '9'){

        //set flag to 1 (true) if the token is an int
        flag = 1;
    }

    return flag;
}

/**
 * @brief Creates an int literal as a token, accounting
 * for an int containing any number of digits
 * 
 * @param token_ptr a pointer to where the current token will be stored
 */
void make_int(char token_ptr[TSIZE]) {

    //Assign the int character to
    //the first element of token.
    token_ptr[0] = line[line_index];

    //Used to keep track of how long the int token is.
    int int_count = 1;

    //While the next character in line is an in,
    //add it to the current token.
    while((is_int(line[line_index + int_count]) == 1) && int_count <= TSIZE){
        token_ptr[int_count] = line[line_index + int_count];
        int_count++;
    }
    //Increment line_index by however long the int token is.
    line_index += int_count;

    token_ptr[int_count + 1] = '\0';
}

/**
 * This function handles lexical errors. It checks if the token
 * is a valid lexeme. If not, it will check if any proceeding characters are also invalid lexemes.
 * @param token the current token
 * @return error the invalid lexemes or "OK".
 */
char * get_lex_error(char * token, int* error_indicator) {
    char * error = malloc(OPSIZE * sizeof(char)); //array to store lexemes

    //Check if the current token is invalid
    if (strcmp(curr_cat, "NOT_A_TOKEN") == 0) {
        error[0] = token[0];
        get_token(token);

        //Check if proceeding lexemes are valid.
        construct_lex_error(token, error, error_indicator);
    }
        //If the lexeme is valid, set error to "OK"
    else {
        strcpy(error, "OK");
    }
    return error;
}

/**
 * This function is passed an invalid lexeme and checks proceeding
 * lexemes to see if they are also invalid.
 * @param token the current token
 * @param error the string that will hold the invalid lexemes.
 * @param error_indicator the int that represents a lexical error
 */
void construct_lex_error(char * token, char * error, int* error_indicator) {
    //Create an int to hold the length of the error array.
    int error_length = 1;
    //While the next token is invalid
    while (strcmp(curr_cat, "NOT_A_TOKEN") == 0) {
        //Add it to error
        error[error_length] = token[0];
        //Increment the length of error
        error_length++;
        //And get the next token
        get_token(token);
    }

    *error_indicator = 0;
}



