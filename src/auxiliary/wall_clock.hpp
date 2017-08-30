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

#ifndef _WALL_CLOCK_H
#define _WALL_CLOCK_H

#include <ctime>

class wall_clock {
private: 
  struct timespec _start;
  struct timespec _cumulative;
  
  static double time_diff(const struct timespec & ts1, const struct timespec & ts2)
  {
    return (double)(ts1.tv_sec - ts2.tv_sec) + (double)(ts1.tv_nsec - ts2.tv_nsec)/1000000000;
  }

  static double to_seconds(const struct timespec & ts)
  {
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000;
  }

  static void time_diff(const struct timespec & ts1, const struct timespec & ts2, struct timespec & ts)
  {
    ts.tv_nsec = ts1.tv_nsec - ts2.tv_nsec;
    ts.tv_sec = ts1.tv_sec - ts2.tv_sec;
    if(ts.tv_nsec < 0)
      {
	ts.tv_sec--;
	ts.tv_nsec += 1000000000;
      }
  }

  static void time_sum(const struct timespec & ts1, const struct timespec & ts2, struct timespec & ts)
  {
    ts.tv_sec = ts1.tv_sec + ts2.tv_sec;
    ts.tv_nsec = ts1.tv_nsec + ts2.tv_nsec;
    if(ts.tv_nsec >= 1000000000)
      {
	ts.tv_sec++;
	ts.tv_nsec -= 1000000000;
      }
  }

  static void reset(struct timespec & ts)
  {
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
  }
  
public:
  wall_clock()
  {
    reset(_cumulative);
    reset(_start);
  }

  
  void start()
  {
    clock_gettime(CLOCK_REALTIME, &_start);
  }

  void reset_cumulative()
  {
    reset(_cumulative);
  }
  
  void acumulate()
  {
    struct timespec stop;
    clock_gettime(CLOCK_REALTIME, &stop);
    time_diff(stop, _start, stop);
    time_sum(_cumulative, stop, _cumulative);
  }

  double cumulative_time()
  {
    return to_seconds(_cumulative);
  }
  
  double seconds_elapsed()
  {  
    struct timespec stop;
    clock_gettime(CLOCK_REALTIME, &stop);
    return time_diff(stop, _start);
  }
};

#endif // _WALL_CLOCK_H
