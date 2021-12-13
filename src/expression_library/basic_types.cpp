/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#include "basic_types.hpp"

object_factory<special_constant::data> * special_constant::_factory = 0;

const special_constant special_constant::UNDEFINED = 
  special_constant(T_UNDEF, "");

std::string convert_hex_to_binary(const std::string & str)
{    
  std::string res;
  for(unsigned i = 0; i < str.size(); i++)
    {
      switch(str[i])
	{
	case '0':
	  res += "0000";
	  break;
	case '1':
	  res += "0001";
	  break;
	case '2':
	  res += "0010";
	  break;
	case '3':
	  res += "0011";
	  break;
	case '4':
	  res += "0100";
	  break;
	case '5':
	  res += "0101";
	  break;
	case '6':
	  res += "0110";
	  break;
	case '7':
	  res += "0111";
	  break;
	case '8':
	  res += "1000";
	  break;
	case '9':
	  res += "1001";
	  break;
	case 'a':
	case 'A':
	  res += "1010";
	  break;
	case 'b':
	case 'B':
	  res += "1011";
	  break;
	case 'c':
	case 'C':
	  res += "1100";
	  break;
	case 'd':
	case 'D':
	  res += "1101";
	  break;
	case 'e':
	case 'E':
	  res += "1110";
	  break;
	case 'f':
	case 'F':
	  res += "1111";
	  break;
	}
    }
  return res;
}

special_constant::special_constant(type tp, unsigned long value)
{
  data d(tp, "");

  std::ostringstream ostr;
  switch(d._type)
    {
    case T_NUMERAL:
      ostr << value;
      d._value = ostr.str();
      break;
    case T_BINARY:
      ostr << std::setbase(16) << value;
      d._value = "#b" + convert_hex_to_binary(ostr.str());
      break;
    case T_HEXADECIMAL:
      ostr << "#x" << std::setbase(16) << value;
      d._value = ostr.str();
      break;
    default:
      throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
      break;
    }
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}

special_constant::special_constant(type tp, unsigned value)
{
  data d(tp, "");

  std::ostringstream ostr;
  switch(d._type)
    {
    case T_NUMERAL:
      ostr << value;
      d._value = ostr.str();
      break;
    case T_BINARY:
      ostr << std::setbase(16) << value;
      d._value = "#b" + convert_hex_to_binary(ostr.str());
      break;
    case T_HEXADECIMAL:
      ostr << "#x" << std::setbase(16) << value;
      d._value = ostr.str();
      break;
    default:
      throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
      break;
    }
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}

special_constant::special_constant(type tp, int value)
{
  value = abs(value); // ensure that the value is non-negative
  
  data d(tp, "");

  std::ostringstream ostr;
  switch(d._type)
    {
    case T_NUMERAL:
      ostr << value;
      d._value = ostr.str();
      break;
    case T_BINARY:
      ostr << std::setbase(16) << value;
      d._value = "#b" + convert_hex_to_binary(ostr.str());
      break;
    case T_HEXADECIMAL:
      ostr << "#x" << std::setbase(16) << value;
      d._value = ostr.str();
      break;
    default:
      throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
      break;
    }
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}



void remove_trailing_zeros(std::string & str)
{
  int i;
  for(i = str.size() - 1; i > 0; i--)
    if(str[i] != '0')
      break;
  str.resize(i + 1);
  if(str[str.size() - 1] == '.')
    str += "0";
}

special_constant::special_constant(type tp, long double value,
				   unsigned precision)
{
  data d(tp, "");
    
  if(d._type != T_DECIMAL)
    throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
    
  std::ostringstream ostr;
  ostr.precision(precision);
  ostr << std::fixed << value;
  d._value = ostr.str();
  remove_trailing_zeros(d._value);
  d.calculate_hash();
 
  _data = get_factory().add_object(std::move(d));
}

special_constant::special_constant(type tp, double value,
				   unsigned precision)
{
  data d(tp, "");
    
  if(d._type != T_DECIMAL)
    throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
    
  std::ostringstream ostr;
  ostr.precision(precision);
  ostr << std::fixed << value;
  d._value = ostr.str();
  remove_trailing_zeros(d._value);
  d.calculate_hash();
 
  _data = get_factory().add_object(std::move(d));
}


