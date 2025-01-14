#ifndef __L22_TARGETS_TYPE_CHECKER_H__
#define __L22_TARGETS_TYPE_CHECKER_H__

#include "targets/basic_ast_visitor.h"
#include <cdk/types/structured_type.h>
#include <cdk/types/functional_type.h>
#include <stack>

namespace l22 {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class type_checker: public basic_ast_visitor {
    cdk::symbol_table<l22::symbol> &_symtab;
    std::stack<std::shared_ptr<cdk::functional_type>> &_funcs;
    basic_ast_visitor *_parent;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<l22::symbol> &symtab,
                  std::stack<std::shared_ptr<cdk::functional_type>> &funcs, basic_ast_visitor *parent) :
        basic_ast_visitor(compiler), _symtab(symtab), _funcs(funcs), _parent(parent) {
    }

  public:
    ~type_checker() {
      os().flush();
    }

  protected:
    void do_PIDExpression(cdk::binary_operation_node *const node, int lvl);
    void do_IDExpression(cdk::binary_operation_node *const node, int lvl);
    void do_IntOnlyExpression(cdk::binary_operation_node *const node, int lvl);
    void do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    void do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    void do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl);
    int check_pointers_types(std::shared_ptr<cdk::basic_type> const left, std::shared_ptr<cdk::basic_type> const right);
    bool check_compatible_functions(std::shared_ptr<cdk::functional_type> left, std::shared_ptr<cdk::functional_type> right);
    bool check_functions_output(std::shared_ptr<cdk::basic_type> left, std::shared_ptr<cdk::basic_type> right);
    bool check_functions_args(std::shared_ptr<cdk::structured_type> left, std::shared_ptr<cdk::structured_type> right);
    template<typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {
    }

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  };

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, funcs, node) { \
  try { \
    l22::type_checker checker(compiler, symtab, funcs, this); \
    (node)->accept(&checker, 0); \
  } \
  catch (const std::string &problem) { \
    std::cerr << (node)->lineno() << ": " << problem << std::endl; \
    return; \
  } \
}

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, _funcs, node)

} // l22

#endif
