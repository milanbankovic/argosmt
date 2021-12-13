/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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
#include "timeout.hpp"
#include "dimacs_solver_interface.hpp"
#include "solver_configuration.hpp"

solver_configuration s_config;

int main(int argc, char ** argv)
{
  cmd_line_parser::parse_cmd_line(argc, argv);
  if(cmd_line_parser::has_config())
    {
      s_config.parse_config_file(cmd_line_parser::config_file());
      cmd_line_parser::close_config();
    }
  
  dimacs_solver_interface dsi;

  if(cmd_line_parser::timeout() != 0)
    {
      timeout<>::init_handler();
      timeout<>::set_timeout(cmd_line_parser::timeout());
    }

  dsi.check_sat();
  
  return 0;
}
