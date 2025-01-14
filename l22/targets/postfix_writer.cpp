#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated
#include "l22_parser.tab.h"

//---------------------------------------------------------------------------

void l22::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void l22::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

void l22::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value());  // load number to the stack
  } else {
    _pf.SDOUBLE(node->value()); // double is on the DATA segment
  }
}

void l22::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value());  // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void l22::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;
  /* generate the string literal */
  _pf.RODATA(); // strings are readonly DATA
  _pf.ALIGN(); // make sure the address is aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters
  if (_funcs.size() > 0) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

void l22::postfix_writer::do_null_node(l22::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // a pointer is a 32-bit integer
  if (_inFunctionBody) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_identity_node(l22::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

void l22::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DNEG();  // 2-complement
  } else {
    _pf.NEG(); // 2-complement
  }
}

void l22::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    auto ptype = cdk::reference_type::cast(node->type());
    _pf.INT(ptype->referenced()->size());
    _pf.MUL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    auto ptype = cdk::reference_type::cast(node->type());
    _pf.INT(ptype->referenced()->size());
    _pf.MUL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DADD();
  else
    _pf.ADD();
}

void l22::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    auto ptype = cdk::reference_type::cast(node->type());
    _pf.INT(ptype->referenced()->size());
    _pf.MUL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DSUB();
  else
    _pf.SUB();
}

void l22::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DMUL();
  else
    _pf.MUL();
}

void l22::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.DDIV();
  else
    _pf.DIV();
}

void l22::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  _pf.MOD();
}

void l22::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.LT();
}

void l22::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.LE();
}

void l22::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.GE();
}

void l22::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.I2D();
  }

  if (node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.GT();
}

void l22::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.NE();
}

void l22::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.DCMP();
    _pf.INT(0);
  }

  _pf.EQ();
}

void l22::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void l22::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  } else {
    _pf.LOCAL(symbol->offset());
  }
}

void l22::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDDOUBLE();
  } else {
    // integers, pointers, strings and functions
    _pf.LDINT();
  }
  if (_inFunctionCall) {
    _pf.BRANCH();
  }
}

void l22::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      _pf.I2D();
    }
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_program_node(l22::program_node * const node, int lvl) {
  _funcs.push(cdk::functional_type::create(cdk::primitive_type::create(4, cdk::TYPE_INT)));
  _symtab.push();

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab, _funcs);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  _offset = -4; // int size, prepare for local variable

  _inFunctionBody++;
  node->statements()->accept(this, lvl);
  _inFunctionBody--;

  // end the main function
  _pf.LEAVE();
  _pf.RET();

  _symtab.pop();
  _funcs.pop();

  for (std::string s : _functions_to_declare) {
    _pf.EXTERN(s);
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_evaluation_node(l22::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.TRASH(node->argument()->type()->size());
}

void l22::postfix_writer::do_block_node(l22::block_node * const node, int lvl) {
  _symtab.push(); // for block-local vars
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

void l22::postfix_writer::do_index_node(l22::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(node->type()->size());
  _pf.MUL();
  _pf.ADD(); // add pointer and index
}

void l22::postfix_writer::do_address_of_node(l22::address_of_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // since the argument is an lvalue, it is already an address
  node->lvalue()->accept(this, lvl + 2);
}

void l22::postfix_writer::do_stack_alloc_node(l22::stack_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  auto ptype = cdk::reference_type::cast(node->type());
  _pf.INT(ptype->referenced()->size());
  _pf.MUL();
  _pf.ALLOC(); // allocate
  _pf.SP(); // put base pointer in stack
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_write_node(l22::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
    auto child = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ix));

    std::shared_ptr<cdk::basic_type> etype = child->type();
    child->accept(this, lvl); // expression to print
    if (etype->name() == cdk::TYPE_INT) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // trash int
    } else if (etype->name() == cdk::TYPE_DOUBLE) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // trash double
    } else if (etype->name() == cdk::TYPE_STRING) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // trash char pointer
    } else {
      std::cerr << "cannot print expression of unknown type" << std::endl;
      return;
    }

  }

  if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

