#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "l8t9.h"

SCOPE_NODE* currentScope;

int main(void)
{
  // set up the random number generator with the time as a seed
  srand(time(NULL));
  yyparse();
  return 0;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
}

SYMBOL_TYPE getType(char *name)
{
  if (currentScope)
  {
    // make a new node to iterate through the table
    SCOPE_NODE* currScope = currentScope;
    SYMBOL_AST_NODE* currentSymbol = currScope->symbols;

    // start with current scope and work up through parents
    while(currScope)
    {
      while (currentSymbol)
      {
        //check to see if this is the symbol were looking for
        if (!strcmp(currentSymbol->name, name) && currentSymbol->type != INVALID) {
          return currentSymbol->type;
        }
        // otherwise keep on iterating
        currentSymbol = currentSymbol->next;
      }
      // move to the next scope and keep looking
      currScope = currScope->parent;
      if (currScope)
        currentSymbol = currScope->symbols;
      }
  }
  return INVALID;
}

SYMBOL_AST_NODE* getSymbol(char *name)
{
  SYMBOL_AST_NODE *result = NULL;
  int found = 0;

  // only check if there is an existing scope
  if (currentScope)
  {
    // make a new node to iterate through the table
    SCOPE_NODE* currScope = currentScope;
    SYMBOL_AST_NODE* currentSymbol = currScope->symbols;

    // start with current scope and work up through parents
    while(currScope)
    {
      while (currentSymbol)
      {
        //check to see if this is the symbol were looking for
        if (!strcmp(currentSymbol->name, name)) {
          result = currentSymbol;
          found = 1;
          break;
        }
        // otherwise keep on iterating
        currentSymbol = currentSymbol->next;
      }
      if (found) break;
      // move to the next scope and keep looking
      currScope = currScope->parent;
      if (currScope)
        currentSymbol = currScope->symbols;
      }
  }
  // throw error if its not found
  else if (!found)
    printf("ERROR: Undeclared variable <%s> used\n", name);

  return result;
}

void leaveScope()
{
  if(currentScope)
    currentScope = currentScope->parent;
}

void enterScope(SCOPE_NODE* newScope)
{
  newScope->parent = currentScope;
  currentScope = newScope;
}

