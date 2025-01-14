#ifndef __L22_TARGETS_POSTFIX_TARGET_H__
#define __L22_TARGETS_POSTFIX_TARGET_H__

#include <cdk/targets/basic_target.h>
#include <cdk/ast/basic_node.h>
#include "targets/postfix_writer.h"
#include <stack>
#include <cdk/types/functional_type.h>

#include <cdk/emitters/postfix_ix86_emitter.h>

namespace l22 {

  class postfix_target: public cdk::basic_target {
    static postfix_target _self;

  private:
    postfix_target() :
        cdk::basic_target("asm") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // during code generation
      cdk::symbol_table<l22::symbol> symtab;
      std::stack<std::shared_ptr<cdk::functional_type>> funcs;

      // this is the backend postfix machine
      cdk::postfix_ix86_emitter pf(compiler);

      // generate assembly code from the syntax tree
      postfix_writer writer(compiler, symtab, funcs, pf);
      compiler->ast()->accept(&writer, 0);

      return true;
    }

  };

} // l22

#endif
