/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _TIMEOUT_H
#define _TIMEOUT_H

#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

struct default_handler {

  void operator () (unsigned seconds)
  {
    std::cout << "Timeout after: " << seconds << " seconds!" << std::endl;
    _exit(1);
  }

};

template <typename T = default_handler>
class timeout {
private:
  static T _t;
  static unsigned _seconds;

  static void signal_handler(int sig)
  {
    if(sig == SIGALRM)
      {
	_t(_seconds);
      }
  }

public:
  static void set_handler_function(T & t)
  {
    _t = t;
  }

  static void init_handler()
  {
    struct sigaction sigact;
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGALRM, &sigact, NULL);
  }

  static void set_timeout(unsigned seconds)
  {
    _seconds = seconds;
    alarm(seconds);
  }
 
  static unsigned reset_timeout()
  {
    _seconds = 0;
    return alarm(0);
  }
};


template <typename T> 
T timeout<T>::_t = T();

template<typename T>
unsigned timeout<T>::_seconds = 0;

#endif // _TIMEOUT_H