void l22::postfix_writer::do_input_node(l22::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else if (node->is_typed(cdk::TYPE_INT)) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else {
    std::cerr << "FATAL: " << node->lineno() << ": cannot read type" << std::endl;
    return;
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_while_node(l22::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  _whileIni.push(++_lbl); // before body
  _whileEnd.push(++_lbl); // after while

  // prepare to test
  _pf.ALIGN();
  _pf.LABEL(mklbl(_whileIni.top()));
  node->condition()->accept(this, lvl + 2);
  _pf.JZ(mklbl(_whileEnd.top()));

  // execute block
  node->block()->accept(this, lvl + 2);

  _pf.JMP(mklbl(_whileIni.top()));

  _pf.ALIGN();
  _pf.LABEL(mklbl(_whileEnd.top()));

  _whileIni.pop();
  _whileEnd.pop();
}

void l22::postfix_writer::do_stop_node(l22::stop_node * const node, int lvl) {
  if (_whileIni.size() != 0) {
    _pf.JMP(mklbl(_whileEnd.top())); // jump to while end
  } else
    std::cerr << "error: " << node->lineno() << ": " << "'stop' outside 'while'" << std::endl;
}

void l22::postfix_writer::do_again_node(l22::again_node * const node, int lvl) {
  if (_whileIni.size() != 0) {
    _pf.JMP(mklbl(_whileIni.top())); // jump to next cycle
  } else
    std::cerr << "error: " << node->lineno() << ": " << "'again' outside 'while'" << std::endl;
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_if_node(l22::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_end;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_end = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl_end));
}

void l22::postfix_writer::do_if_else_node(l22::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl_else, lbl_end;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl_else = lbl_end = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  if (node->elseblock()) {
    _pf.JMP(mklbl(lbl_end = ++_lbl));
    _pf.LABEL(mklbl(lbl_else));
    node->elseblock()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbl_end));
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_sizeof_node(l22::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->expression()->type()->size());
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_variable_declaration_node(l22::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();

  // type size?
  int offset = 0, typesize = node->type()->size(); // in bytes
  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
    offset = 0; // global variable
  }

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER) || node->is_typed(cdk::TYPE_FUNCTIONAL)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        if (node->initializer()->is_typed(cdk::TYPE_INT))
          _pf.I2D();
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } else {
    if (_funcs.size() == 0) {
      if (node->initializer() == nullptr) {
        if (node->qualifier() == tFOREIGN) {
          _functions_to_declare.insert(id);
        } else {
          _pf.BSS();
          _pf.ALIGN();
          if (node->qualifier() == tPUBLIC) _pf.GLOBAL(id, _pf.OBJ());
          _pf.LABEL(id);
          _pf.SALLOC(typesize);
        }
      } else {

        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
          _pf.DATA();
          _pf.ALIGN();
          if (node->qualifier() == tPUBLIC) _pf.GLOBAL(id, _pf.OBJ());
          _pf.LABEL(id);

          if (node->is_typed(cdk::TYPE_INT)) {

            node->initializer()->accept(this, lvl);

          } else if (node->is_typed(cdk::TYPE_POINTER)) {

            node->initializer()->accept(this, lvl);

          } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            
            if (node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
              node->initializer()->accept(this, lvl);
            } else if (node->initializer()->is_typed(cdk::TYPE_INT)) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node*>(node->initializer());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
            }

          }
        } else if (node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_FUNCTIONAL)) {

            _pf.DATA();
            _pf.ALIGN();
            if (node->qualifier() == tPUBLIC) _pf.GLOBAL(id, _pf.OBJ());
            _pf.LABEL(id);
            node->initializer()->accept(this, lvl);

        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
        }
      }
    }
  }
}

//---------------------------------------------------------------------------

