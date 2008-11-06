/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_H
#define __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_H

#include "_apps_enable_cmake.h"
//#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/misc/node_change_listener.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/processors/processor_factory.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/vec.h"
#include "apps/tcpip/socket.h"
#include "apps/tcpip/storage.h"
#include "mobility_interface_constants.h"
#include <string>
#include <list>
#include <map>
#ifdef ENABLE_TRACICLIENT

namespace traci
{
	class TraCINodeMovementCreator :
		public shawn::NodeMovementCreator,
		public shawn::SimulationTask,
		public shawn::NodeChangeListener
	{
	public:
		TraCINodeMovementCreator(shawn::SimulationController& sc);
		~TraCINodeMovementCreator();

		std::string name( void ) const throw();

		std::string description( void ) const throw();

		shawn::MovementInfo* next_movement(void) throw( std::runtime_error );

		void run(shawn::SimulationController&) throw( std::runtime_error );
	
		void reset();

		/// Is called whenever a node gets added to the world
		virtual void node_added( shawn::Node& ) throw();
		/// Is called whenever a node gets removed from the world
		virtual void node_removed( shawn::Node& ) throw();
		/** Is called whenever the id of a node has changed. This occurs, for
         *  example, if the method reorganize_nodes() of the world is called,
         *  because the internal NodeVector is optimized (compressed) there
         *  and ids may change.
         *
         *  \param int Old ID
         *  \param int New ID
         */
		virtual void id_changed( int, int ) throw();
		/// Is called when the world gets deleted.
		/** \return true, if the world should delete the listener */
		virtual bool invalidate( void ) throw();

		struct TraCIID{
			TraCIID();
			TraCIID(int d,int i);
			TraCIID(const TraCIID&);
			virtual ~TraCIID();
			int domain() const;
			int id() const;
			traci::TraCINodeMovementCreator::TraCIID& operator=(const traci::TraCINodeMovementCreator::TraCIID& rv);
		private:
			//friend class traci::TraCINodeMovementCreator;
			friend int operator<(const traci::TraCINodeMovementCreator::TraCIID&, const traci::TraCINodeMovementCreator::TraCIID&);
			friend int operator>(const traci::TraCINodeMovementCreator::TraCIID&, const traci::TraCINodeMovementCreator::TraCIID&);
			friend int operator==(const traci::TraCINodeMovementCreator::TraCIID&, const traci::TraCINodeMovementCreator::TraCIID&);
			
			int domain_;
			int id_;
		};

		// Interface for listeners
		class TraCINodeChangeListener{
		public:
			virtual ~TraCINodeChangeListener(){}
			virtual void node_added( shawn::Node& node, const traci::TraCINodeMovementCreator::TraCIID& traci_id ) throw() = 0;
			virtual void node_removed( shawn::Node& node, const traci::TraCINodeMovementCreator::TraCIID& traci_id ) throw() = 0;
		protected:
			TraCINodeChangeListener(){}
		private:
			TraCINodeChangeListener(const TraCINodeChangeListener&);
		};

		struct TLSwitchInfo{
			double switchTime;
			std::string fromEdge;
			float fromEdgePos;
			std::string toEdge;
			int newStatus;
			double yellowTime;
		};
		typedef std::vector<TLSwitchInfo> TLSwitchInfoVec;

		void command_set_maximum_speed(const shawn::Node& node, double max_speed);
		void command_stop(const shawn::Node& node, double x, double y, double radius, double wait_time);
		void command_change_route(const shawn::Node& node, std::string road_id, double travel_time);
		void command_change_lane(const shawn::Node& node, char lane, float time);
		bool command_roadmap_position(const shawn::Node& node, double targetTime, std::string& roadId_, float& relPosition_, std::string& laneID_);
		bool command_scenario_integer(bool write, int domain, int domainId, int variable, int& value);
		bool command_scenario_string(bool write, int domain, int domainId, int variable, std::string& value);
		bool command_scenario_pos3d(bool write, int domain, int domainId, int variable, float& x, float& y, float& z);
		bool command_scenario_bounding_box(bool write, int domain, int domainId, int variable, float& x1, float& y1, float& x2, float& y2);
		float command_scenario_distance_to_roadmap_position(bool write, int domain, int domainId, int variable, std::string roadId, float roadPos, int lane);
		bool command_get_TL_status(int tlId, double from, double until, TLSwitchInfoVec& switchInfos);

	public:

		const shawn::Node* find_node_by_traci_id(const TraCIID& traci_id);
		shawn::Node* find_node_by_traci_id_w(const TraCIID& traci_id);
		const TraCIID& find_traci_id_by_node(const shawn::Node& node) throw(std::logic_error);

		shawn::Node* new_node(const TraCIID& traci_id, bool attach_processors_by_domain = true);

		void add_traffic_lights();

		void add_node_change_listener( traci::TraCINodeMovementCreator::TraCINodeChangeListener& listener ) throw();

	private:

		// Reads a Status command that is expected to start at s' actual position.
        // If the status is unequal to RTYPE_OK or the given commandId differs from
        // that in the Storage, Shawn will stop.
		void extract_command_status(tcpip::Storage& s, unsigned char commandId, std::string& description);

		// Connect and disconnect to the mobility server         
		// The connect method is called by startSimStepHandler         
		// The close method is called by the destructor
		bool connect();
		void close();

		void fetch_processor_factories();

		bool check_domain(int domain, const TraCIID& traci_id, bool abrt = false, std::string err_msg = "");

		//void switch_processors_state(int id = -1, int state = 0);

		//bool all_processors_state(int node_id, int state);

		void check_for_unused_vehicle_nodes();

		//void increase_nodes(int to);

		void add_node(shawn::Node* node, const TraCIID& traci_id);
		void remove_node_from_map(shawn::Node* node);

		// Internal variables
		shawn::SimulationController& sc_;
		tcpip::Socket* socket_;
		tcpip::Storage in_;
		std::string remotehost_;
		int remoteport_;
		double time_interval_;
		double target_time_;
		double feed_time_;
		shawn::MovementInfo *mi_;
		// Every domain has it's associated processor factories map<domain,processor factories>
		std::map<int,std::set<shawn::ProcessorFactoryHandle>* > domain_processor_factories_;
		// Current nodes of the step
		std::set<TraCIID> current_vehicle_nodes_;
		// Two-way "node id" mapping from TraCI to shawn
		std::map<shawn::Node*,TraCIID> node_to_traci_ids_;
		std::map<TraCIID,shawn::Node*> traci_ids_to_node_;

		typedef std::list<traci::TraCINodeMovementCreator::TraCINodeChangeListener*> TraCINodeChangeListenerList;
		TraCINodeChangeListenerList node_change_listeners_;

		static const int DOMAIN_IDS[];
		static const std::string DOMAIN_NAMES[];

   };
}
#endif
#endif
