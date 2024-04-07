#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <alloc.h>

typedef struct Factor {
   char operator;
   char toktyp;
   double value;
   struct Term *Expr;
   struct Factor *Arg;
   struct Factor *Power;
   struct Factor *next;
} Factor;

typedef struct Term {
   char operator;
   struct Factor *FactorList;
   struct Term *next;
} Term;

typedef struct Expr {
   char *vars;
   int numvars;
   struct Term *TermList;
} Expr;

extern Expr *ParseExpr(char *, int, char *);
extern Expr *DiffExpr(Expr *, char);
extern Expr *CopyExpr(Expr *);
extern void SimpExpr(Expr *);

extern double EvalExpr(Expr *, double *);
extern void FreeExpr(Expr *);

extern Expr *AddExpr(Expr *, Expr *);
extern Expr *SubExpr(Expr *, Expr *);
extern Expr *DivExpr(Expr *, Expr *);
extern Expr *MultExpr(Expr *, Expr *);

extern char *UnParseExpr(Expr *);

extern char Expr_errstr[];
