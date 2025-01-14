#ifndef __L22_TARGETS_SYMBOL_H__
#define __L22_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace l22 {

  class symbol {
    
    std::string _name; // identifier
    int _value;

    bool _constant; // is it a constant?
    int _qualifier; // qualifiers: public, external, "private" (i.e., none)
    std::shared_ptr<cdk::basic_type> _type; // type (type id + type size)
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;
    bool _initialized; // initialized?
    int _offset = 0; // 0 (zero) means global variable

  public:
    symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name, bool initialized) :
        _name(name), _value(0), _constant(constant), _qualifier(qualifier), _type(type), _initialized(initialized) {
    }

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }

    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    const std::string &name() const {
      return _name;
    }

    int value() const {
      return _value;
    }

    int value(int v) {
      return _value = v;
    }

    bool constant() const {
      return _constant;
    }

    int qualifier() const {
      return _qualifier;
    }

    const std::string& identifier() const {
      return name();
    }

    bool initialized() const {
      return _initialized;
    }

    int offset() const {
      return _offset;
    }

    void set_offset(int offset) {
      _offset = offset;
    }

    bool global() const {
      return _offset == 0;
    }

    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }

    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }

  };

  inline auto make_symbol(bool constant, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &name,
                          bool initialized) {
    return std::make_shared<symbol>(constant, qualifier, type, name, initialized);
  }

} // l22

#endif
