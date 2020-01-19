/**********************************************************************

  This files has stuctures,macros,global variabls & etc.. related to 
  1. Cell
  2. UE
  3. Scheduling
  4. Architechture
  

**********************************************************************/

// TODO: these typedefs has to be changed based on system type
typedef int U8;
typedef int U16;
typedef int U32;
typedef int U64;
typedef int Bool;


#define printf PF

#define TRUE 1
#define FALSE 0


/* NUMBER_OF_CELLS for eNB  */
#define NUMBER_OF_CELLS  2


typedef struct _eNb       ENb;
typedef struct _cell      Cell;


typedef enum
{
   BW_1_4,
   BW_3,
   BW_5,
   BW_10,
   BW_15,
   BW_20
}BW;




/*
----------------------------
|    BW       |  RBG size  |
----------------------------
1.4 (6 RB's)  |     1      |
3 (15 RB's)   |     2      |
5 (25 RB's)   |     2      |
10 (50 RB's)  |     3      |
15 (75 RB's)  |     4      |
20 (100 RB's) |     4      |
----------------------------  */
U32 rbg[]            = {1, 2,  2,  3,  4,  4};
U32 numOfRbsPerRbg[] = {6, 8, 13, 17, 19, 25};








/*
   @Brief _eNb
    > _eNb struct is a stucture to store eNodeB related parameters.
    > Initialization will be handled during cell setup or mac configuration.
*/
typedef struct _eNb
{
   Cell*	cells[NUMBER_OF_CELLS];	/* > NUMBER_OF_CELLS being the system constant which 
					     is set as eNB parameter.
	     				     numberOfCells is sent/set from higher layer.
					    while current implementation being static for 3 cells.
					  > common instance for bth DL & UL
					  > Static memory allocation as this stuct will be necessary
					    for mac to work.
					  >
	                	        */
   U8		numOfInitializedCells; /* configured cells based on input from above layers.
   				          TODO:numOfInitializedCells should not be greater than NUMBER_OF_CELLS*/
   Bool         isCellConfPending;	// TRUE if configuration received and cell not initialized.
   Bool		isEpc;	/* is EPC connected to eNb*/


};  /*  _eNb  */




/*
   @Brief _cell
    > _cell structure is a stuctre for cell.
    > Memory allocation & initialization will be handled during cell setup.
      Reconfigring few parameter(like BW,cyclic prefix,etc...) can block 
      scheduling until reconfiguration is completed.
*/
typedef struct _cell
{
   U16		cellId;		// cell ID
   BW		cellBw;		// cell BandWidth
   U8		index;		/* _eNb[index] = this cell
   				  index must not be more than NUMBER_OF_CELLS*/
   Bool		isFdd;		// FDD = 1, TDD = 0;
   U16		puschRbs;	// number of RBs allocated for PUSCH
   U8		cfi;		// current Conto; Format Indication
   U8		cyclicPrefix;	// normal/extended cyclic prifix
   U8		phichNgFactor;	// PHICH resources
   Bool         phichDuration;	// normal/extended
   /*UE can figure out how many antenna eNB is using by checking PBCH CRC Mask.*/
   U8		numOfAnt;	// number of antenna in used in this cell

   U64		timer;		// Date:Hr:Min:Sec:Milsec:NanoSec: (reference for SFC & other timers)
   U16		sfc;		// subframeCounter
   Bool		isAbsSf;	// Absolute blank sub-frame(condition no SI/paging/Msg2)
	
   U16		maxUes;		// number of UE's supported by cell
   U16		atvUes;
   ***		atvUeSes;	// active UE sessions(RRC connected) in sorted manner
   ***		atvUeDrxSleepSes;
   ***		ueSesForDeAtv;
   ***		msg1Ses;
   ***		usedRaRntis;	// to handle repeated RaRnti in successive RACH occassions
   ***		msg2Ses;
   ***		msg3Ses;
   ***		msg4Ses;
				// max aggregation and min MCS
   ***		pdcchAllocInfo;	// PDCCH allocations for current SF(n-2)
   ***		pdschAllocInfo;	// PDSCH allocations for corresponding PDCCH allocation

   /*control info received from higher layer received n-2*/
   Bool		isPaginSf;
   ***		pagingInfo;

   Bool		isSiSf;
   ***		siInfo;


   /* average DL/UL channel estimation includes parameters like path loss, fading effects,
      multipath propagation, Doppler effect, thermal noise, inter-modulation, etc... */
   U8		avgAvgPucchSinr;	// average UL channel estimation(updated based on PUCCH received)
   U8		avgAvgPuschSinr;	// average UL channel estimation(updated based on Ul allocation)
   U8		avgAvgDlCqi;	// average DL channel estimation
   U8		dlCqiPerSb[MAX_SUBBAND];
   U8		avgTa;		// average TA detected in cell 
	
   U16		numOfCellEdgeUrs;	// Aggressiveness parameter
   U8		crntiCeReceived;



   /*KPI's*/
   U32		cellDlThr;  /*DL KPI: throughput per cell for refreshed for every 5 min*/
   U32		cellUlThr;  /*UL KPI: throughput per cell for refreshed for every 5 min*/

   U8		dlPrbUtilization; 
   U8		ulPebUtilization;

   U8		dlBler;
   U8		ulBler;

   U8		avgPrbPerUE;
   U8		avgAllocLatencyGbr;  // average SF difference between allocation for GBR bearers
   U8		avgAllocLatencyNgbr;  // average SF difference between allocation for N-GBR bearers



};  /*  _cell  */





