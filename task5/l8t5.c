#include <math.h>
#include <string.h>
#include "l8t5.h"

SYMBOL_TABLE_NODE *symbolTable;

int main(void)
{
  yyparse();
  return 0;
}

void insertSymbol(char* name, AST_NODE* s_expr)
{
  // create a new symbol table node & fill it with the given data
  SYMBOL_TABLE_NODE* newHead = malloc(sizeof(SYMBOL_TABLE_NODE));
  newHead->name = name;
  newHead->s_expr = s_expr;

  //make it point to the current head and then make symbolTable point to newHead
  newHead->next = symbolTable;
  symbolTable = newHead;
}

double getSymbolValue(char *name)
{
  // make a new node to iterate through the table
  SYMBOL_TABLE_NODE* current = symbolTable;
  double result = 0.0;
  int found = 0;

  // iterate while current is not NULL
  while (current)
  {
    //check to see if this is the symbol were looking for
    if (!strcmp(current->name, name)) {
      // get its value, if it is
      result = eval(current->s_expr);
      found = 1;
      break;
    }
    // otherwise keep on iterating
    current = current->next;
  }
  // throw error if its not found
  if (!found)
    yyerror("invalid symbol used");

  return result;
}

void removeSymbol(char* name)
{
  // make a new node to iterate through the table
  SYMBOL_TABLE_NODE* current = symbolTable;

  if (current && !strcmp(current->name, name))
    symbolTable = current->next;

  // iterate while current is not NULL
  while (current)
  {
    //check to see if this is the symbol were looking for
    if (current->next && !strcmp(current->next->name, name)) {
      // then delete it if it is
      current->next = current->next->next;
      break;
    }
    // otherwise keep on iterating
    current = current->next;
  }
}

void leaveScope(AST_NODE* root)
{
  // only do stuff if this node is not null
  if(root) {
    // if its a let_elem node, remove the symbol
    if (root->type == LET_ELEM)
      removeSymbol(root->data.let_elem.symbol);
    // otherwise keep traversing
    else {
      leaveScope(root->data.let_list.let_elem);
      leaveScope(root->data.let_list.let_list);
    }
  }
}

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

// find out which function it is
int resolveFunc(char *func)
{
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "exp2", "cbrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "let", ""};

  int i = 0;
  while (funcs[i][0] !='\0')
  {
    if (!strcmp(funcs[i], func))
      return i;

    i++;
  }
  yyerror("invalid function"); // paranoic -- should never happen
  return -1;
}

// create a node for let
AST_NODE *let(AST_NODE *let_list, AST_NODE *s_expr)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(LET_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = LET_TYPE;
  p->data.let.let_list = let_list;
  p->data.let.s_expr = s_expr;
  return p;
}

// create a node for let_list
AST_NODE *let_list(AST_NODE *let_elem, AST_NODE *let_list)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed size and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(LET_LIST_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = LET_LIST;
  p->data.let_list.let_elem = let_elem;
  p->data.let_list.let_list = let_list;

  return p;
}

// create a node for let_elem
AST_NODE *let_elem(char* symbol, AST_NODE *s_expr)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed size and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(LET_ELEM_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = LET_ELEM;
  p->data.let_elem.symbol = symbol;
  insertSymbol(symbol, s_expr);
  return p;
}

// create a symbol node
AST_NODE *symbol(char* name)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(SYMBOL_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = SYM;
  p->data.symbol.name = name;
  return p;
}

// create a node for a number
AST_NODE *number(double value)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = NUM_TYPE;
  p->data.number.value = value;
  return p;
}

// create a node for a function
AST_NODE *function(char *funcName, AST_NODE *op1, AST_NODE *op2)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = FUNC_TYPE;
  p->data.function.name = funcName;
  p->data.function.op1 = op1;
  p->data.function.op2 = op2;

  return p;
}

// free a node
void freeNode(AST_NODE *p)
{
  if (!p)
    return;

  if (p->type == FUNC_TYPE)
  {
    free(p->data.function.name);
    freeNode(p->data.function.op1);
    freeNode(p->data.function.op2);
  }

  free (p);
}

double eval(AST_NODE *p)
{
  double result = 0.0;
  if (!p)
    return result;

  else if (p->type == NUM_TYPE)
    result = p->data.number.value;

  else if (p->type == FUNC_TYPE)
  {
    int funcType = resolveFunc(p->data.function.name);
    switch (funcType)
    {
      //unary instructions
      case NEG:
        result = -eval(p->data.function.op1); break;
      case ABS:
        result = fabs(eval(p->data.function.op1)); break;
      case EXP:
        result = exp(eval(p->data.function.op1)); break;
      case SQRT:
        result = sqrt(eval(p->data.function.op1)); break;
      case EXP2:
        result = exp2(eval(p->data.function.op1)); break;
      case CBRT:
        result = cbrt(eval(p->data.function.op1)); break;

      //arithmetic operations
      case ADD:
        result = eval(p->data.function.op1) + eval(p->data.function.op2); break;
      case SUB:
        result = eval(p->data.function.op1) - eval(p->data.function.op2); break;
      case MULT:
        result = eval(p->data.function.op1) * eval(p->data.function.op2); break;
      case DIV:
        result = eval(p->data.function.op1) / eval(p->data.function.op2); break;

      // other binary operations
      case MOD:
        result = remainder(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case LOG:
        if (eval(p->data.function.op1) == 2.0)
          result = log2(eval(p->data.function.op2));
        else if (eval(p->data.function.op1) == 10.0)
          result = log10(eval(p->data.function.op2));
        else
          yyerror("invalid log base");
      break;
      case POW:
        result = pow(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case MAX:
        result = fmax(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case MIN:
        result = fmin(eval(p->data.function.op1), eval(p->data.function.op2)); break;
      case HYPOT:
        result = hypot(eval(p->data.function.op1), eval(p->data.function.op2)); break;
    }
  }
  // just evaluate the right side of each let
  else if (p->type == LET_TYPE)
  {
    result = eval(p->data.let.s_expr);
    printf("%lf\n", result);
    leaveScope(p->data.let.let_list);
  }

  else if (p->type == SYM)
    result = getSymbolValue(p->data.symbol.name);

  return result;
}
