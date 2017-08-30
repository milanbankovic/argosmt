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
#include "argosmt_solver_interface.hpp"
#include "timeout.hpp"
#include "conditional_tbb_wrapper.hpp"
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
#ifdef _USE_TBB
#ifdef _PARALLEL_PORTFOLIO
  tbb::task_scheduler_init ts(std::max(s_config.number_of_threads(),
				       s_config.number_of_solvers()));
#else
  tbb::task_scheduler_init ts(s_config.number_of_threads());
#endif
#endif
  
  smtlib_api smt_lib_api;
#ifdef _PARALLEL_PORTFOLIO
  argosmt_solver_interface sl(s_config.number_of_solvers());
#else
  argosmt_solver_interface sl;
#endif
  smt_lib_api.set_solver_interface(&sl);

  if(cmd_line_parser::timeout() != 0)
    {
      timeout<>::init_handler();
      timeout<>::set_timeout(cmd_line_parser::timeout());
    }

  cmd_line_parser::input() >> smt_lib_api;
  
  attribute_value * avalue = smt_lib_api.get_info(keyword(":all-statistics"));

  if(avalue)
    {
      avalue->print_value(std::cout);
      std::cout << std::endl;
    }
  
  sl.print_reports(std::cout);
  
  return 0;
}
