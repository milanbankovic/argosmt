/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public Lcense
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#include "cmd_line_parser.hpp"
#include "solver_configuration.hpp"

unsigned cmd_line_parser::_timeout = 0;
unsigned cmd_line_parser::_decide_limit = 0;
std::ifstream cmd_line_parser::_input_file;
std::ofstream cmd_line_parser::_model_out;
std::istream * cmd_line_parser::_input = &std::cin;
std::ifstream cmd_line_parser::_config_file;

#include "config.h"

void cmd_line_parser::parse_cmd_line(int argc, char ** argv)
{
  // For compatibility with older versions..
  if(argc == 2 && (_timeout = atoi(argv[1])) != 0)
    return;

  for(unsigned i = 1; i < (unsigned)argc; i++)
    {
      if(std::string("-t") == argv[i] || std::string("--timeout") == argv[i])
	{
	  if(i == (unsigned)(argc - 1))
	    {
	      std::cerr << "Command line error: no timeout specified!!" << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	  i++;
	  if((_timeout = atoi(argv[i])) == 0)
	    {
	      std::cerr << "Command line error: bad timeout!!" << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }	  
	}
      else if(std::string("-d") == argv[i] || std::string("--decide-limit") == argv[i])
	{
	  if(i == (unsigned)(argc - 1))
	    {
	      std::cerr << "Command line error: no decide limit specified!!" << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	  i++;
	  if((_decide_limit = atoi(argv[i])) == 0)
	    {
	      std::cerr << "Command line error: bad decide limit!!" << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }	  
	}
      else if(std::string("-c") == argv[i] || std::string("--config") == argv[i])
	{
	  if(i == (unsigned)(argc - 1))
	    {
	      std::cerr << "Command line error: no config file specified!!" << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	  i++;
	  _config_file.open(argv[i]);
	  if(_config_file.fail())
	    {
	      std::cout << "Cannot open config file " << argv[i] << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	}
      else if(std::string("-h") == argv[i] || std::string("--help") == argv[i])
	{
	  print_usage_message(argv[0]);
	  exit(1);
	}
      else if(std::string("-v") == argv[i] || std::string("--version") == argv[i])
	{
	  std::cerr << PACKAGE_NAME << "-" << PACKAGE_VERSION << std::endl;
	  std::cerr << "Copyright (C) 2008-2015 Milan Bankovic (" << PACKAGE_BUGREPORT << ")" << std::endl;
	  std::cerr << "This is free software; see the source for copying conditions.  There is NO\n"
	    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
	  exit(1);
	}
      else if(std::string("-l") == argv[i] || std::string("--list-config-options") == argv[i])
	{
	  solver_configuration::print_option_list(std::cerr);
	  exit(1);
	}
      else if(_input == &std::cin)
	{
	  _input_file.open(argv[i]);
	  if(_input_file.fail())
	    {
	      std::cout << "Cannot open input file " << argv[i] << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	  else
	    {
	      _input = &_input_file;
	    }
	}
      else if(!_model_out.is_open())
	{
	  _model_out.open(argv[i]);
	  if(_model_out.fail())
	    {
	      std::cout << "Cannot open output file " << argv[i] << std::endl;
	      print_usage_message(argv[0]);
	      exit(1);
	    }
	}      
    }
  if(_input == &std::cin)
    std::cin.sync_with_stdio(false);
}
