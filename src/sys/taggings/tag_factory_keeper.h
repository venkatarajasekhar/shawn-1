/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_TAG_FACTORY_KEEPER_H
#define __SHAWN_SYS_TAGGINGS_TAG_FACTORY_KEEPER_H

#include "sys/util/handle_keeper.h"
#include "sys/taggings/tag_factory.h"

namespace shawn
{

   class TagFactoryKeeper
      : public HandleKeeper<TagFactory>
   {
   public:
      TagFactoryKeeper();
      virtual ~TagFactoryKeeper();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/tag_factory_keeper.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2005/08/05 10:00:42 $
 *-----------------------------------------------------------------------
 * $Log: tag_factory_keeper.h,v $
 *-----------------------------------------------------------------------*/
