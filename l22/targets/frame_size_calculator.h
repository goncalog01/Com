#ifndef __L22_TARGET_FRAME_SIZE_CALCULATOR_H__
#define __L22_TARGET_FRAME_SIZE_CALCULATOR_H__

#include "targets/basic_ast_visitor.h"

#include <sstream>
#include <stack>
#include <cdk/types/functional_type.h>

namespace l22 {

  class frame_size_calculator: public basic_ast_visitor {
    cdk::symbol_table<l22::symbol> &_symtab;
    std::stack<std::shared_ptr<cdk::functional_type>> &_funcs;

    size_t _localsize;

  public:
    frame_size_calculator(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<l22::symbol> &symtab, std::stack<std::shared_ptr<cdk::functional_type>> &funcs) :
        basic_ast_visitor(compiler), _symtab(symtab), _funcs(funcs), _localsize(0) {
    }

  public:
    ~frame_size_calculator();

  public:
    size_t localsize() const {
      return _localsize;
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
