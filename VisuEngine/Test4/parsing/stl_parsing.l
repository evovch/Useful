%{
#include "../cls_stl_file.h"
#include "stl_parsing.tab.h"  // include header generated by bison from the YACC file
void yyerror (cls_stl_file*& myStlFile, char*); // declaration
int yyparse (cls_stl_file*& myStlFile); // declaration
// yylex is declared for us by LEX
// yywrap is declared for us by LEX

#ifdef _WIN32
	#define STRDUP(...) _strdup(__VA_ARGS__)
#else
	#define STRDUP(...) strdup(__VA_ARGS__)
#endif

%}

DIGIT                   [0-9]
LOWER                   [a-zа-я]
UPPER                   [A-ZА-Я]|"_"
CHARACTER               {DIGIT}|{LOWER}|{UPPER}

SIGN                    "+"|"-"

%%

"\r\n"                  ;

"vertex"                return VERTEX;
"normal"                return NORMAL;
"outer"                 return OUTER;
"loop"                  return LOOP;
"endloop"               return ENDLOOP;
"facet"                 return FACET;
"endfacet"              return ENDFACET;
"solid"                 return SOLID;
"endsolid"              return ENDSOLID;

{SIGN}?{DIGIT}+                                    { yylval.sval = STRDUP(yytext); return INTEGER; }
{SIGN}?{DIGIT}+"."{DIGIT}*("E"{SIGN}?{DIGIT}+)?    { yylval.sval = STRDUP(yytext); return REAL; }
{CHARACTER}+                                       { yylval.sval = STRDUP(yytext); return IDENTIFICATOR; }

%%

// implementation
void yyerror (cls_stl_file*& /*myStlFile*/, char* str)
{
	printf ("ERROR: Could not parse: %s\n", str);
}

int yywrap ()
{
	return 1;
}