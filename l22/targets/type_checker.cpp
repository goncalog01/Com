#include <string>
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void l22::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void l22::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void l22::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl);
}

//---------------------------------------------------------------------------

void l22::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void l22::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void l22::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

void l22::type_checker::do_null_node(l22::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, nullptr));
}

//---------------------------------------------------------------------------

void l22::type_checker::do_identity_node(l22::identity_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT) || node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(node->argument()->type());
  } else {
    throw std::string("integer or double expressions expected");
  }
}

void l22::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT) || node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(node->argument()->type());
  } else {
    throw std::string("integer or double expressions expected");
  }
}

void l22::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong type in unary logical expression");
  }
}

//---------------------------------------------------------------------------

void l22::type_checker::do_PIDExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(node->left()->type());
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_POINTER)) {
    node->type(node->right()->type());
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void l22::type_checker::do_IDExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else if (node->left()->is_typed(cdk::TYPE_UNSPEC) && node->right()->is_typed(cdk::TYPE_UNSPEC)) {
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->left()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    node->right()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  } else {
    throw std::string("wrong types in binary expression");
  }
}

void l22::type_checker::do_IntOnlyExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary operator (left)");
  }

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary operator (right)");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void l22::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  do_PIDExpression(node, lvl);
}

void l22::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  do_PIDExpression(node, lvl);
}

void l22::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  do_IDExpression(node, lvl);
}

void l22::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  do_IDExpression(node, lvl);
}

void l22::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  do_IntOnlyExpression(node, lvl);
}

//---------------------------------------------------------------------------

void l22::type_checker::do_ScalarLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!(node->left()->is_typed(cdk::TYPE_INT) || node->left()->is_typed(cdk::TYPE_DOUBLE))) {
    throw std::string("integer or double expression expected in binary logical expression (left)");
  }

  node->right()->accept(this, lvl + 2);
  if (!(node->right()->is_typed(cdk::TYPE_INT) || node->right()->is_typed(cdk::TYPE_DOUBLE))) {
    throw std::string("integer or double expression expected in binary logical expression (right)");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void l22::type_checker::do_GeneralLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!(node->left()->is_typed(cdk::TYPE_INT) || node->left()->is_typed(cdk::TYPE_DOUBLE) || node->left()->is_typed(cdk::TYPE_POINTER))) {
    throw std::string("integer, double or pointer expression expected in equality operator (left)");
  }

  node->right()->accept(this, lvl + 2);
  if (!(node->right()->is_typed(cdk::TYPE_INT) || node->right()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_POINTER))) {
    throw std::string("integer, double or pointer expression expected in equality operator (right)");
  }

  if (node->left()->type() != node->right()->type()) {
    throw std::string("same type expected on both sides of equality operator");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void l22::type_checker::do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void l22::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}

void l22::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}

void l22::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}

void l22::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  do_ScalarLogicalExpression(node, lvl);
}

void l22::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
}

void l22::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  do_GeneralLogicalExpression(node, lvl);
}

void l22::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

void l22::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------

