#ifndef __L22_AST_FUNCTION_DEFINITION_H__
#define __L22_AST_FUNCTION_DEFINITION_H__

#include <string>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>
#include "ast/block_node.h"

namespace l22 {

  class function_definition_node: public cdk::expression_node {
    cdk::sequence_node *_arguments;
    l22::block_node *_block;

  public:
    inline function_definition_node(int lineno, cdk::sequence_node *arguments,
                             l22::block_node *block) :
        cdk::expression_node(lineno), _arguments(arguments), _block(block) {

      auto input = new std::vector<std::shared_ptr<cdk::basic_type>>();

      for (size_t i = 0; i < arguments->size(); i++) {
        cdk::typed_node* n = dynamic_cast<cdk::typed_node*>(arguments->node(i));
        input->push_back(n->type());
      }

      type(cdk::functional_type::create(*input, cdk::primitive_type::create(0, cdk::TYPE_VOID)));
    }

    inline function_definition_node(int lineno, std::shared_ptr<cdk::basic_type> funType,
                             cdk::sequence_node *arguments, l22::block_node *block) :
        cdk::expression_node(lineno), _arguments(arguments), _block(block) {

      auto input = new std::vector<std::shared_ptr<cdk::basic_type>>();

      for (size_t i = 0; i < arguments->size(); i++) {
        cdk::typed_node* n = dynamic_cast<cdk::typed_node*>(arguments->node(i));
        input->push_back(n->type());
      }

      type(cdk::functional_type::create(*input, funType));
    }

  public:
    inline cdk::sequence_node* arguments() {
      return _arguments;
    }
    inline cdk::typed_node* argument(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_arguments->node(ax));
    }
    inline l22::block_node* block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // l22

#endif