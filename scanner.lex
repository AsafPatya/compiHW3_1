%{
#include "types.hpp"
"hw3_output.hpp"

#define YYSTYPE NodePointer
#include "parser.tab.hpp"
#include "scanner_utils.hpp"
"types.hpp"
"hw3_output.hpp"

%}

%option yylineno
%option noyywrap
digit   		                              ([0-9])
letter  		                              ([a-zA-Z])
no_new_line                                   [\x00-\x09\x0b-\x0c\x0e-\x7f]
whitespace                                    ([\t\n\r ])
comment                                       (\/\/{no_new_line}*)

%%

void                                          { return tokenToNode(yytext, VOID); }
int                                           { return tokenToNode(yytext, INT); }
byte                                          { return tokenToNode(yytext, BYTE); }
b                                             { return tokenToNode(yytext, B); }
bool                                          { return tokenToNode(yytext, BOOL); }
auto                                          { return tokenToNode(yytext, AUTO); }
and                                           { return tokenToNode(yytext, AND); }
or                                            { return tokenToNode(yytext, OR); }
not                                           { return tokenToNode(yytext, NOT); }
true                                          { return tokenToNode(yytext, TRUE); }
false                                         { return tokenToNode(yytext, FALSE); }
return                                        { return tokenToNode(yytext, RETURN); }
if                                            { return tokenToNode(yytext, IF); }
else                                          { return tokenToNode(yytext, ELSE); }
while                                         { return tokenToNode(yytext, WHILE); }
break                                         { return tokenToNode(yytext, BREAK); }
continue                                      { return tokenToNode(yytext, CONTINUE); }
;                                             { return tokenToNode(yytext, SC); }
,                                             { return tokenToNode(yytext, COMMA); }
(\()                                          { return tokenToNode(yytext, LPAREN); }
(\))                                          { return tokenToNode(yytext, RPAREN); }
(\{)                                          { return tokenToNode(yytext, LBRACE); }
(\})                                          { return tokenToNode(yytext, RBRACE); }
=                                             { return tokenToNode(yytext, ASSIGN); }
!=|==                                         { return tokenToNode(yytext, EQUALITY); }
([<>]=?)                                      { return tokenToNode(yytext, RELATIONAL); }
(\+|\-)                                       { return tokenToNode(yytext, ADDITIVE); }
(\*|\/)                                       { return tokenToNode(yytext, MULTIPLICATIVE); }
{letter}({letter}|{digit})*                   { return tokenToNode(yytext, ID); }
[1-9]{digit}*|0                               { return tokenToNode(yytext, NUM); }
\"([^\n\r\"\\]|\\[rnt"\\])+\"                 { return tokenToNode(yytext, STRING); }
{whitespace}|{comment}                        ;
.                                             { errorLex(yylineno); };
%%
