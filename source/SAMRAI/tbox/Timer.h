/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   Timer class to track elapsed time in portions of a program. 
 *
 ************************************************************************/

#ifndef included_tbox_Timer
#define included_tbox_Timer

#include "SAMRAI/SAMRAI_config.h"
#include "SAMRAI/tbox/Clock.h"
#include "SAMRAI/tbox/Database.h"
#include "SAMRAI/tbox/PIO.h"
#include "SAMRAI/tbox/Pointer.h"

#include <string>
#include <vector>

#ifdef HAVE_TAU
#if (PROFILING_ON || TRACING_ON)
#include <Profile/Profiler.h>
#endif
#endif

namespace SAMRAI {
namespace tbox {

/**
 * Class Timer holds the exclusive and total start, stop, and elapsed
 * time for timers instrumented in SAMRAI.  Total time is simply the time
 * between calls to the start() and stop() functions.  Exclusive time is
 * applicable if there are nested timers called.
 *
 * System and user start and end times are stored as variables of type
 * clock_t, defined in the sys/times.h include file.  A detailed explanation
 * of the structures used to store system and user times is given in the
 * header for the Clock class. This routine simply accesses the functions
 * specified in that class.
 *
 * Wallclock time may be computed by the systems internal clocks which require
 * an object of type clock_t, or by SAMRAI_MPI::Wtime() if the code is linked to MPI
 * libraries.
 *
 * In addition to running or not running, a timer may be active or inactive.
 * An inactive timer is one that is created within a program but will never
 * be turned on or off because it is either not specified as active in
 * an input file or it was not explicitly made active by the user.  When
 * a timer is created, it is active by default.
 *
 * Note that the constructor is protected so that timer objects can only
 * be created by the TimerManager class.
 *
 * @see tbox::TimerManager
 */

class TimerManager;

class Timer:public DescribedClass
{
   friend class TimerManager;
public:
   /**
    * Empty virtual destructor for Timer class.
    */
   virtual ~Timer();

   /**
    * Return string name for timer.
    */
   const std::string&
   getName() const;

   /**
    * Start the timer if active.
    */
   void
   start();

   /**
    * Stop the timer if active.
    */
   void
   stop();

   /**
    * If active, SAMRAI_MPI::getSAMRAIWorld().Barrier() then start the timer.
    */
   void
   barrierAndStart();

   /**
    * If active, SAMRAI_MPI::getSAMRAIWorld().Barrier() then stop the timer.
    */
   void
   barrierAndStop();

   /**
    * Start exclusive time.
    */
   void
   startExclusive();

   /**
    * Stop exclusive time.
    */
   void
   stopExclusive();

   /**
    * Reset the state of the timing information.
    */
   void
   reset();

   /**
    * Return total system time (between starts and stops)
    */
   double
   getTotalSystemTime() const;

   /**
    * Return total user time
    */
   double
   getTotalUserTime() const;

   /**
    * Return total wallclock time
    */
   double
   getTotalWallclockTime() const;

   /**
    * Return max wallclock time
    */
   double
   getMaxWallclockTime() const;

   /**
    * Return exclusive system time.
    */
   double
   getExclusiveSystemTime() const;

   /**
    * Return exclusive user time.
    */
   double
   getExclusiveUserTime() const;

   /**
    * Return exclusive wallclock time.
    */
   double
   getExclusiveWallclockTime() const;

   /**
    * Return true if the timer is active; false otherwise.
    */
   bool
   isActive() const;

   /**
    * Return true if timer is running; false otherwise.
    */
   bool
   isRunning() const;

   /**
    * Return number of accesses to start()-stop() functions for the
    * timer.
    */
   int
   getNumberAccesses() const;

   /**
    * Compute load balance efficiency based on wallclock (non-exclusive)
    * time.
    */
   double
   computeLoadBalanceEfficiency();

   /**
    * Compute max wallclock time based on total (non-exclusive) time.
    */
   void
   computeMaxWallclock();

   /**
    * Write timer data members to database.
    */
   virtual void
   putToDatabase(
      Pointer<Database> db);

   /**
    * Read restarted times from restart database.  When assertion checking
    * is on, the database pointer must be non-null.
    */
   virtual void
   getFromRestart(
      Pointer<Database> db);

protected:
   /**
    * The constructor for the Timer class sets timer name string
    * and integer identifiers, and initializes the timer state.
    */
   Timer(
      const std::string& name);

   /*
    * Set this timer object to be a active or inactive.
    */
   void
   setActive(
      bool is_active);

   /**
    * Add Timer that running concurrently with this one.
    */
   void
   addConcurrentTimer(
      const Timer& timer);

   /**
    * Return if the timer is running concurrently with this one.
    */
   bool
   isConcurrentTimer(
      const Timer& timer) const;

private:
   /*
    * Class name, id, and concurrent timer flag.
    */
   std::string d_name;
   std::vector<const Timer *> d_concurrent_timers;

   bool d_is_running;
   bool d_is_active;

   /*
    *  Total times (non-exclusive)
    */
   double d_user_total;
   double d_system_total;
   double d_wallclock_total;

   /*
    *  Exclusive times
    */
   double d_user_exclusive;
   double d_system_exclusive;
   double d_wallclock_exclusive;

   /*
    *  Cross processor times (i.e. determined across processors)
    */
   double d_max_wallclock;

   /*
    *  Timestamps.  User and system times are stored as type clock_t.
    *  Wallclock time is also stored as clock_t unless the library has
    * been compiled with MPI.  In this case, the wall time is stored
    * as type double.
    */
   clock_t d_user_start_total;
   clock_t d_user_stop_total;
   clock_t d_system_start_total;
   clock_t d_system_stop_total;
   clock_t d_user_start_exclusive;
   clock_t d_user_stop_exclusive;
   clock_t d_system_start_exclusive;
   clock_t d_system_stop_exclusive;
   double d_wallclock_start_total;
   double d_wallclock_stop_total;
   double d_wallclock_start_exclusive;
   double d_wallclock_stop_exclusive;

   /*
    * Counter of number of times timers start/stop
    * are accessed.
    */
   int d_accesses;

   static const int DEFAULT_NUMBER_OF_TIMERS_INCREMENT;

   /*
    * Static integer constant describing this class's version number.
    */
   static const int TBOX_TIMER_VERSION;

   /*
    * Objects used for performance analysis with TAU.  The "tautimer" mapping
    * object is a tau timer that is associated with this SAMRAI timer.
    */
#ifdef HAVE_TAU
   TAU_MAPPING_OBJECT(tautimer)
#endif

};

}
}

#ifdef SAMRAI_INLINE
#include "SAMRAI/tbox/Timer.I"
#endif
#endif