#ifdef _USE_GMP
special_constant::special_constant(type tp, const mpz_class & value)
{   
  data d(tp, "");

  switch(d._type)
    {
    case T_NUMERAL:
      d._value = value.get_str(10);
      break;
    case T_BINARY:
      d._value = "#b" + value.get_str(2);
      break;
    case T_HEXADECIMAL:
      d._value = "#x" + value.get_str(16);
      break;
    default:
      throw special_constant_initialization_exception("Special constant initialized with a bad value",
						      d._type);
      break;
    }
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}


special_constant::special_constant(type tp, const mpf_class & value,
				   unsigned precision)
{
  data d(tp, "");

  if(d._type != T_DECIMAL)
    throw special_constant_initialization_exception("Special constant initialized with a bad value", 
						    d._type);
    
  std::ostringstream ostr;
  ostr.precision(precision);
  ostr << std::fixed << value;
  d._value = ostr.str();
  remove_trailing_zeros(d._value);
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}


special_constant::special_constant(type tp, const mpq_class & value,
				   unsigned precision)
{          

  data d(tp, "");

  if(d._type == T_DECIMAL)
    {
      std::ostringstream ostr;
      ostr.precision(precision);
      ostr << std::fixed << mpf_class(value);
      d._value = ostr.str();
      remove_trailing_zeros(d._value);
    }
  else if(d._type == T_NUMERAL && value.get_den() == 1)
    {
      d._value = value.get_num().get_str(10);
    }
  else if(d._type == T_BINARY && value.get_den() == 1)
    {
      d._value = "#b" + value.get_num().get_str(2);
    }
  else if(d._type == T_HEXADECIMAL && value.get_den() == 1)
    {
      d._value = "#x" + value.get_num().get_str(16);
    }
  else
    throw special_constant_initialization_exception("Special constant initialized with a bad value", d._type);
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}

#endif

  
unsigned long special_constant::get_ul_value() const
{
  const data & d = *_data;

  switch(d._type)
    {
    case T_NUMERAL:
      return strtoul(d._value.c_str(), 0, 10);
      break;
    case T_BINARY:
      return strtoul(d._value.c_str() + 2, 0, 2);
      break;
    case T_HEXADECIMAL:
      return strtoul(d._value.c_str() + 2, 0, 16);
      break;
    default:
      throw bad_special_constant_type_exception("Special constant is not numeral, binary or hexadecimal", *this);
      break;
    }
}

unsigned special_constant::get_u_value() const
{
  const data & d = *_data;

  switch(d._type)
    {
    case T_NUMERAL:
      return (unsigned)strtoul(d._value.c_str(), 0, 10);
      break;
    case T_BINARY:
      return (unsigned)strtoul(d._value.c_str() + 2, 0, 2);
      break;
    case T_HEXADECIMAL:
      return (unsigned)strtoul(d._value.c_str() + 2, 0, 16);
      break;
    default:
      throw bad_special_constant_type_exception("Special constant is not numeral, binary or hexadecimal", *this);
      break;
    }
}

int special_constant::get_i_value() const
{
  const data & d = *_data;

  switch(d._type)
    {
    case T_NUMERAL:
      return (int)strtoul(d._value.c_str(), 0, 10);
      break;
    case T_BINARY:
      return (int)strtoul(d._value.c_str() + 2, 0, 2);
      break;
    case T_HEXADECIMAL:
      return (int)strtoul(d._value.c_str() + 2, 0, 16);
      break;
    default:
      throw bad_special_constant_type_exception("Special constant is not numeral, binary or hexadecimal", *this);
      break;
    }
}


long double special_constant::get_ld_value() const
{
  const data & d = *_data;

  if(d._type != T_DECIMAL)
    throw bad_special_constant_type_exception("Special constant is not decimal", 
					      *this);
    
  return strtold(d._value.c_str(), 0);
}

