#ifndef _DEFS_H_
#define _DEFS_H_

// Global values
const int NR_OF_GLOBALVALUES 						= 11;

const int POS_OF_TOUT 									= 0;
const int POS_OF_CP											= 1;
const int POS_OF_T_AMBIENT							= 2;
const int POS_OF_GRAVITY								= 3;
const int POS_OF_WATER_DENSITY					= 4;
const int POS_OF_KINEMATIC_VISCOSITY		= 5;
const int POS_OF_PI											= 6;
const int POS_OF_PIPE_ROUGHNESS					= 7;
const int POS_OF_TOTAL_FLOW							= 8;
const int POS_OF_TIN_OFFSET							= 9;

///////////////////////////////
// Values for the Component matrix
///////////////////////////////
const int NR_OF_COMPONENTVALUES 				= 18;

const int TYPE_TOUT											= 100;
const int TYPE_PRODUCER_BOUNDARY				= 101;
const int TYPE_CONSUMER_BOUNDARY				= 102;
const int TYPE_PIPE											= 103;
const int TYPE_PUMP											= 104;
const int TYPE_VALVE										= 105;

const int POS_OF_ID											= 0;
const int POS_OF_TYPE										= 1;
const int POS_OF_BEGIN									= 2;
const int POS_OF_END										= 3;

// producer positions (boundaries)
const int PRODUCER_POS_OF_ID						= POS_OF_ID;
const int PRODUCER_POS_OF_TYPE					= POS_OF_TYPE;
const int PRODUCER_POS_OF_BEGIN					= POS_OF_BEGIN;
const int PRODUCER_POS_OF_END						= POS_OF_END;
const int PRODUCER_POS_OF_HEAD					= 4;
const int PRODUCER_POS_OF_TFOR_LOW			= 5;
const int PRODUCER_POS_OF_TOUT_LOW			= 6;
const int PRODUCER_POS_OF_TFOR_HIGH			= 7;
const int PRODUCER_POS_OF_TOUT_HIGH			= 8;
const int PRODUCER_POS_OF_TFOR					= 9;
const int PRODUCER_POS_OF_TRET					= 10;
const int PRODUCER_POS_OF_FLOW					= 11;

// pipe positions
const int PIPE_POS_OF_ID								= POS_OF_ID;
const int PIPE_POS_OF_TYPE							= POS_OF_TYPE;
const int PIPE_POS_OF_BEGIN							= POS_OF_BEGIN;
const int PIPE_POS_OF_END								= POS_OF_END;
const int PIPE_POS_OF_LENGTH						= 4;
const int PIPE_POS_OF_INNER_D						= 5;
const int PIPE_POS_OF_THICKNESS					= 6;
const int PIPE_POS_OF_KVALUE						= 7;
const int PIPE_POS_OF_FLOW							= 8;
const int PIPE_POS_OF_HEADLOSS					= 9;
const int PIPE_POS_OF_T_START						= 10;
const int PIPE_POS_OF_T_END							= 11;

// consumer positions (boundaries)
const int CONSUMER_POS_OF_ID						= POS_OF_ID;
const int CONSUMER_POS_OF_TYPE					= POS_OF_TYPE;
const int CONSUMER_POS_OF_BEGIN					= POS_OF_BEGIN;
const int CONSUMER_POS_OF_END						= POS_OF_END;
const int CONSUMER_POS_OF_ESIG					= 4;
const int CONSUMER_POS_OF_TCONST				= 5;
const int CONSUMER_POS_OF_TIN_WANTED		= 6;
const int CONSUMER_POS_OF_DT_PRIM				= 7;
const int CONSUMER_POS_OF_DIM_TOUT			= 8;
const int CONSUMER_POS_OF_FLOW					= 9;
const int CONSUMER_POS_OF_FTEMP					= 10;
const int CONSUMER_POS_OF_RTEMP					= 11;
const int CONSUMER_POS_OF_HEAD					= 12;
const int CONSUMER_POS_OF_HEATLOAD			= 13;
const int CONSUMER_POS_OF_LOADCONTROL		= 14;
const int CONSUMER_POS_OF_HEATLOAD_QUOTA= 15;
const int CONSUMER_POS_OF_HL_STOCH			= 16;
const int CONSUMER_POS_OF_TIN_ACTUAL		= 17;

// event types
const int EVENT_TOUT										= 0;
const int EVENT_AMBIENT									= 1;
const int EVENT_LOADCONTROL							= 2;

#endif 