%option c++ prefix="l22_scanner_" outfile="l22_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <vector>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "l22_parser.tab.h"

static bool restart_functions = false, block_end = false, last_instr = false;

static int lLevelsToPop = 0, lIndex = 0;
static std::vector<int> indents;

// output stream for building string literals
static std::ostringstream strlit;

inline bool valid(int indent) {
    lLevelsToPop = lIndex = -1;
    bool indentOk = false;
    for (size_t ix = 0; ix < indents.size(); ix++) {
        if (indent == indents[ix]) {
            lLevelsToPop = indents.size() - ix - 1;
            lIndex = ix;
            indentOk = true;
        }
    }
    return indentOk;
}

// don't change this
#define yyerror LexerError
%}
%x X_TEXT X_COMMENT X_BACKSLASH X_NEWLINE X_IGNORE
%%
  {
    yydebug=1; set_debug(1);
    if (last_instr) {
      last_instr = false;
      block_end = true;
      return '}';
    }
    if (block_end) {
      block_end = false;
      return ';';
    }
    if (restart_functions) {
      indents.pop_back();
      if (indents.size() == 0) restart_functions = false;
      last_instr = true;
      return ';';
    }
    else {
      if (lLevelsToPop > 0) {
        lLevelsToPop--;
        indents.pop_back();
        last_instr = true;
        return ';';
      }
    }
  }

  /* ====================================================================== */
  /* ====[                        Comentários                         ]==== */
  /* ====================================================================== */

[ ]*"..."\n            ; /* ignore continuation marks */

";".*$                 /* match the whole line (it is ignored) */