double special_constant::get_d_value() const
{
  const data & d = *_data;

  if(d._type != T_DECIMAL)
    throw bad_special_constant_type_exception("Special constant is not decimal", 
					      *this);
    
  return strtod(d._value.c_str(), 0);
}


#ifdef _USE_GMP

mpz_class special_constant::get_gmpz_value() const
{    
  const data & d = *_data;

  switch(d._type)
    {
    case T_NUMERAL:
      return mpz_class(d._value.c_str(), 10);
      break;
    case T_BINARY:
      return mpz_class(d._value.c_str() + 2, 2);
      break;
    case T_HEXADECIMAL:
      return mpz_class(d._value.c_str() + 2, 16);
      break;
    default:
      throw bad_special_constant_type_exception("Special constant is not numeral, binary or hexadecimal", *this);
      break;
    }
}
  
mpf_class special_constant::get_gmpf_value(unsigned precision) const
{
  const data & d = *_data;

  if(d._type != T_DECIMAL)
    throw bad_special_constant_type_exception("Special constant is not decimal",
					      *this);
    
  return mpf_class(d._value.c_str(), 
		   precision != 0 ? precision : mpf_get_default_prec());
}


mpq_class get_rational_from_decimal(const std::string & value)
{
  bool fraction = false;
  unsigned frac_count = 0;
  std::string rational;
  for(unsigned i = 0; i < value.size(); i++)
    {
      if(fraction)
	frac_count++;

      if(value[i] == '.')
	fraction = true;
      else
	rational.push_back(value[i]);
    }

  rational.push_back('/');
  rational.push_back('1');
  while(frac_count--)
    {
      rational.push_back('0');
    }
    
  mpq_class ret_val(rational, 10);
  ret_val.canonicalize();
  return ret_val;
}


mpq_class special_constant::get_gmpq_value() const
{
  const data & d = *_data;

  switch(d._type)
    {
    case T_NUMERAL:
      return mpq_class(d._value.c_str(), 10);
      break;
    case T_BINARY:
      return mpq_class(d._value.c_str() + 2, 2);
      break;
    case T_HEXADECIMAL:
      return mpq_class(d._value.c_str() + 2, 16);
      break;
    case T_DECIMAL:
      return get_rational_from_decimal(d._value);
      break;
    default:
      throw bad_special_constant_type_exception("Special constant is not numeral, binary or hexadecimal", *this);
      break;
    }
}

#endif

object_factory<symbol::data> * symbol::_factory = 0;
object_factory<keyword::data> * keyword::_factory = 0;
object_factory<identifier::data> * identifier::_factory = 0;

const identifier::index identifier::UNSPECIFIED_INDEX = identifier::index();
const identifier::index_vector identifier::EMPTY_INDEX_VECTOR = index_vector();
identifier::index_vector identifier::_dummy_index_vector = index_vector();

bool identifier_weak_comparator::
operator () (const identifier & id1, const identifier & id2) const
{
  if(id1.get_name() != id2.get_name())
    return false;
    
  if(id1.get_indices().size() != id2.get_indices().size())
    return false;
  
  for(identifier::index_vector::const_iterator 
	it1 = id1.get_indices().begin(), 
	it2 = id2.get_indices().begin(), 
	it1_end = id1.get_indices().end(); it1 != it1_end; ++it1, ++it2)
    {
      if(*it1 == identifier::UNSPECIFIED_INDEX ||
	 *it2 == identifier::UNSPECIFIED_INDEX)
	continue;
      
      if(*it1 != *it2)
	return false;
    } 
  return true;
}

void identifier::out(std::ostream & ostr) const
{
  if(_data->_indices->empty())
    ostr << _data->_name;
  else
    {
      ostr << "(_ ";
      ostr << _data->_name;
      
      for(index_vector::const_iterator 
	    it = _data->_indices->begin(), 
	    end = _data->_indices->end(); 
	  it != end; ++it)
	{
	  ostr << ' ';
	  it->out(ostr);
	}
      ostr << ")";
    }
}

