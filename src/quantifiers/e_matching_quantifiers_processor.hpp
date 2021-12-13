#ifndef _E_MATCHING_QUANTIFIERS_PROCESSOR_H
#define _E_MATCHING_QUANTIFIERS_PROCESSOR_H

#include "quantifiers_processor.hpp"

class e_matching_quantifiers_processor : public quantifiers_processor {
public:
  e_matching_quantifiers_processor(solver & sl)
    :quantifiers_processor(sl)
  {}
  
  virtual void add_literal(const expression & l_pos, const expression & l_neg)
  {

  }

  virtual void new_level()
  {

  }
  
  virtual void backjump(unsigned level)
  {

  }
  
  virtual void check_quantifiers()
  {

  }
};


#endif // _E_MATCHING_QUANTIFIERS_PROCESSOR_H
