/*
 * parser.c - recursive descent parser for a simple expression language.
 * Most of the functions in this file model a non-terminal in the
 * grammar listed below
 * Date:   Modified 9-29-08 and 3-25-15 and 14 april 2020
 */
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

#include <math.h>

#include "tokenizer.h"

#include "parser.h"


char lex_error[OPSIZE]; //global variable to store lexical errors.
int syntax_error = 0; // value to represent a syntax error


/*
 * <bexpr> ::= <expr> ;
 * <expr> ::=  <term> <ttail>
 * <ttail> ::=  <add_sub_tok> <term> <ttail> | e
 * <term> ::=  <stmt> <stail>
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * <stmt> ::=  <factor> <ftail>
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * <expp> ::=  ( <expr> ) | <num>
 * <add_sub_tok> ::=  + | -
 * <mul_div_tok> ::=  * | /
 * <compare_tok> ::=  < | > | <= | >= | != | ==
 * <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 */

/**
 * <expr> -> <term> <ttail>
 * The function for the non-terminal <expr> that views
 * the expression as a series of terms and addition and
 * subtraction operators.
 * @param token: the current lexeme
 * @return: the number of the evaluated expression or an error
 */
int expr(char * token) {
   int exprReturn;
   int subtotal = term(token);
   if (subtotal == ERROR) {

      return subtotal;
   } else {
      exprReturn = ttail(token, subtotal);
      return exprReturn;
   }
}

/**
 * The function for the non terminal <term> that views
 * the term  as a series of statements and statement tails.
 * @param token the current lexeme
 * @return the number of the evaluated term
 */
int term(char * token) {
   int termReturn;
   int statement = stmt(token);
   if (statement == ERROR)
      return statement;
   else
      termReturn = stail(token, statement);
   return termReturn;
}

/**
 * The function for the non terminal <stmt> that views
 * the statement as a series factors and factor tails.
 * @param token the current lexeme
 * @return the number of the evaluated statement
 */
int stmt(char * token) {
   int stmtReturn;
   int fac = factor(token);
   if (fac == ERROR)
      return fac;
   else
      stmtReturn = ftail(token, fac);
   return stmtReturn;
}

/**
 * The function for the non terminal <bexpr> that starts the evaluation of the expression
 * @param token the current lexeme
 * @return the evaluation of the expression
 */
int bexpr(char * token) {
   syntax_error = 0;
   //Will hold the return value.
   int to_return;


   //Pass the token down to expr
   to_return = expr(token);

   //Make sure there is a semicolon
   if (strcmp(token, ";") != 0) {
      strcpy(lex_error, ";");
      syntax_error = 1;
   }
   return to_return;
}

/**
 * <ttail> -> <add_sub_tok> <term> <ttail> | e
 * The function for the non-terminal <ttail> that is the
 * the rest of an arithmetic expression after the initial
 * term. So it expects an addition or subtraction operator
 * first or the empty string. 
 * @param token: the line being read
 * @param subtotal: the number we have evaluated up to this
 *                  point
 * @return: the number of the evaluated expression or an error
 */
int ttail(char * token, int subtotal) {
   int term_value;

   //checks if the current token is the add operator.
   if (!strncmp(token, "+", 1)) {
      add_sub_tok(token);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         term_value = ttail(token, (subtotal + term_value));
      return term_value;
   } else if (!strncmp(token, "-", 1)) {
      add_sub_tok(token);
      term_value = term(token);

      // if term returned an error, give up otherwise call ttail
      if (term_value == ERROR)
         return term_value;
      else
         return ttail(token, (subtotal - term_value));
   }
   /* empty string */
   else
      return subtotal;
}



/**
 * Move the pointer along to the next lexeme
 * @param token the current lexeme
 */
void add_sub_tok(char * token) {
   get_token(token);

}

/**
 * Move the pointer along to the next lexeme
 * @param token the current lexeme
 */
void mult_div_tok(char * token) {
   get_token(token);

}

/**
 * Move the pointer along to the next lexeme
 * @param token the current lexeme
 */
void compare_tok(char * token) {
   get_token(token);

}

/**
 * <expp> ::=  ( <expr> ) | <num>
 * The function for the non terminal <expp> responsible for parenthesis and numbers.
 * @param token the current lexeme
 * @return the evaluated number
 */
