/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/modules/position/localization_lateration_module.h"
#include "apps/localization/math/localization_triangulation.h"
#include <fstream>

namespace localization
{

   LocalizationLaterationModule::
   LocalizationLaterationModule()
      : state_( lat_wait )
   {}
   // ----------------------------------------------------------------------
   LocalizationLaterationModule::
   ~LocalizationLaterationModule()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationLaterationModule::
   work( void )
      throw()
   {
      if ( state_ == lat_finished )
         return;

      if ( state_ == lat_wait )
         state_ = lat_work;

      shawn::Vec est_pos;
      NeighborInfoList neighbors;
      collect_neighbors( neighborhood(), lat_anchors, neighbors );
      if(neighbors.empty())
      {
          std::cout << " LocalizationLaterationModule: No neighbors found" <<std::endl;
          return;
      }
      assert(!neighbors.empty());
      // Following idea is from K. Langendoen and N. Reijers in their
      // implementation of ad-hoc positioning using OMNeT++.
      // See also:
      //    http://www.consensus.tudelft.nl/software.html
      //
      // Execute lateration two times. First, to simply get the
      // estimated position. Second, use the estimated position
      // in the lateration phase as new parameter in least squares
      // approach. Particularly if you know only 3 anchors, the new
      // parameter in the second pass is very useful.
	 // if(node().id() == 13){
//	 std::ofstream ofs( "distances.csv",std::ios::app );
	  ConstNeighborInfoListIterator iter = neighbors.begin();
	  for(; iter!=neighbors.end(); iter++){
//		  const shawn::Node* n = &(*iter)->node();

	/*	  ofs<< "Distance from Node" <<node().id() << "to Anker"<< n->label() << " real distance:"<<  (node().real_position()-n->real_position()).euclidean_norm()<<
			  " estimated distance:" << (*iter)->distance()<< std::endl;
*/
	//	ofs<< std::endl;
	  //}
	  }
      if ( est_pos_lateration( neighbors, est_pos, lat_anchors, false ) &&
           est_pos_lateration( neighbors, est_pos, lat_anchors, true ) )
      {
         if ( !observer().check_residue() || check_residue( neighbors, est_pos, lat_anchors, observer().comm_range() ) )
            node_w().set_est_position( est_pos );
      }

      state_ = lat_finished;
   }
   // ----------------------------------------------------------------------
   bool
   LocalizationLaterationModule::
   finished( void )
      throw()
   {
      return state_ == lat_finished;
   }

	void  LocalizationLaterationModule::
		rollback(void)
		throw()
	{
		state_ = lat_wait;
	}

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/modules/position/localization_lateration_module.cpp,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: localization_lateration_module.cpp,v $
 *-----------------------------------------------------------------------*/
