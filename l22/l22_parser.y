%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  double                d;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

  l22::block_node      *block;
  std::vector<std::shared_ptr<cdk::basic_type>> *types;
};

%token tAND tOR tNE tLE tGE tEQ tNOT tSIZEOF
%token tINPUT tWRITE tWRITELN
%token tPUBLIC tUSE tFOREIGN tPRIVATE
%token tTYPE_VAR tTYPE_INT tTYPE_DOUBLE tTYPE_TEXT tTYPE_VOID
%token tRETTYPE
%token tIF tTHEN tELIF tELSE
%token tWHILE tDO tBEGIN tEND
%token tSTOP tAGAIN tRETURN
%token tIFX

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tTEXT
%token <expression> tNULL

%type <node>     declaration  argdec      instruction  no_semi_colon_instr elif program
%type <sequence> declarations argdecs     instructions expressions file
%type <sequence> opt_decs     opt_argdecs opt_instrs   opt_args args
%type <expression> expression fundef integer double initializer
%type <lvalue> lvalue
%type <block> block
%type <type>  data_type  function_type  basic_type
%type <types> data_types opt_data_types
%type <s> text

%nonassoc tIF
%nonassoc tTHEN
%nonassoc tELIF tELSE

%right '='
%left tOR
%left tAND
%nonassoc tNOT
%left tNE tEQ
%left '<' tLE tGE '>'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY '?'

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file      : opt_decs           { compiler->ast($$ = $1); }
          | opt_decs program   { compiler->ast($$ = new cdk::sequence_node(LINE, $2, $1)); } 
          ;

program	: tBEGIN block ';' tEND { $$ = new l22::program_node(LINE, $2); }
	      ;

opt_decs : /* empty */   { $$ = new cdk::sequence_node(LINE); }
         | declarations  { $$ = $1; }
         ;

declarations   :              declaration  { $$ = new cdk::sequence_node(LINE, $1);     }
               | declarations declaration  { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

declaration    : tUSE     data_type     tIDENTIFIER                 ';'  { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, nullptr); }
               | tUSE     data_type     tIDENTIFIER                      { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, nullptr); }
               | tPUBLIC  data_type     tIDENTIFIER                 ';'  { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, nullptr); }
               | tPUBLIC  data_type     tIDENTIFIER                      { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, nullptr); }
               | tPUBLIC  data_type     tIDENTIFIER '=' initializer ';'  { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, $5); }
               | tPUBLIC  data_type     tIDENTIFIER '=' initializer      { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, $2, *$3, $5); }
               | tPUBLIC  tTYPE_VAR     tIDENTIFIER '=' initializer ';'  { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, nullptr, *$3, $5); }
               | tPUBLIC  tTYPE_VAR     tIDENTIFIER '=' initializer      { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, nullptr, *$3, $5); }
               | tPUBLIC                tIDENTIFIER '=' initializer ';'  { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, nullptr, *$2, $4); }
               | tPUBLIC                tIDENTIFIER '=' initializer      { $$ = new l22::variable_declaration_node(LINE, tPUBLIC, nullptr, *$2, $4); }
               |          tTYPE_VAR     tIDENTIFIER '=' initializer ';'  { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, nullptr, *$2, $4); }
               |          tTYPE_VAR     tIDENTIFIER '=' initializer      { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, nullptr, *$2, $4); }
               |          data_type     tIDENTIFIER                 ';'  { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); }
               |          data_type     tIDENTIFIER                      { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); }
               |          data_type     tIDENTIFIER '=' initializer ';'  { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $4); }
               |          data_type     tIDENTIFIER '=' initializer      { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $4); }
               | tFOREIGN function_type tIDENTIFIER                 ';'  { $$ = new l22::variable_declaration_node(LINE, tFOREIGN, $2, *$3, nullptr); }
               | tFOREIGN function_type tIDENTIFIER                      { $$ = new l22::variable_declaration_node(LINE, tFOREIGN, $2, *$3, nullptr); }
               ;

initializer    : expression         { $$ = $1; }
               | fundef             { $$ = $1; }
               ;

opt_data_types : /* empty */  { $$ = new std::vector<std::shared_ptr<cdk::basic_type>>(); }
               | data_types   { $$ = $1; }
               ;

