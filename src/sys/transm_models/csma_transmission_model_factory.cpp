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
#include <string>
#include <iostream>

using namespace std;

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
		int bandwidth;

		double short_inter_frame_spacing;
		double long_inter_frame_spacing;
		int max_short_inter_frame_spacing_size;
		   
		double backoff;
		double sending_jitter;
		sending_jitter;
		double sending_jitter_lb;
		int max_sending_attempts;
		int backoff_factor_base;
		int min_backoff_exponent;
		int max_backoff_exponent;
		
		string type = sc.environment().optional_string_param("csma_type", "");
		if( type == "802.15.4" )
		{
			cout << "CSMA transmission model factory: Setting parameters to fit IEEE 802.15.4" << endl;
			bandwidth =16000;

			short_inter_frame_spacing = 12.0/double(bandwidth); // 12 symbols
			long_inter_frame_spacing = 40.0/double(bandwidth); // 40 symbols
			max_short_inter_frame_spacing_size = 18;

			   
			backoff = 20.0/double(bandwidth); //20 symbols
			sending_jitter = backoff;
			sending_jitter_lb = 0;
			max_sending_attempts = 5;
			backoff_factor_base = 2;
			min_backoff_exponent = 3;
			max_backoff_exponent = 5;
		} else {
			bandwidth =sc.environment().required_int_param( "bandwidth");

			short_inter_frame_spacing = sc.environment().optional_double_param( "short_inter_frame_spacing",12.0/double(bandwidth) ); // 12 symbols
			long_inter_frame_spacing = sc.environment().optional_double_param( "long_inter_frame_spacing",40.0/double(bandwidth) ); // 40 symbols
			max_short_inter_frame_spacing_size = sc.environment().optional_int_param( "max_short_inter_frame_spacing_size",18 );

			backoff = sc.environment().required_double_param( "backoff");
			sending_jitter = sc.environment().optional_double_param( "jitter",0.0 );
			sending_jitter = sc.environment().optional_double_param( "jitter_upper_bound",sending_jitter );
			sending_jitter_lb = sc.environment().optional_double_param( "jitter_lower_bound",0.001 );
			max_sending_attempts =  sc.environment().optional_int_param("max_sending_attempts",3);
			backoff_factor_base =  sc.environment().optional_int_param("backoff_factor_base",2);
			min_backoff_exponent = sc.environment().optional_int_param("min_backoff_exponent",0);
			max_backoff_exponent = sc.environment().optional_int_param("max_backoff_exponent",100);
		}
		return new CsmaTransmissionModel( short_inter_frame_spacing, long_inter_frame_spacing, max_short_inter_frame_spacing_size, bandwidth,backoff,sending_jitter,sending_jitter_lb,max_sending_attempts,backoff_factor_base, min_backoff_exponent, max_backoff_exponent);
   }
	

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model_factory.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: csma_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