"(*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"(*"        yy_push_state(X_COMMENT);
<X_COMMENT>"*)"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */

  /* ====================================================================== */
  /* ====[                         Indentação                         ]==== */
  /* ====================================================================== */

\n                   yy_push_state(X_NEWLINE); /* delay decision */

<X_NEWLINE>^[ ]+  {
    yy_pop_state();

    if (valid(yyleng)) {
          if (lLevelsToPop == 0) {
            // at the same level: ignore index
            // do not return tINDENT;
            return ';';
          }
          else {
            // nothing is done here: the scanner will ignore the input while levels > 0
            // and send block-end tokens to the parser
            // at least one has to be returned here...
            if (lLevelsToPop > 0) {
              lLevelsToPop--;
              indents.pop_back();
              last_instr = true;
              return ';';
            }
          }
    }
    else if (indents.size() == 0) {
          // first block to open
          indents.push_back(yyleng);
          return '{';
    }
    else if (yyleng > indents.back()) {
          // this is like the one before, but not the first
          indents.push_back(yyleng);
          return '{';
    }
    else {
          // something wrong: bad indent: communicate to user
          std::cerr << "Bad indent at line " << yylineno;
    }

}
<X_NEWLINE>\n      ; // ignore successive newline chars
<X_NEWLINE>.       {
  // in this case, we have a new function starting
  // first, return whatever we are seeing to the input
  yyless(0);
  yy_pop_state();
  if (indents.size() > 0) {
    indents.pop_back();
    if (indents.size() > 0) { restart_functions = true; }
    last_instr = true;
    return ';';
  }
  else return ';'; // top-level declarations
}

  /* ====================================================================== */
  /* ====[                      Tipos de dados                        ]==== */
  /* ====[                      Null pointers                         ]==== */
  /* ====================================================================== */

"var"                  return tTYPE_VAR;
"int"                  return tTYPE_INT;
"double"               return tTYPE_DOUBLE;
"text"                 return tTYPE_TEXT;
"void"                 return tTYPE_VOID;

"null"                 return tNULL;
"sizeof"               return tSIZEOF;

  /* ====================================================================== */
  /* ====[                      Símbolos globais                      ]==== */
  /* ====================================================================== */

"public"               return tPUBLIC;
"use"                  return tUSE;
"foreign"              return tFOREIGN;

  /* ====================================================================== */
  /* ====[                    Instrução condicional                   ]==== */
  /* ====================================================================== */

"if"                   return tIF;
"then:"                return tTHEN;
"elif"                 return tELIF;
"else:"                return tELSE;

  /* ====================================================================== */
  /* ====[                    Instrução de iteração                   ]==== */
  /* ====[                    Instrução de terminação                 ]==== */
  /* ====[                    Instrução de continuação                ]==== */
  /* ====================================================================== */

"while"                return tWHILE;
"do:"                  return tDO;
"stop"                 return tSTOP;
"again"                return tAGAIN;

  /* ====================================================================== */
  /* ====[                   Operadores de expressões                 ]==== */
  /* ====================================================================== */

[\[\]()]               return *yytext;
[-+*/%<>@?=]           return *yytext;
"=="                   return tEQ;
"!="                   return tNE;
"<="                   return tLE;
">="                   return tGE;
"not"                  return tNOT;
"and"                  return tAND;
"or"                   return tOR;
"input"                return tINPUT;

  /* ====================================================================== */
  /* ====[                 Delimitadores e separadores                ]==== */
  /* ====[                 Corpo (de função)                          ]==== */
  /* ====[                 Instrução de retorno                       ]==== */
  /* ====================================================================== */

[,:]                   return *yytext;
"->"                   return tRETTYPE;
"return"               return tRETURN;
"write"                return tWRITE; 
"writeln"              return tWRITELN;
"begin"                return tBEGIN; 
"end"                  return tEND; 

  /* ====================================================================== */
  /* ====[                       Inteiros                             ]==== */
  /* ====================================================================== */

0[0-6]+                { yylval.i = strtoul(yytext, NULL, 7);
                          if (errno == ERANGE) yyerror("Integer out of range (overflow)");
                          else return tINTEGER; }

0[0-9]+                yyerror("Invalid base 7 number");

[0-9]+                 { yylval.i = strtoul(yytext, NULL, 10);
                          if (errno == ERANGE) yyerror("Integer out of range (overflow)");
                          else return tINTEGER; }

  /* ====================================================================== */
  /* ====[                       Reais                                ]==== */
  /* ====================================================================== */

([0-9]*\.[0-9]+|[0-9]+\.?[0-9]*)([eE][-+]?[0-9]+)? { yylval.d = strtod(yytext, NULL);
                                                      if (errno == ERANGE) yyerror("Double out of range (overflow)");
                                                      else return tDOUBLE; }

  /* ====================================================================== */
  /* ====[                   Identificadores (nomes)                  ]==== */
  /* ====================================================================== */

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

  /* ====================================================================== */
  /* ====[                  Cadeias de caracteres                     ]==== */
  /* ====================================================================== */

"\""                           yy_push_state(X_TEXT);
<X_TEXT>\\                     yy_push_state(X_BACKSLASH);
<X_TEXT>"\""                   {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 return tTEXT;
                               }
<X_TEXT>.                      strlit << *yytext;

<X_BACKSLASH>t                 strlit << '\t'; yy_pop_state();
<X_BACKSLASH>n                 strlit << '\n'; yy_pop_state();
<X_BACKSLASH>r                 strlit << '\r'; yy_pop_state();
<X_BACKSLASH>"\""              strlit << '"'; yy_pop_state();
<X_BACKSLASH>\\                strlit << '\\'; yy_pop_state();

<X_BACKSLASH>[0-6]{1,3}        {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 7);
                                 yy_pop_state();
                               }

<X_BACKSLASH>0[^0-6]           {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 yy_pop_state();
                                 yy_push_state(X_IGNORE);
                                 return tTEXT;
                               }

<X_BACKSLASH>.                 strlit << *yytext; yy_pop_state();

<X_IGNORE>"\""                 yy_pop_state();
<X_IGNORE>.                    ;

  /* ====================================================================== */
  /* ====[                    (outros caracteres)                     ]==== */
  /* ====================================================================== */

<<EOF>>                {
  if (indents.size() > 0) {
    indents.pop_back();
    last_instr = true;
    return ';';
  }
  return 0;  // nothing left to do: tell parser to proceed
}

[ ]                 /* ignore remaining white space inside the program */
.                      yyerror(yytext);    /* all other chars: error! */

%%
