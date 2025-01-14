#ifndef __L22_TARGETS_XML_TARGET_H__
#define __L22_TARGETS_XML_TARGET_H__

#include <cdk/targets/basic_target.h>
#include <cdk/ast/basic_node.h>
#include "targets/xml_writer.h"
#include <stack>
#include <cdk/types/functional_type.h>

namespace l22 {

  class xml_target: public cdk::basic_target {
    static xml_target _self;

  private:
    xml_target() :
        cdk::basic_target("xml") {
    }

  public:
    bool evaluate(std::shared_ptr<cdk::compiler> compiler) {
      // this symbol table will be used to check identifiers
      // an exception will be thrown if identifiers are used before declaration
      cdk::symbol_table<l22::symbol> symtab;
      std::stack<std::shared_ptr<cdk::functional_type>> funcs;

      xml_writer writer(compiler, symtab, funcs);
      compiler->ast()->accept(&writer, 0);
      return true;
    }

  };

} // l22

#endif
