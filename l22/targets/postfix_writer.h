#ifndef __L22_TARGETS_POSTFIX_WRITER_H__
#define __L22_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <set>
#include <stack>
#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>
#include <cdk/types/functional_type.h>

namespace l22 {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    cdk::symbol_table<l22::symbol> &_symtab;
    std::stack<std::shared_ptr<cdk::functional_type>> &_funcs; // for keeping track of the current function and its arguments

    std::set<std::string> _functions_to_declare;

    bool _inFunctionArgs;
    int _inFunctionBody, _inFunctionCall;
    bool _returnSeen; // when building a function
    std::stack<int> _whileIni, _whileEnd; // for stop/again
    int _offset; // current framepointer offset (0 means no vars defined)

    cdk::basic_postfix_emitter &_pf;
    int _lbl, _lastFuncLbl;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<l22::symbol> &symtab,
                    std::stack<std::shared_ptr<cdk::functional_type>> &funcs, cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _symtab(symtab), _funcs(funcs), _inFunctionArgs(false),
        _inFunctionBody(0), _inFunctionCall(0), _returnSeen(false), _offset(0), _pf(pf), _lbl(0), _lastFuncLbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // l22

#endif