std::ostream & operator << (std::ostream & ostr, const attribute_set & attr)
{
  for(attribute_set::const_iterator it = attr.begin(),
	it_end = attr.end(); it != it_end; ++it)
    ostr << *it << " "; 
    
  return ostr;
}

const attribute_set EMPTY_ATTRIBUTE_SET = attribute_set(HASH_TABLE_SIZE);

const keyword keyword::LEFT_ASSOC = keyword(":left-assoc");
const keyword keyword::RIGHT_ASSOC = keyword(":right-assoc");
const keyword keyword::CHAINABLE = keyword(":chainable");
const keyword keyword::PAIRWISE = keyword(":pairwise");
const keyword keyword::NAMED = keyword(":named");
const keyword keyword::PATTERN = keyword(":pattern");
const keyword keyword::DEFINITION = keyword(":definition");
const keyword keyword::STATUS = keyword(":status");
const keyword keyword::ALL_STATISTICS = keyword(":all-statistics");
const keyword keyword::PRINT_SUCCESS = keyword(":print-success");
const keyword keyword::EXPAND_DEFINITIONS = keyword(":expand-definitions");
const keyword keyword::INTERACTIVE_MODE = keyword(":interactive-mode");
const keyword keyword::PRODUCE_ASSERTIONS = keyword(":produce-assertions");
const keyword keyword::PRODUCE_PROOFS = keyword(":produce-proofs");
const keyword keyword::PRODUCE_UNSAT_CORES = keyword(":produce-unsat-cores");
const keyword keyword::PRODUCE_MODELS = keyword(":produce-models");
const keyword keyword::PRODUCE_ASSIGNMENTS = keyword(":produce-assignments");
const keyword keyword::GLOBAL_DECLARATIONS = keyword(":global-declarations");
const keyword keyword::SYNTAX_CHECKING = keyword(":syntax-checking");
const keyword keyword::LOGIC_SYNTAX_CHECKING = keyword(":logic-syntax-checking");
const keyword keyword::REGULAR_OUTPUT_CHANNEL = keyword(":regular-output-channel");
const keyword keyword::DIAGNOSTIC_OUTPUT_CHANNEL = keyword(":diagnostic-output-channel");
const keyword keyword::RANDOM_SEED = keyword(":random-seed");
const keyword keyword::VERBOSITY = keyword(":verbosity");
const keyword keyword::ERROR_BEHAVIOR = keyword(":error-behavior");
const keyword keyword::REASON_UNKNOWN = keyword(":reason-unknown");

const sort_symbol sort_symbol::UNDEFINED = sort_symbol("");
const sort_symbol sort_symbol::BOOL = sort_symbol("Bool");
const sort_symbol sort_symbol::INT = sort_symbol("Int");
const sort_symbol sort_symbol::REAL = sort_symbol("Real");
const sort_symbol sort_symbol::ARRAY = sort_symbol("Array");
const sort_symbol sort_symbol::BITVEC = sort_symbol("BitVec", { identifier::UNSPECIFIED_INDEX });

const sort_parameter sort_parameter::UNDEFINED = sort_parameter("");
const sort_parameter_vector EMPTY_SORT_PARAMETER_VECTOR = sort_parameter_vector();
const sort_parameter_set EMPTY_SORT_PARAMETER_SET = sort_parameter_set();