typedef struct _ue
{
  U8		ueId;	// UE identity in Mac & RRC
  ***		cell[MAX_CELLS];	// ponter to Pcell & Scell
  U8		lcBitMask;	// Active Logical Channels bit mask
  U8		dataBal[MAX_LC];	// Remaining data per logical channel
  U16		totData;	// Accumilated data from all LC's

  U8		tm;
  BOOL		isTmSwitchInProgress;	/* This will be set to FALSE after some delay
                                   of MAC configuration */
  BOOL		isMsg4Done;

  BOOL		isDl256Enabled;
  BOOL		isTtiBundlingEnabled;

  U32		weight[MAX_LCS];

  ***		ueCpability;
  U8		isDlFssEnabled;

  U8		aggregation;
  ***		dlLa;	/* PDSCH link adaptation parameters
	                   curr/prv RI
	                   curr/prv CQI
	                   curr/prv PMI
	                   MCS	// current mcs
	                   nxtMcs
                         */
                        /* MCS for next allocation
	        	   when channel is good this is MCS+1 (when CQI is discarded)
	                   when channel is bad this is MCS-1 after 2 Nacks*/

  U8		maxTbSize; // Decided based on rank
  U8		ta;
  U8		phr;
  U16		bsr[MAX_LCG];
  U32		totBsr;
  U8		ulSinr[MAX_SUBBAND];

  BOOL		isAperiodicCqitriggered;
  U8		aPeriodicSchedTime;

  BOOL		isTaSched;
  U8		taSchedTime;

  BOOL		isTpcSched;
  U8		tpcSchedTime;

  BOOL		isHoTriggered;
  U8		targetCellId;
  BOOL		delUeContext;

  BOOL		isDtxEnabled;
  *** 		dtx;

  BOOL		isSpsConf;
  BOOL		isSpsEnabled;
  BOOL		isDynamicSched;
  ***		sps;

  BOOL		isVolte;

  ***		ulHarqPrs[MAX_HARQ_PROCESS]; /*(hqId, raType, rv, numRbs, isDtx,
	                                        sbBitMap, agg, isAperiodicSched,
	                                        isTaSched, isTpcSched, isSrbSched,
	                                        mcs, tbSize, isGbr, numLcsSched,
	                                        isVolte)*/
  ***		dlHarqPrs[MAX_HARQ_PROCESS];
  U8		atvNumOfDlHqPrs;
  U8		atvNumOfUlHqPrs;

  ***		dlFreeHarqPrs;
  ***		ulFreeHarqPrs;

  BOOL		isCaEnabled;
  BOOL		isCaConfigured;
  U8		numOfSecCellsConf;
  U8		numOfSecCellsActiv;
  ***		ca;

  BOOL		isCellEdge;	// set TRUE based on measurement report
  U16		numOfAcks;
  U16		numOfNacks;
  U8		numOfDtx;

  ***		timers;	//csiPeriodicity,SPS,BSR,SR,RI,DRX,tti bundling
}; /* _ue */
