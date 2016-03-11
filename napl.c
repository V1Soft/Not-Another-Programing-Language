#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else // _WIN32
#include <readline/readline.h>
#include <readline/history.h>
#endif

int main(int argc, char *argv[])
{
  /* Create Parsers */
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Product = mpc_new("product");
  mpc_parser_t* Value = mpc_new("value");
  mpc_parser_t* Maths = mpc_new("maths");

  /* Define them */
  mpca_lang(MPCA_LANG_DEFAULT,
            " expression : <product> (('+' | '-') <product>)*; "
            " product    : <value>   (('*' | '/')   <value>)*; "
            " value      : /[0-9]+/ | '(' <expression> ')';    "
            " maths      : /^/ <expression> /$/;               "
            ,
            Expression, Product, Value, Maths, NULL);
  
  puts("Napl. V0.0.0.1");
  puts("Press Ctrl-C to exit");
  while (1) {
    char* input = readline("Napl> ");
    add_history(input);
    /* Where the parsing(magic) happens. */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Maths, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    }
    else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    /* End magic */
    free(input);
  }
  mpc_cleanup(4, Expression, Product, Value, Maths);
  
  return 0;
}