void l22::postfix_writer::do_function_call_node(l22::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  size_t argsSize = 0;
  if (node->arguments()->size() > 0) {
    for (int ax = node->arguments()->size() - 1; ax >= 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax));
      arg->accept(this, lvl + 2);
      if (node->function()) {
        if (cdk::functional_type::cast(node->function()->type())->input(ax)->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_INT)) {
          _pf.I2D();
        }
        argsSize += cdk::functional_type::cast(node->function()->type())->input(ax)->size();
      } else {
        if (_funcs.top()->input(ax)->name() == cdk::TYPE_DOUBLE && arg->is_typed(cdk::TYPE_INT)) {
          _pf.I2D();
        }
        argsSize += _funcs.top()->input(ax)->size();
      }
    }
  }

  _inFunctionCall++;

  if (node->function()) {
    node->function()->accept(this, lvl + 2);
  } else {  // recursive call
    _pf.CALL(mklbl(_lastFuncLbl));
  }

  _inFunctionCall--;

  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (_funcs.top()->output(0)->name() == cdk::TYPE_INT || _funcs.top()->output(0)->name() == cdk::TYPE_POINTER || _funcs.top()->output(0)->name() == cdk::TYPE_STRING || _funcs.top()->output(0)->name() == cdk::TYPE_FUNCTIONAL) {
    _pf.LDFVAL32();
  } else if (_funcs.top()->output(0)->name() == cdk::TYPE_DOUBLE) {
    _pf.LDFVAL64();
  } else {
    // cannot happen!
  }
}

void l22::postfix_writer::do_function_definition_node(l22::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _funcs.push(cdk::functional_type::cast(node->type()));

  _offset = 8; // prepare for arguments (4: remember to account for return address)
  _symtab.push(); // scope of args

  if (node->arguments()->size() > 0) {
    _inFunctionArgs = true;
    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
      cdk::basic_node *arg = node->arguments()->node(ix);
      if (arg == nullptr) break; // this means an empty sequence of arguments
      arg->accept(this, 0); // the function symbol is at the top of the stack
    }
    _inFunctionArgs = false;
  }

  int lbl1;

  if (_inFunctionCall) {
    _pf.CALL(mklbl(_lastFuncLbl = lbl1 = ++_lbl));
  };

  _pf.TEXT(mklbl(lbl1));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));

  // compute stack size to be reserved for local variables
  frame_size_calculator lsc(_compiler, _symtab, _funcs);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

  _offset = - cdk::functional_type::cast(node->type())->output(0)->size(); // prepare for local variable

  _inFunctionBody++;
  node->block()->accept(this, lvl + 4); // block has its own scope
  _inFunctionBody--;

  if (!_returnSeen && cdk::functional_type::cast(node->type())->output(0)->name() != cdk::TYPE_VOID) {
    std::cerr << "return missing on non void function" << std::endl;
  }

  _returnSeen = false;

  _pf.LEAVE();
  _pf.RET();

  _symtab.pop(); // scope of arguments
  _funcs.pop();

  if (_funcs.size() > 0) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

void l22::postfix_writer::do_return_node(l22::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  // should not reach here without returning a value (if not void)
  if (_funcs.top()->output(0)->name() != cdk::TYPE_VOID) {
    node->retval()->accept(this, lvl + 2);

    if (_funcs.top()->output(0)->name() == cdk::TYPE_INT || _funcs.top()->output(0)->name() == cdk::TYPE_STRING
        || _funcs.top()->output(0)->name() == cdk::TYPE_POINTER) {
      _pf.STFVAL32();
    } else if (_funcs.top()->output(0)->name() == cdk::TYPE_DOUBLE) {
      if (node->retval()->is_typed(cdk::TYPE_INT)) _pf.I2D();
      _pf.STFVAL64();
    } else {
      std::cerr << node->lineno() << ": should not happen: unknown return type" << std::endl;
    }
  }

  _pf.LEAVE();
  _pf.RET();
  _returnSeen = true;
}