%{
#include <stdio.h>
#include "cls_stl_file.h"
#include "stl_structs.h"
extern int yylex (); // declared in LEX file
extern void yyerror (cls_stl_file*& myStlFile, char*); // declared in LEX file
%}

%union {
   unsigned long int ival;
   char* sval;
   double dval;
   cls_stl_file* stlfileval;
   struct facet_t* facetval;
   struct normal_t* normalval;
   struct loop_t* loopval;
   struct vertex_t* vertexval;
}

%parse-param {cls_stl_file*& myStlFile}

%token<ival> VERTEX NORMAL OUTER LOOP ENDLOOP FACET ENDFACET SOLID ENDSOLID
%token<sval> INTEGER
%token<sval> REAL
%token<sval> IDENTIFICATOR

%type<stlfileval> exchange_file
%type<sval> solidname
%type<facetval> facet_list
%type<facetval> facet
%type<normalval> normal
%type<loopval> outer_loop
%type<vertexval> vertex
%type<dval> value

%start root_node

%%

root_node     : exchange_file
                  {
                      myStlFile = $1;
                  }
              ;

exchange_file : SOLID solidname facet_list ENDSOLID solidname
                  {
                      cls_stl_file* f = new cls_stl_file();
                      f->SetName($2);
                      f->SetFirstFacet($3);
                      $$ = f;
                  }
              ;

solidname     : IDENTIFICATOR
                  {
                      $$ = $1;
                  }
              ;

facet_list    : facet_list facet
                  {
                      $2->SetNext($1);
                      $$ = $2;
                  }
              | facet
                  {
                      $$ = $1;
                  }
              ;

facet         : FACET normal outer_loop ENDFACET
                  {
                      struct facet_t* f = new struct facet_t($2, $3);
                      $$ = f;
                  }
              ;

normal        : NORMAL value value value
                  {
                      struct normal_t* v = new struct normal_t($2, $3, $4);
                      $$ = v;
                      /*printf("normal x=%f\ty=%f\tz=%f", $2, $3, $4);*/
                  }
              ;

outer_loop    : OUTER LOOP vertex vertex vertex ENDLOOP
                  {
                      struct loop_t* l = new struct loop_t($3, $4, $5);
                      $$ = l;
                      /*printf("facet x=%f\ty=%f\tz=%f\n      x=%f\ty=%f\tz=%f\n      x=%f\ty=%f\tz=%f\n",
                          $3->mX, $3->mY, $3->mZ, $4->mX, $4->mY, $4->mZ, $5->mX, $5->mY, $5->mZ);*/
                  }
              ;

vertex        : VERTEX value value value
                  {
                      struct vertex_t* v = new struct vertex_t($2, $3, $4);
                      $$ = v;
                      /*printf("vertex x=%f\ty=%f\tz=%f", $2, $3, $4);*/
                  }
              ;

value         : INTEGER
                  {
                      $$ = atof($1); // return double
                  }
              | REAL
                  {
                      $$ = atof($1); // return double
                  }
              ;