// find out which function it is
int resolveFunc(char *func)
{
  char *funcs[] = { "neg", "abs", "exp", "sqrt", "exp2", "cbrt", "hypot", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "print", "equal", "smaller", "larger", "rand", "read" ""};

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

int resolveType(char* type)
{
  char *types[] = { "integer", "real" };

  int i = 0;
  while (types[i][0] != '\0')
  {
    if (!strcmp(types[i], type))
      return i;
    i++;
  }
  return -1;
}

// create a node for let
AST_NODE *let(SYMBOL_AST_NODE *symbols, AST_NODE *s_expr)
{
  AST_NODE *p;
  size_t nodeSize;

  // create a new scope node
  SCOPE_NODE *localScope = malloc(sizeof(SCOPE_NODE));
  localScope->symbols = symbols;

  // allocate space for the fixed size and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(LET_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  // put scope and s_expr into the let node and return it
  p->type = LET_TYPE;
  p->data.let.scope = localScope;
  p->data.let.s_expr = s_expr;

  return p;
}

// add the new symbol to the list and return it
SYMBOL_AST_NODE* let_list(SYMBOL_AST_NODE *symbol, SYMBOL_AST_NODE *let_list)
{
  // insert the new symbol into the let_list
  SYMBOL_AST_NODE* current = let_list;
  int found = 0;

  // check for duplicates
  while (current)
  {
    if (!strcmp(symbol->name, current->name))
    {
      printf("ERROR: redeclaration of variable <%s> attempted\n", symbol->name);
      current->value = symbol->value;
      found = 1;
      symbol = let_list;
      break;
    }
    // keep looking through the list
    current = current->next;
  }

  // otherwise add it to the front of the list
  if (!found)
    symbol->next = let_list;

  // return the list with the new symbol
  return symbol;
}

// create a new symbol and return it
SYMBOL_AST_NODE *let_elem(char* type, char* symbol, AST_NODE *s_expr)
{
  SYMBOL_AST_NODE *p;
  size_t nodeSize;

  // allocate space the symbol
  nodeSize = sizeof(SYMBOL_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  if (type)
    p->type = resolveType(type);
  else
    p->type = INVALID;
  p->name = symbol;
  p->value = s_expr;
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
  p->data.number.type = REAL;
  p->data.number.value = value;
  return p;
}

// create a node for a function
AST_NODE *condition(AST_NODE* condition, AST_NODE* ifTrue, AST_NODE* ifFalse)
{
  AST_NODE *p;
  size_t nodeSize;

  // allocate space for the fixed sie and the variable part (union)
  nodeSize = sizeof(AST_NODE) + sizeof(COND_AST_NODE);
  if ((p = malloc(nodeSize)) == NULL)
    yyerror("out of memory");

  p->type = COND_TYPE;
  p->data.condition.condition = condition;
  p->data.condition.true_expr = ifTrue;
  p->data.condition.false_expr = ifFalse;

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

NUMBER_AST_NODE* eval(AST_NODE *p)
{
  NUMBER_AST_NODE* result = malloc(sizeof(NUMBER_AST_NODE));
  NUMBER_AST_NODE* op1 = malloc(sizeof(NUMBER_AST_NODE));
  NUMBER_AST_NODE* op2 = malloc(sizeof(NUMBER_AST_NODE));

  if (!p)
    return result;

  else if (p->type == NUM_TYPE)
  {
    result = &p->data.number;
    result->value = result->type == INTEGER ? round(result->value) : result->value;
  }

  else if (p->type == FUNC_TYPE)
  {
    int funcType = resolveFunc(p->data.function.name);
    switch (funcType)
    {
      //unary instructions
      case NEG:
        result = eval(p->data.function.op1);
        result->value = -result->value;
        break;
      case ABS:
        result = eval(p->data.function.op1);
        result->value = fabs(result->value);
        break;
      case EXP:
        result = eval(p->data.function.op1);
        result->value = exp(result->value);
        break;
      case SQRT:
        result = eval(p->data.function.op1);
        result->value = sqrt(result->value);
        break;
      case EXP2:
        result = eval(p->data.function.op1);
        result->value = exp2(result->value);
        break;
      case CBRT:
        result = eval(p->data.function.op1);
        result->value = cbrt(result->value);
        break;

      //arithmetic operations
      case ADD:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);

        // set the new type and value
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = op1->value + op2->value;
        break;
      case SUB:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        if ((op1->type==INTEGER) && (op2->type==INTEGER)) {
          result->type = INTEGER;
          result->value = round(op1->value - op2->value);
        }
        else {
          result->type = REAL;
          result->value = op1->value - op2->value;
        }
        break;
      case MULT:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);

        // set the new type and value
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = op1->value * op2->value;
        break;
      case DIV:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);

        if ((op1->type==INTEGER) && (op2->type==INTEGER)) {
          result->type = INTEGER;
          result->value = round(op1->value / op2->value);
        }
        else {
          result->type = REAL;
          result->value = op1->value / op2->value;
        }
        break;

      // other binary operations
      case MOD:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = remainder(op1->value, op2->value);
        break;
      case LOG:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);

        if (op1->value == 2.0)
          result->value = log2(op2->value);
        else if (op1->value == 10.0)
          result->value = log10(op2->value);
        else {
          yyerror("invalid log base");
          result->value = 0.0;
        }

        if ((op1->type==INTEGER) && (op2->type==INTEGER)) {
          result->type = INTEGER;
          result->value = round(result->value);
        }
        else result->type = REAL;
        break;
      case POW:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);

        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = pow(op1->value, op2->value);
        result->value = result->type == INTEGER ? round(result->value) : result->value;
        break;
      case MAX:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = fmax(op1->value, op2->value);
        break;
      case MIN:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = fmin(op1->value, op2->value);
        break;
      case HYPOT:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = ((op1->type==INTEGER) && (op2->type==INTEGER)) ? INTEGER : REAL;
        result->value = hypot(op1->value, op1->value);
        break;
      case PRINT:
        result = eval(p->data.function.op1);
        if (result->type == INTEGER)
          printf("%d", (int)result->value);
        else printf("%.2lf", result->value);
        break;
      case EQUAL:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = INTEGER;
        result->value = op1->value == op2->value ? 1 : 0;
        break;
      case SMALLER:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = INTEGER;
        result->value = op1->value < op2->value ? 1 : 0;
        break;
      case LARGER:
        op1 = eval(p->data.function.op1);
        op2 = eval(p->data.function.op2);
        result->type = INTEGER;
        result->value = op1->value > op2->value ? 1 : 0;
        break;
      case READ:
        result->type = REAL;
        scanf("\n%lf", &result->value);
        //throw out the trailing newline
        getc(stdin);
        break;
      case RAND:
        result->value = (double)rand();
        result->type = REAL;
        break;
    }
  }
  // just evaluate the right side of each let
  else if (p->type == LET_TYPE)
  {
    //enter the new scope, evaluate, then leave
    enterScope(p->data.let.scope);
    result = eval(p->data.let.s_expr);
    leaveScope();
  }

  else if (p->type == SYM)
  {
    p->data.symbol = *getSymbol(p->data.symbol.name);
    // if there was no type declared
    // check with previously declared values
    if (p->data.symbol.type == INVALID)
      p->data.symbol.type = getType(p->data.symbol.name);
    // if theres still no type, error
    if (p->data.symbol.type == INVALID) {
      printf("ERROR: undeclared variable <%s> used\n", p->data.symbol.name);
      exit(1);
    }
    else {
      op1 = eval(p->data.symbol.value);
      if (p->data.symbol.type == INTEGER) {
        result->type = INTEGER;
        if (fmod(op1->value, 1) != 0)
          printf("WARNING: incompatible type assignment for variable <%s>\n", p->data.symbol.name);
        result->value = round(op1->value);
      }
      else {
        result->type = REAL;
        result->value = op1->value;
      }
    }
  }
  else if (p->type == COND_TYPE)
  {
    result = eval(p->data.condition.condition)->value ? eval(p->data.condition.true_expr) : eval(p->data.condition.false_expr);
  }

  return result;
}
