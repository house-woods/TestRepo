// $Id: Time_i.cpp 77003 2007-02-12 09:23:36Z johnnyw $

#include "Time_i.h"
#include "ace/OS_NS_time.h"

ACE_RCSID(Time, Time_i, "$Id: Time_i.cpp 77003 2007-02-12 09:23:36Z johnnyw $")

// Constructor
Time_i::Time_i (void)
{
  // no-op
}

// Destructor

Time_i::~Time_i (void)
{
  // no-op
}

// Set the ORB pointer.

void
Time_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Return the current date/time on the server.

CORBA::Long
Time_i::current_time (void)
{
  return CORBA::Long (ACE_OS::time (0));
}

// Shutdown.

void
Time_i::shutdown ( )
{
  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
              "Time_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

