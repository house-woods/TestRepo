#include "EmsLock.h"
#include "BaseException.h"
#include "sysErrLog.h"
#include "TL1ConnectionCountSemaphore.h"
#include "ace/Synch.h"

extern const char* sourceName;

using namespace EMS_FW;
using namespace ELogger;

static const char *className = (const char *)"TL1ConnectionCountSemaphore";

TL1ConnectionCountSemaphore::TL1ConnectionCountSemaphore ()
{
    _minNumberOfResources = 0;
    _maxNumberOfResources = 1;
    _numberOfResources = 0;
    _postState = false;
}

TL1ConnectionCountSemaphore::TL1ConnectionCountSemaphore (int min, int max)
{
    _minNumberOfResources = min;
    _maxNumberOfResources = max;
    _numberOfResources = 0;
    _postState = false;
}

TL1ConnectionCountSemaphore::~TL1ConnectionCountSemaphore ()
{}

void TL1ConnectionCountSemaphore::wait ()
{
    _notify.wait ();
}

int TL1ConnectionCountSemaphore::reset ()
{
    MessageLock lock(_resourceMutex);
    int retVal = 0;

    // if no resources are available,  make all future waiting threads block
    if (_numberOfResources > 0)
    {
        retVal = --_numberOfResources;
        if (_numberOfResources == 0)
        {
            _postState = false;
            _notify.reset();
        }
    }

    return retVal;
}

int TL1ConnectionCountSemaphore::post ()
{
    // if make all waiting threads un-block if minimum resources are available
    MessageLock lock(_resourceMutex);
    int retVal = 0;

    if (_numberOfResources < _maxNumberOfResources)
    {
        retVal = ++_numberOfResources;
        if (_numberOfResources == _minNumberOfResources)
        {
            _postState = true;
            _notify.signal();
        }
    }
    
    return retVal;
}

bool TL1ConnectionCountSemaphore::posted ()
{
    return _postState;
}

void TL1ConnectionCountSemaphore::setMin(int min)
{
    MessageLock lock (_resourceMutex);
    _minNumberOfResources = min;
}

void TL1ConnectionCountSemaphore::setMax(int max)
{
    MessageLock lock (_resourceMutex);
    _maxNumberOfResources = max;
}