data_types   :                data_type  { $$ = new std::vector<std::shared_ptr<cdk::basic_type>>(); $$->push_back($1); }
             | data_types ',' data_type  { $$ = $1; $$->push_back($3); }
             ;

data_type    : basic_type                { $$ = $1; }
             | function_type             { $$ = $1; }
             ;

basic_type   : tTYPE_TEXT                { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
             | tTYPE_INT                 { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
             | tTYPE_DOUBLE              { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
             | tTYPE_VOID                { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);   }
             | '[' basic_type ']'        { $$ = cdk::reference_type::create(4, $2); }
             ;

function_type  : data_type '<' opt_data_types '>'  { $$ = cdk::functional_type::create(*$3, $1); delete $3; }
               ;

fundef   : '(' opt_argdecs ')' tRETTYPE data_type ':' block ';'  { $$ = new l22::function_definition_node(LINE, $5, $2, $7); }
         ;

opt_argdecs : /* empty */   { $$ = new cdk::sequence_node(LINE);  }
            | argdecs       { $$ = $1; }

argdecs  :             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
         | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
         ;

argdec   : data_type tIDENTIFIER { $$ = new l22::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); }
         ;

block    : '{' opt_decs opt_instrs '}'   { $$ = new l22::block_node(LINE, $2, $3); }
         ;

opt_instrs   : /* empty */    { $$ = new cdk::sequence_node(LINE); }
             | instructions   { $$ = $1; }
             ;

instructions    :              instruction ';'     { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction ';'     { $$ = new cdk::sequence_node(LINE, $2, $1); }
                |              no_semi_colon_instr { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions no_semi_colon_instr { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

instruction  : tWHILE '(' expression ')' tDO block          { $$ = new l22::while_node(LINE, $3, $6); }
             | expression                                   { $$ = new l22::evaluation_node(LINE, $1);  }
             | tWRITE expressions                           { $$ = new l22::write_node(LINE, $2, false); }
             | tWRITELN expressions                         { $$ = new l22::write_node(LINE, $2, true); }
             | tAGAIN                                       { $$ = new l22::again_node(LINE); }
             | tSTOP                                        { $$ = new l22::stop_node(LINE); }
             | tRETURN                                      { $$ = new l22::return_node(LINE); }
             | tRETURN expression                           { $$ = new l22::return_node(LINE, $2); }
             | tRETURN fundef                               { $$ = new l22::return_node(LINE, $2); }
             | block                                        { $$ = $1; }
             ;

no_semi_colon_instr : tIF '(' expression ')' tTHEN block ';'       { $$ = new l22::if_node(LINE, $3, $6); }
                    | tIF '(' expression ')' tTHEN block ';' elif  { $$ = new l22::if_else_node(LINE, $3, $6, $8); }
                    | lvalue '=' fundef                            { $$ = new l22::evaluation_node(LINE, new cdk::assignment_node(LINE, $1, $3)); }
                    | lvalue '=' expression                        { $$ = new l22::evaluation_node(LINE, new cdk::assignment_node(LINE, $1, $3)); }
                    ;

elif         : tELSE block ';'                                { $$ = $2; }
             | tELIF '(' expression ')' tTHEN block ';'       { $$ = new l22::if_node(LINE, $3, $6); }
             | tELIF '(' expression ')' tTHEN block ';' elif  { $$ = new l22::if_else_node(LINE, $3, $6, $8); }
             ;

lvalue       : tIDENTIFIER                                             { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
             | lvalue                              '[' expression ']'  { $$ = new l22::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
             | '(' expression ')'                  '[' expression ']'  { $$ = new l22::index_node(LINE, $2, $5); }
             | tIDENTIFIER        '(' opt_args ')' '[' expression ']'  { $$ = new l22::index_node(LINE, new l22::function_call_node(LINE, new cdk::rvalue_node(LINE, new cdk::variable_node(LINE, *$1)), $3), $6); delete $1; }
             | '(' fundef ')'     '(' opt_args ')' '[' expression ']'  { $$ = new l22::index_node(LINE, new l22::function_call_node(LINE, $2, $5), $8); }
             |      '@'           '(' opt_args ')' '[' expression ']'  { $$ = new l22::index_node(LINE, new l22::function_call_node(LINE, nullptr, $3), $6); }
             ;

opt_args     : /* empty */   { $$ = new cdk::sequence_node(LINE); }
             | args          { $$ = $1; }
             ;

args         :          expression         { $$ = new cdk::sequence_node(LINE, $1); }
             |          fundef             { $$ = new cdk::sequence_node(LINE, $1); }
             | args ',' expression         { $$ = new cdk::sequence_node(LINE, $3, $1); }
             | args ',' fundef             { $$ = new cdk::sequence_node(LINE, $3, $1); }
             ;

expressions  :                 expression  { $$ = new cdk::sequence_node(LINE, $1); }
             | expressions ',' expression  { $$ = new cdk::sequence_node(LINE, $3, $1); }
             ;

expression   : integer                          { $$ = $1; }
             | double                           { $$ = $1; }
             | text                             { $$ = new cdk::string_node(LINE, $1); }
             | tNULL                            { $$ = new l22::null_node(LINE); }
             /* LEFT VALUES */
             | lvalue                           { $$ = new cdk::rvalue_node(LINE, $1); }
             /* ASSIGNMENTS */
             | lvalue '=' expression            { $$ = new cdk::assignment_node(LINE, $1, $3); }
             | lvalue '=' fundef                { $$ = new cdk::assignment_node(LINE, $1, $3); }
             /* ARITHMETIC EXPRESSIONS */
             | expression '+' expression        { $$ = new cdk::add_node(LINE, $1, $3); }
             | expression '-' expression        { $$ = new cdk::sub_node(LINE, $1, $3); }
             | expression '*' expression        { $$ = new cdk::mul_node(LINE, $1, $3); }
             | expression '/' expression        { $$ = new cdk::div_node(LINE, $1, $3); }
             | expression '%' expression        { $$ = new cdk::mod_node(LINE, $1, $3); }
             /* LOGICAL EXPRESSIONS */
             | expression '<'  expression       { $$ = new cdk::lt_node(LINE, $1, $3);  }
             | expression tLE  expression       { $$ = new cdk::le_node(LINE, $1, $3);  }
             | expression tEQ  expression       { $$ = new cdk::eq_node(LINE, $1, $3);  }
             | expression tGE  expression       { $$ = new cdk::ge_node(LINE, $1, $3);  }
             | expression '>'  expression       { $$ = new cdk::gt_node(LINE, $1, $3);  }
             | expression tNE  expression       { $$ = new cdk::ne_node(LINE, $1, $3);  }
             | expression tAND expression       { $$ = new cdk::and_node(LINE, $1, $3); }
             | expression tOR  expression       { $$ = new cdk::or_node(LINE, $1, $3);  }
             /* UNARY EXPRESSIONS */
             | '-'  expression %prec tUNARY     { $$ = new cdk::neg_node(LINE, $2); }
             | '+'  expression %prec tUNARY     { $$ = new l22::identity_node(LINE, $2); }
             | tNOT expression                  { $$ = new cdk::not_node(LINE, $2); }
             /* OTHER EXPRESSIONS */
             | tINPUT                           { $$ = new l22::input_node(LINE); }
             | tIDENTIFIER    '(' opt_args ')'  { $$ = new l22::function_call_node(LINE, new cdk::rvalue_node(LINE, new cdk::variable_node(LINE, *$1)), $3); delete $1;}
             | '(' fundef ')' '(' opt_args ')'  { $$ = new l22::function_call_node(LINE, $2, $5); }
             |      '@'       '(' opt_args ')'  { $$ = new l22::function_call_node(LINE, nullptr, $3); }
             | tSIZEOF '(' expression ')'       { $$ = new l22::sizeof_node(LINE, $3); }
             | '(' expression ')'               { $$ = $2; }
             | '[' expression ']'               { $$ = new l22::stack_alloc_node(LINE, $2); }
             | lvalue '?'                       { $$ = new l22::address_of_node(LINE, $1); }
             ;

integer      : tINTEGER                     { $$ = new cdk::integer_node(LINE, $1); }
             ;

double       : tDOUBLE                      { $$ = new cdk::double_node(LINE, $1); }
             ;

text         : tTEXT                        { $$ = $1; }
             | text tTEXT                   { $$ = $1; $$->append(*$2); delete $2; }
             ;

%%