int expp(char * token) {

   int to_return;

   if (strcmp(curr_cat, "LEFT_PAREN") == 0){
      if (!get_token(token)){
         return ERROR;
      }

      to_return = expr(token);
      if (strcmp(curr_cat, "RIGHT_PAREN") != 0) {
         //No closing parenthesis error
         strcpy(lex_error, ")");
         syntax_error = 1;
         fprintf(out_file, "===> ')' expected\nSyntax Error\n");
         return ERROR;
      }else{
         if (!get_token(token)){
            return ERROR;
         }
      }
   }
   //If there is no parenthesis, store the value of the
   //number in to_return.
   else {
      to_return = num(token);
   }
   return to_return;
}


/**
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * The function for the non terminal <factor> responsible for exponential expressions
 */
int factor(char * token) {
   int factor_num;
   factor_num = expp(token);

   if (strcmp(curr_cat, "EXPON_OP") == 0) {
      if (!get_token(token)) {
         return ERROR;
      }



      if (factor_num == ERROR) {
         return factor_num;
      } else {
         factor_num = pow(factor_num, factor(token));
      }
   }

   return factor_num;

}


/**
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * The function for the non terminal <ftail> responsible for comparison operators.
 * @param token the current lexeme
 * @return the evaluated expression from the comparison
 *
 */
int ftail(char * token, int subtotal) {

   int compare_value;
   //If the current token is the less than operator, get the next token,
   //pass it to factor and then recurse.
   if (strcmp(curr_cat, "LESS_THEN_OP") == 0) {
      compare_tok(token); // move token
      compare_value = factor(token);

      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal < compare_value);
      }
      //If the current token is the greater than operator, get the next token,
      //pass it to factor and then recurse.
   } else if (strcmp(curr_cat, "GREATER_THEN_OP") == 0) {
      compare_tok(token);
      compare_value = factor(token);
      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal > compare_value);
      }
       //If the current token is the less than or equal operator, get the next token,
       //pass it to factor and then recurse.
   } else if (strcmp(curr_cat, "LESS_THEN_OR_EQUAL_OP") == 0) {
      compare_tok(token);
      compare_value = factor(token);
      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal <= compare_value);
      }
       //If the current token is the greater than or equal operator, get the next token,
       //pass it to factor and then recurse.
   } else if (strcmp(curr_cat, "GREATER_THEN_OR_EQUAL_OP") == 0) {
      compare_tok(token);

      compare_value = factor(token);
      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal >= compare_value);
      }
       //If the current token is the equals' operator, get the next token,
       //pass it to factor and then recurse.
   } else if (strcmp(curr_cat, "EQUALS_OP") == 0) {
      compare_tok(token);
      compare_value = factor(token);
      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal == compare_value);
      }
       //If the current token is the not equals' operator, get the next token,
       //pass it to factor and then recurse.
   } else if (strcmp(curr_cat, "NOT_EQUALS_OP") == 0) {
      compare_tok(token);

      compare_value = factor(token);
      if (compare_value == ERROR) {
         return compare_value;
      } else {
         return ftail(token, subtotal != compare_value);
      }
   } else {
      return subtotal;
   }
}


/**
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * The function for the non terminal <stail> responsible for the multiplication and division operators
 * @param token the current lexeme
 * @return the evaluated expression from the operators
 */
int stail(char * token, int subtotal) {

   //Hold the value of this statement.
   int stmt_value;

   //If the current token is the mult opp, get the next token,
   //pass it to stmt and then recurse.
   if (strcmp(curr_cat, "MULT_OP") == 0) {
      mult_div_tok(token);
      stmt_value = stmt(token);

      if (stmt_value == ERROR) {
         return stmt_value;
      } else {
         return stail(token, (subtotal * stmt_value));
      }
   }
   //If the current token is the div opp, get the next token,
   //pass it to stmt and then recurse.
   else if (strcmp(curr_cat, "DIV_OPP") == 0) {
      mult_div_tok(token);
      stmt_value = stmt(token);

      if (stmt_value == ERROR) {
         return stmt_value;
      } else {
         return stail(token, (subtotal / stmt_value));
      }
   }
   //If the current token is not the mult or div op
   //Just return the subtotal.
   else {
      return subtotal;
   }

}


/**
 * function to convert one or more ascii characters to integers
 * @param token - the possible number
 * @return the integer value of the character
 */
int num(char * token) {
   int value;

   if (strcmp(curr_cat, "INT_LITERAL") == 0) {
      value = (atoi(token));
      if (!get_token(token)) {
         return ERROR;
      }
   } else {
      value = ERROR;
   }
   return value;
}



