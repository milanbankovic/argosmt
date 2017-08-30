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

#ifndef _CMD_LINE_PARSER_H
#define _CMD_LINE_PARSER_H

#include <iostream>
#include <fstream>
#include <cstdlib>

class cmd_line_parser {
private:
  static unsigned _timeout;
  static unsigned _decide_limit;
  static std::ifstream _input_file;
  static std::ofstream _model_out;
  static std::istream * _input;
  static std::ifstream _config_file;
  
  static void print_usage_message(const char * prg_name)
  {
    std::cerr << "usage:   " << prg_name << " [timeout] < input_file" << std::endl;
    std::cerr << "\n or:\n\t" << prg_name << " [options] [input_file] [model_out]" << std::endl;

    std::cerr << "\nThe supported options are: "
      "\n\t'-h' (or '--help')"
      "\n\t'-v' (or '--version')"
      "\n\t'-t <seconds>' (or '--timeout <seconds>')"
      "\n\t'-d <num>'  (or '--decide-limit <num>')"
      "\n\t'-c <file>' (or '--config <file>')"
      "\n\t'-l' (or '--list-config-options')"
	      << std::endl;
  }
public:
  static void parse_cmd_line(int argc, char ** argv);
  static std::istream & input()
  {
    return *_input;
  }

  static bool has_model_out()
  {
    return _model_out.is_open();
  }

  static std::ostream & model_out()
  {
    return _model_out;
  }

  static unsigned timeout()
  {
    return _timeout;
  }

  static unsigned decide_limit()
  {
    return _decide_limit;
  }
  
  static std::istream & config_file()
  {
    return _config_file;
  }

  static bool has_config()
  {
    return _config_file.is_open();
  }

  static void close_config()
  {
    _config_file.close();
  }
};

#endif // _CMD_LINE_PARSER_H