void l22::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<l22::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void l22::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void l22::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl + 2);
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
  node->rvalue()->accept(this, lvl + 2);

  if (node->lvalue()->is_typed(cdk::TYPE_INT)) {

    if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {  // input
      node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
    } else {
      throw std::string("wrong assignment to integer");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {

    if (node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
      int type_check = check_pointers_types(node->lvalue()->type(), node->rvalue()->type());
      if (type_check == 1) {
        throw std::string("non matching pointer levels in assignment");
      } else if (type_check == 2) {
        node->rvalue()->type(node->lvalue()->type());
        node->type(node->lvalue()->type());
      } else if (type_check == 3) {
        throw std::string("non matching pointer types in assignment");
      } else {
        node->type(node->lvalue()->type());
      }
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_ERROR));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_ERROR));
    } else {
      throw std::string("wrong assignment to pointer");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {

    if (node->rvalue()->is_typed(cdk::TYPE_DOUBLE) || node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {  // input
      node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      node->rvalue()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
    } else {
      throw std::string("wrong assignment to double");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_STRING)) {

    if (node->rvalue()->is_typed(cdk::TYPE_STRING)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
      node->rvalue()->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
    } else {
      throw std::string("wrong assignment to string");
    }

  } else if (node->lvalue()->is_typed(cdk::TYPE_FUNCTIONAL)) {

    if (node->rvalue()->is_typed(cdk::TYPE_FUNCTIONAL)) {
      if (check_compatible_functions(cdk::functional_type::cast(node->lvalue()->type()), cdk::functional_type::cast(node->rvalue()->type()))) {
        node->type(node->lvalue()->type());
      } else {
        throw std::string("non matching function types in assignment");
      }
    } else {
      throw std::string("wrong assignment to function");
    }

  } else {
    throw std::string("wrong types in assignment");
  }
}

//---------------------------------------------------------------------------

void l22::type_checker::do_program_node(l22::program_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void l22::type_checker::do_evaluation_node(l22::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void l22::type_checker::do_block_node(l22::block_node *const node, int lvl) {
  // EMPTY
}

void l22::type_checker::do_index_node(l22::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  std::shared_ptr<cdk::reference_type> btype;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    btype = cdk::reference_type::cast(node->base()->type());
    if (!node->base()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected in index left-value");
  } else {
    btype = cdk::reference_type::cast(_funcs.top()->output(0));
    if (_funcs.top()->output(0)->name() != cdk::TYPE_POINTER) throw std::string("return pointer expression expected in index left-value");
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected in left-value index");

  node->type(btype->referenced());
}

void l22::type_checker::do_address_of_node(l22::address_of_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

void l22::type_checker::do_stack_alloc_node(l22::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in allocation expression");
  }

  auto mytype = cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
  node->type(mytype);
}

//---------------------------------------------------------------------------

void l22::type_checker::do_write_node(l22::write_node *const node, int lvl) {
  node->arguments()->accept(this, lvl + 2);

  for (size_t i = 0; i < node->arguments()->size(); i++) {
    cdk::expression_node* n = dynamic_cast<cdk::expression_node*>(node->arguments()->node(i));
    if (n->is_typed(cdk::TYPE_POINTER))
      throw std::string("invalid write argument type (pointer)");
    else if (n->is_typed(cdk::TYPE_FUNCTIONAL))
      throw std::string("invalid write argument type (function)");
    else if (n->is_typed(cdk::TYPE_UNSPEC))  // input
      n->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
  }
}

void l22::type_checker::do_input_node(l22::input_node *const node, int lvl) {
  node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void l22::type_checker::do_while_node(l22::while_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void l22::type_checker::do_stop_node(l22::stop_node *const node, int lvl) {
  // EMPTY
}

void l22::type_checker::do_again_node(l22::again_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void l22::type_checker::do_if_node(l22::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT)) throw std::string("expected integer condition");
}

void l22::type_checker::do_if_else_node(l22::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if (!node->condition()->is_typed(cdk::TYPE_INT)) throw std::string("expected integer condition");
}

//---------------------------------------------------------------------------

void l22::type_checker::do_sizeof_node(l22::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->expression()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void l22::type_checker::do_variable_declaration_node(l22::variable_declaration_node *const node, int lvl) {
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);

    if (node->type() == nullptr) {  // var

      if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {  // input
        node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      } else if (node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        auto ptype = node->initializer()->type();
        while (ptype->name() == cdk::TYPE_POINTER) {
          ptype = cdk::reference_type::cast(ptype)->referenced();
        }
        if (ptype->name() == cdk::TYPE_UNSPEC) {  // stack alloc
          throw std::string("can't allocate memory for unspecified data type");
        }
      }

      node->type(node->initializer()->type());

    } else if (node->is_typed(cdk::TYPE_INT)) {

      if (!node->initializer()->is_typed(cdk::TYPE_INT)) {
        throw std::string("wrong type for initializer (integer expected)");
      } else if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {  // input
        node->initializer()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      }

    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {

      if (!node->initializer()->is_typed(cdk::TYPE_INT) && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for initializer (integer or double expected)");
      } else if (node->initializer()->is_typed(cdk::TYPE_UNSPEC)) {  // input
        node->initializer()->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      }

    } else if (node->is_typed(cdk::TYPE_STRING)) {

      if (!node->initializer()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for initializer (string expected)");
      }

    } else if (node->is_typed(cdk::TYPE_POINTER)) {

      if (!node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        throw std::string("wrong type for initializer (pointer expected)");
      } else {
        int type_check = check_pointers_types(node->type(), node->initializer()->type());
        if (type_check == 1) {
          throw std::string("non matching pointer levels in declaration");
        } else if (type_check == 2) {
          node->initializer()->type(node->type());
        } else if (type_check == 3) {
          throw std::string("non matching pointer types in declaration");
        }
      }

    } else if (node->is_typed(cdk::TYPE_FUNCTIONAL)) {

      if (!node->initializer()->is_typed(cdk::TYPE_FUNCTIONAL)) {
        throw std::string("wrong type for initializer (function expected)");
      } else {
        if (!check_compatible_functions(cdk::functional_type::cast(node->type()), cdk::functional_type::cast(node->initializer()->type()))) {
          throw std::string("non matching function types in declaration");
        }
      }

    } else {
      throw std::string("unknown type for initializer");
    }
  }

  const std::string &id = node->identifier();
  auto symbol = l22::make_symbol(false, node->qualifier(), node->type(), id, (bool)node->initializer());
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

//---------------------------------------------------------------------------

void l22::type_checker::do_function_call_node(l22::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  std::shared_ptr<cdk::basic_type> type;
  if (node->function()) {  // identifier or function definition
    node->function()->accept(this, lvl + 2);
    type = node->function()->type();
  } else {  // recursive call
    type = _funcs.top();
  }

  if (!(type->name() == cdk::TYPE_FUNCTIONAL)) {
    throw std::string("non functional expression can't be called");
  }

  auto func_type = cdk::functional_type::cast(type);
  node->type(func_type->output(0));

  if (node->arguments()->size() == func_type->input_length()) {
    node->arguments()->accept(this, lvl + 4);
    for (size_t ax = 0; ax < node->arguments()->size(); ax++) {
      if (node->argument(ax)->type() == func_type->input(ax)) {
        if (node->argument(ax)->is_typed(cdk::TYPE_POINTER)) {
          if (!check_pointers_types(node->argument(ax)->type(), func_type->input(ax))) {
            continue;
          } else {
            throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " in function call");
          }
        } else if (node->argument(ax)->is_typed(cdk::TYPE_FUNCTIONAL)) {
          if (check_compatible_functions(cdk::functional_type::cast(func_type->input(ax)), cdk::functional_type::cast(node->argument(ax)->type()))) {
            continue;
          } else {
            throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " in function call");
          }
        } else {
          continue;
        }
      }
      if (func_type->input(ax)->name() == cdk::TYPE_DOUBLE && node->argument(ax)->is_typed(cdk::TYPE_INT)) continue;
      if (node->argument(ax)->is_typed(cdk::TYPE_UNSPEC) && (func_type->input(ax)->name() == cdk::TYPE_INT || func_type->input(ax)->name() == cdk::TYPE_DOUBLE)) {  // input
        node->argument(ax)->type(func_type->input(ax));
        continue;
      }
      throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " in function call");
    }
  } else {
    throw std::string(
        "number of arguments in call (" + std::to_string(node->arguments()->size()) + ") must match declaration ("
            + std::to_string(func_type->input_length()) + ")");
  }
}

void l22::type_checker::do_function_definition_node(l22::function_definition_node *const node, int lvl) {
  // EMPTY
}

void l22::type_checker::do_return_node(l22::return_node *const node, int lvl) {
  if (node->retval()) {
    auto func_type = _funcs.top();
    auto output_type = func_type->output(0);

    if (output_type->name() == cdk::TYPE_VOID) {
      throw std::string("return specified for void function");
    }

    node->retval()->accept(this, lvl + 2);

    if (output_type->name() == cdk::TYPE_INT) {

      if (!node->retval()->is_typed(cdk::TYPE_INT)) {
        throw std::string("wrong type for return expression (integer expected)");
      }

    } else if (output_type->name() == cdk::TYPE_DOUBLE) {

      if (!node->retval()->is_typed(cdk::TYPE_INT) && !node->retval()->is_typed(cdk::TYPE_DOUBLE)) {
        throw std::string("wrong type for return expression (integer or double expected)");
      }

    } else if (output_type->name() == cdk::TYPE_STRING) {

      if (!node->retval()->is_typed(cdk::TYPE_STRING)) {
        throw std::string("wrong type for return expression (string expected)");
      }

    } else if (output_type->name() == cdk::TYPE_POINTER) {

      if (!node->retval()->is_typed(cdk::TYPE_POINTER)) {
        throw std::string("wrong type for return expression (pointer expected)");
      }

      int type_check = check_pointers_types(output_type, node->retval()->type());
      if (type_check == 1) {
        throw std::string("wrong type for return expression (non matching pointer levels)");
      } else if (type_check == 3) {
        throw std::string("wrong type for return expression (non matching pointer types)");
      }

    } else if (output_type->name() == cdk::TYPE_FUNCTIONAL) {

      if (!node->retval()->is_typed(cdk::TYPE_FUNCTIONAL)) {
        throw std::string("wrong type for return expression (function expected)");
      } else {
        if (!check_compatible_functions(cdk::functional_type::cast(output_type), cdk::functional_type::cast(node->retval()->type()))) {
          throw std::string("wrong type for return expression (non matching function types)");
        }
      }

    } else {
      throw std::string("unknown type for return expression");
    }
  }
}

//---------------------------- AUXILIARY FUNCTIONS --------------------------

int l22::type_checker::check_pointers_types(std::shared_ptr<cdk::basic_type> const left, std::shared_ptr<cdk::basic_type> const right) {
  int left_lvl = 0, right_lvl = 0;

  auto left_ref = left;
  while (left_ref->name() == cdk::TYPE_POINTER) {
    left_lvl++;
    left_ref = cdk::reference_type::cast(left_ref)->referenced();
  }

  auto right_ref = right;
  auto referenced = cdk::reference_type::cast(right_ref)->referenced();
  if (referenced == nullptr) return 0; // null
  while (right_ref->name() == cdk::TYPE_POINTER) {
    right_lvl++;
    right_ref = cdk::reference_type::cast(right_ref)->referenced();
  }

  if (left_lvl != right_lvl) return 1;
  else if (right_ref->name() == cdk::TYPE_UNSPEC) return 2; // stack alloc
  else if (left_ref->name() != right_ref->name()) return 3;
  else return 0;
}

bool l22::type_checker::check_compatible_functions(std::shared_ptr<cdk::functional_type> left, std::shared_ptr<cdk::functional_type> right) {
  if (check_functions_args(left->input(), right->input()) && check_functions_output(left->output(0), right->output(0))) return true;
  else return false;
}

bool l22::type_checker::check_functions_output(std::shared_ptr<cdk::basic_type> left, std::shared_ptr<cdk::basic_type> right) {
  if (left->name() == right->name()) {
      if (left->name() == cdk::TYPE_POINTER) {
        if (!check_pointers_types(left, right)) return true;
        else return false;
      } else if (left->name() == cdk::TYPE_FUNCTIONAL) {
        if (check_compatible_functions(cdk::functional_type::cast(left), cdk::functional_type::cast(right))) return true;
        else return false;
      } else {
        return true;
      }
    } else if (left->name() == cdk::TYPE_DOUBLE && right->name() == cdk::TYPE_INT) {
      return true;
    } else {
      return false;
    }
}

bool l22::type_checker::check_functions_args(std::shared_ptr<cdk::structured_type> left, std::shared_ptr<cdk::structured_type> right) {
  if (left->length() != right->length()) return false;

  for (size_t i = 0; i < left->length(); i++) {
    if (left->component(i)->name() == right->component(i)->name()) {
      if (left->component(i)->name() == cdk::TYPE_POINTER) {
        if (!check_pointers_types(left->component(i), right->component(i))) continue;
        else return false;
      } else if (left->component(i)->name() == cdk::TYPE_FUNCTIONAL) {
        if (check_compatible_functions(cdk::functional_type::cast(left->component(i)), cdk::functional_type::cast(right->component(i)))) continue;
        else return false;
      } else {
        continue;
      }
    } else if (left->component(i)->name() == cdk::TYPE_INT && right->component(i)->name() == cdk::TYPE_DOUBLE) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}