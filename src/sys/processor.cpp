/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include "sys/processor.h"
#include "sys/node.h"

using namespace std;

namespace shawn
{
   Processor::
   Processor()
      : owner_ ( NULL ),
        state_ ( Active )
   {}
   // ----------------------------------------------------------------------
   Processor::
   ~Processor()
   {}
   // ----------------------------------------------------------------------
   void
   Processor::
   set_owner( Node& n )
      throw()
   {
      owner_ = &n;
   }
   // ----------------------------------------------------------------------
   const Node&
   Processor::
   owner( void )
      const throw()
   {
      assert( owner_ != NULL );
      return *owner_;
   }
   // ----------------------------------------------------------------------
   Node&
   Processor::
   owner_w( void )
      throw()
   {
      assert( owner_ != NULL );
      return *owner_;
   }
   // ----------------------------------------------------------------------
   void
   Processor::
   special_boot( void )
      throw()
   {
      //cout << "BOOT_NODE id=" << owner().id() << endl;
   }
   // ----------------------------------------------------------------------
   void
   Processor::
   boot( void )
      throw()
   {
      //cout << "BOOT_NODE id=" << owner().id() << endl;
   }
   // ----------------------------------------------------------------------
   bool
   Processor::
   process_message( const ConstMessageHandle& )
      throw()
   { return false; }
   // ----------------------------------------------------------------------
   void
   Processor::
   work( void )
      throw()
   {
      //cout << "WORK id=" << owner().id() << endl;
   }
   // ----------------------------------------------------------------------
   void
   Processor::
   send( const MessageHandle& mh )
      throw()
   {
      owner_w().send( mh );
   }
   // ----------------------------------------------------------------------
   void
   Processor::
   set_state( const Processor::ProcessorState& ps )
      throw()
   {
      state_ = ps;
   }
   // ----------------------------------------------------------------------
   Processor::ProcessorState
   Processor::
   state( void )
      const throw()
   {
      return state_;
   }
   // ----------------------------------------------------------------------
   int
   Processor::
   simulation_round( void )
      const throw()
   {
      return owner().simulation_round();
   }
   // ----------------------------------------------------------------------
   int
   Processor::
   id( void )
      const throw()
   {
      return owner().id();
   }
   // ----------------------------------------------------------------------
   void
   Processor::
   sync_to_tags( TagContainer& )
      throw()
   { ABORT_NOT_IMPLEMENTED; }
   // ----------------------------------------------------------------------
   void
   Processor::
   sync_from_tags( const TagContainer& )
      throw( std::runtime_error )
   { ABORT_NOT_IMPLEMENTED; }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processor.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:36 $
 *-----------------------------------------------------------------------
 * $Log: processor.cpp,v $
 *-----------------------------------------------------------------------*/
