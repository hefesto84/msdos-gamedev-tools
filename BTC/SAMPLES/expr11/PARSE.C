#include <expr.h>

char vars[3]={'x', 'y', 'z'};
double varvals[3]={1.0, 1.0, 1.0};

main(int argc, char *argv[])
{
   Expr *expr, *dx, *dy, *dz;
   char *unparse;
   char string[128];
   double val;

   if (argc<2) {
      printf("Usage: parse <f(x,y,z)>\n");
      exit(-1);
   }
   if ((expr=ParseExpr(argv[1], 3, vars)) == NULL) {
      printf("Error parsing: %s\n", Expr_errstr);
      exit(-1);
   }
   unparse=UnParseExpr(expr);
   printf("parsed equation: %s\n",unparse);
   free(unparse);

   SimpExpr(expr);
   unparse=UnParseExpr(expr);
   printf("simplified: %s\n",unparse);
   free(unparse);

   dx=DiffExpr(expr, 0);
   SimpExpr(dx);
   unparse=UnParseExpr(dx);
   printf("dx: %s\n",unparse);
   free(unparse);

   dy=DiffExpr(expr, 1);
   SimpExpr(dy);
   unparse=UnParseExpr(dy);
   printf("dy: %s\n",unparse);
   free(unparse);

   dz=DiffExpr(expr, 2);
   SimpExpr(dz);
   unparse=UnParseExpr(dz);
   printf("dz: %s\n",unparse);
   free(unparse);

   val=EvalExpr(expr, varvals);
   printf("f(%.6g,%.6g,%.6g)=%.6g\n",varvals[0],varvals[1],varvals[2],val);
   
   FreeExpr(expr);
   FreeExpr(dx);
   FreeExpr(dy);
   FreeExpr(dz);
}
