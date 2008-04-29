/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_RECTANGLE_TOPOLOGY_H
#define __SHAWN_APPS_TOPOLOGY_RECTANGLE_TOPOLOGY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/topology_2d.h"
#include "sys/misc/box.h"

#include <string>


namespace topology
{

   class RectangleTopology
      : public Topology2D
   {
   public:
      RectangleTopology( const shawn::Box&,
                         const std::string& name = "" );
      virtual ~RectangleTopology();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual bool value( const shawn::Vec& ) const throw();
      virtual shawn::Box domain( void ) const throw();

   private:
      shawn::Box  box_;
      std::string name_;
      std::string description_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/rectangle_topology.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: rectangle_topology.h,v $
 *-----------------------------------------------------------------------*/
