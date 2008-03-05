/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "AutoCast_init.h"
#ifdef ENABLE_AUTOCAST

#include "apps/AutoCast/AutoCast/AutoCast_init.h"
#include "apps/AutoCast/AutoCast/AutoCast_processor_factory.h"
#include "sys/simulation/simulation_controller.h"

extern "C" void init_autocast( shawn::SimulationController& sc )
{
   std::cout << "AutoCast init" << std::endl;
   autocast::AutoCastProcessorFactory::register_factory(sc);
}

#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/