const function_symbol function_symbol::UNDEFINED = function_symbol("");
const function_symbol function_symbol::TRUE = function_symbol("true");
const function_symbol function_symbol::FALSE = function_symbol("false");
const function_symbol function_symbol::NOT = function_symbol("not");
const function_symbol function_symbol::IMPLIES = function_symbol("=>");
const function_symbol function_symbol::AND = function_symbol("and");
const function_symbol function_symbol::OR = function_symbol("or");
const function_symbol function_symbol::XOR = function_symbol("xor");
const function_symbol function_symbol::EQ = function_symbol("=");
const function_symbol function_symbol::DISTINCT = function_symbol("distinct");
const function_symbol function_symbol::ITE = function_symbol("ite");
const function_symbol function_symbol::MINUS = function_symbol("-");
const function_symbol function_symbol::PLUS = function_symbol("+");
const function_symbol function_symbol::TIMES = function_symbol("*");
const function_symbol function_symbol::DIV = function_symbol("div");
const function_symbol function_symbol::SLASH = function_symbol("/");
const function_symbol function_symbol::MOD = function_symbol("mod");
const function_symbol function_symbol::ABS = function_symbol("abs");
const function_symbol function_symbol::LT = function_symbol("<");
const function_symbol function_symbol::LE = function_symbol("<=");
const function_symbol function_symbol::GT = function_symbol(">");
const function_symbol function_symbol::GE = function_symbol(">=");
const function_symbol function_symbol::DIVISIBLE = function_symbol("divisible", { identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::TO_REAL = function_symbol("to_real");
const function_symbol function_symbol::TO_INT = function_symbol("to_int");
const function_symbol function_symbol::IS_INT = function_symbol("is_int");
const function_symbol function_symbol::SELECT = function_symbol("select");
const function_symbol function_symbol::STORE = function_symbol("store");
const function_symbol function_symbol::CONCAT = function_symbol("concat");
const function_symbol function_symbol::EXTRACT = function_symbol("extract", { identifier::UNSPECIFIED_INDEX, identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::BVNOT = function_symbol("bvnot");
const function_symbol function_symbol::BVNEG = function_symbol("bvneg");
const function_symbol function_symbol::BVAND = function_symbol("bvand");
const function_symbol function_symbol::BVOR = function_symbol("bvor");
const function_symbol function_symbol::BVADD = function_symbol("bvadd");
const function_symbol function_symbol::BVMUL = function_symbol("bvmul");
const function_symbol function_symbol::BVUDIV = function_symbol("bvudiv");
const function_symbol function_symbol::BVUREM = function_symbol("bvurem");
const function_symbol function_symbol::BVSHL = function_symbol("bvshl");
const function_symbol function_symbol::BVLSHR = function_symbol("bvlshr");
const function_symbol function_symbol::BVULT = function_symbol("bvult");
const function_symbol function_symbol::BVNAND = function_symbol("bvnand");
const function_symbol function_symbol::BVNOR = function_symbol("bvnor");
const function_symbol function_symbol::BVXOR = function_symbol("bvxor");
const function_symbol function_symbol::BVXNOR = function_symbol("bvxnor");
const function_symbol function_symbol::BVCOMP = function_symbol("bvcomp");
const function_symbol function_symbol::BVSUB = function_symbol("bvsub");
const function_symbol function_symbol::BVSDIV = function_symbol("bvsdiv");
const function_symbol function_symbol::BVSREM = function_symbol("bvsrem");
const function_symbol function_symbol::BVSMOD = function_symbol("bvsmod");
const function_symbol function_symbol::BVASHR = function_symbol("bvashr");
const function_symbol function_symbol::REPEAT = function_symbol("repeat", { identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::ZERO_EXTEND = function_symbol("zero_extend", { identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::SIGN_EXTEND = function_symbol("sign_extend", { identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::ROTATE_LEFT = function_symbol("rotate_left", { identifier::UNSPECIFIED_INDEX });
const function_symbol function_symbol::ROTATE_RIGHT = function_symbol("rotate_right", { identifier::UNSPECIFIED_INDEX});
const function_symbol function_symbol::BVULE = function_symbol("bvule");
const function_symbol function_symbol::BVUGT = function_symbol("bvugt");
const function_symbol function_symbol::BVUGE = function_symbol("bvuge");
const function_symbol function_symbol::BVSLT = function_symbol("bvslt");
const function_symbol function_symbol::BVSLE = function_symbol("bvsle");
const function_symbol function_symbol::BVSGT = function_symbol("bvsgt");
const function_symbol function_symbol::BVSGE = function_symbol("bvsge");

const function_symbol function_symbol::ALLDIFF =  function_symbol("alldiff");
const function_symbol function_symbol::NOT_ALLDIFF = function_symbol("not_alldiff");

const variable variable::UNDEFINED = variable("");
const variable_vector EMPTY_VARIABLE_VECTOR = variable_vector();


