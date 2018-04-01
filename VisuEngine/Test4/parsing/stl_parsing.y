%{
#include <stdio.h>
#include "../cls_stl_file.h"
extern int yylex (); // declared in LEX file
extern void yyerror (cls_stl_file*& myStlFile, char*); // declared in LEX file
%}

%union {
   int ival;
   char* sval;
   cls_stl_file* stlfileval;
}

%parse-param {cls_stl_file*& myStlFile}

%token<ival> VERTEX NORMAL OUTER LOOP ENDLOOP FACET ENDFACET SOLID ENDSOLID

%token<sval> INTEGER
%token<sval> REAL
%token<sval> IDENTIFICATOR

%type<stlfileval> exchange_file

%type<sval> value

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
                      $$ = f;
                  }
              ;

solidname     : IDENTIFICATOR
              ;

facet_list    : facet_list facet
              | facet
              ;

facet         : FACET normal outer_loop ENDFACET
              ;

normal        : NORMAL value value value
              ;

outer_loop    : OUTER LOOP vertex vertex vertex ENDLOOP
              ;

vertex        : VERTEX value value value
                  {
                      //printf("x=%s\ty=%s\tz=%s", $2, $3, $4);
                  }
              ;

value         : INTEGER
                  {
                      $$ = $1;
                  }
              | REAL
                  {
                      $$ = $1;
                  }
              ;
