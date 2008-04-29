/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_NCS_MESSAGES_H
#define __SHAWN_APPS_LOCALIZATION_MESSAGES_DISTANCE_GPSFREE_NCS_MESSAGES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/node.h"
#include "sys/message.h"
#include "apps/localization/util/localization_defutils.h"
#include "apps/localization/neighborhood/localization_local_coordinate_system.h"


namespace localization
{

   class LocalizationGPSfreeNCSLCSMessage
      : public shawn::Message
   {

   public:

      LocalizationGPSfreeNCSLCSMessage( const LocalizationLocalCoordinateSystem& );
      LocalizationGPSfreeNCSLCSMessage( const LocalizationGPSfreeNCSLCSMessage& );
      virtual ~LocalizationGPSfreeNCSLCSMessage();

      ///
      const LocalizationLocalCoordinateSystem& local_coord_sys( void ) const throw();


   private:

      const LocalizationLocalCoordinateSystem *local_coord_sys_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/messages/position/localization_gpsfree_ncs_messages.h,v $
 * Version $Revision$
 * Date    $Date$
 *-----------------------------------------------------------------------
 * $Log: localization_gpsfree_ncs_messages.h,v $
 *-----------------------------------------------------------------------*/
