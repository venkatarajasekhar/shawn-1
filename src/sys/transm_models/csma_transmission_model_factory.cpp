/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <assert.h>
#include "sys/transm_models/csma_transmission_model.h"
#include "sys/transm_models/csma_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   CsmaTransmissionModelFactory::
   CsmaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   CsmaTransmissionModelFactory::
   ~CsmaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   CsmaTransmissionModelFactory::
   name( void )
      const throw()
   {
      return std::string("csma");
   }
   // ----------------------------------------------------------------------
   std::string
   CsmaTransmissionModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A CsmaTransmissionModel (\"csma\") sense the carrier before it delivers messages");
   }
   // ----------------------------------------------------------------------
   TransmissionModel*
   CsmaTransmissionModelFactory::
   create( const SimulationController& sc)
      throw()
   {	
	
		int bandwidth =sc.environment().required_int_param( "bandwidth");
		double backoff = sc.environment().required_double_param( "backoff");
		double sending_jitter = sc.environment().optional_double_param( "jitter",0.0 );
		sending_jitter = sc.environment().optional_double_param( "jitter_upper_bound",sending_jitter );
		double sending_jitter_lb = sc.environment().optional_double_param( "jitter_lower_bound",0.001 );
		//if (sending_jitter > 0.0) assert(sending_jitter >= sending_jitter_lb); 
		int max_sending_attempts =  sc.environment().optional_int_param("max_sending_attempts",5);
		int backoff_factor_base =  sc.environment().optional_int_param("backoff_factor_base",1);
		return new CsmaTransmissionModel(bandwidth,backoff,sending_jitter,sending_jitter_lb,max_sending_attempts,backoff_factor_base);
   }
	

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model_factory.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: csma_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
