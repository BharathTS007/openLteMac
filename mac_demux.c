/**********************************************************************

 This files have functions which handles UL data or subframe UL data
 from L1.
 
 After demultiplexing corresponging UE's & cell's parameters will be 
 updated w.r.t MAC.
 
Functions:
	Demux mac elements.
	update UE's or cells's parameter.
	send LC data to RLC.
	
 number of threads will be equal to number of cells. Triggers scheduling
 after performing above functions.
 
**********************************************************************/


extern *** phySfErrList; /*L1 subframe error record list, common to all cells.
	    		   will be refreshed every SF*/
extern U8  phySfErrCnt; /*incremented for every new record added to error list,
                      will be refreshed every SF*/

/*
 * @brief This function is the is called by L1 every subframe.
 *        if any error while processing data all data will bu copied to
 *        "phySfErrList" global & common variable with L1 
 *
 * @details
 *
 * Function: phy_mac_sfData 
 * Purpose : demultiplex of L1 data 
 * 
 * Invoked by: sighandler for L1_sf_ind. 
 *            number of instance will be equal to number of cells.
 *            once all threas completes demuxing hanler will send
 *            a accumulated error recored to L1.
 * 
 * @param[in]  : pucch sf data
 * @param[in]  : number of pucch records in list
 * @param[in]  : pusch sf data
 * @param[in]  : number of pusch records in list
 *               this count has to be sa
 *
 * @returns    : FALSE
 *               if error while processing PUCCH/PUSCH data.
 *               error records will be in "phySfErrList"
 *               which is commom to all cells
 *               else TRUE
 *
 */
BOOL phy_mac_sfData(void** pucch, U8 numPucchRec, void** pusch, U8 numPuschRec, U8 cellId)
{
  U8 rec;
  *** pucchErrRec,puschErrRec;

  /* BELOW CASE HAS TO BE HANDLED IN "L1_SF_IND" handler*/
  /*if (phySfErrList != NULLP && phySfErrCnt != 0)
  {
	//TODO: take bakup of previous error list
	phySfErrList = NULLP;
	phySfErrCnt = 0;
  }*/

  for (rec = 0; rec <= numPucchRec; rec++)
  {
	pucchErrRec = mac_demux(pucch, numPucchRec, cellId);
  }
  for (rec = 0; rec <= numPucchRec; rec++)
  {
	puschErrRec = mac_demux(pusch, numPuschRec, cellId);
  }

  return(addRec(pucchErrRec,puschErrRec));
 
}



/* demux Ul subframe data as per 36.321 */
/* after processing each data we delete record allocated by L1*/
/* we will skip record if unable to process & include ueId to L1 in error list. */
/* sfData is either PUCCH and PUSCH data  */
/*TODO:return type will be null if all records are processed successfully
 (it's L1 responsibility to delete that record)*/
TODO mac_demux(void* sfData, U8 numData, U8 cellId)
{
	U8 rec;
	TODO ptr = sfData;


	for (rec = 0; rec <= numOfRec; rec++)
	{
	  switch (ptr->type)
	  {
		case **:
		  break;
	  }
	  ptr = ptr->nxt;
	}

}




