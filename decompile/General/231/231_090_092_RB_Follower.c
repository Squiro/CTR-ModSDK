#include <common.h>

void DECOMP_RB_Follower_ProcessBucket(struct Thread* t)
{
	int i;
	int numPlyr;
	int driverID;
	struct Follower* fObj;
	struct Instance* inst;
	struct InstDrawPerPlayer* idpp;
	struct GameTracker* gGT = sdata->gGT;
	
	numPlyr = gGT->numPlyrNextGame;
	
	for(/**/; t != 0; t = t->siblingThread)
	{
		// skip dead threads
		if((t->flags & 0x800) != 0) continue;
		
		fObj = t->object;
		driverID = fObj->driver->driverID;
		
		inst = t->inst;
		idpp = INST_GETIDPP(inst);
		
		// make Follower invisible to all other players
		for(i = 0; i < numPlyr; i++)
			if(i != driverID)
				idpp[i].unkb8[0] &= ~(0x40);
		
		// make Mine invisible to this player
		inst = fObj->mineTh->inst;
		idpp = INST_GETIDPP(inst);
		idpp[driverID].unkb8[0] &= ~(0x40);
	}
}

void DECOMP_RB_Follower_ThTick(struct Thread* t)
{
	int kartState;
	struct Driver* d;
	struct Follower* fObj;
	struct Instance* inst;
	
	fObj = t->object;
	d = fObj->driver;
	kartState = d->kartState;
	
	fObj->frameCount--;
	
	if(
			(fObj->frameCount > 0) &&
			(
				(kartState == KS_NORMAL) ||
				(kartState == KS_DRIFTING)
			) &&
			
			// terrible way of checking if mineTh was destroyed
			// before the follower thread was destroyed
			(t->timesDestroyed == fObj->backupTimesDestroyed) &&
			
			(d->speedApprox > -1)
		)
	{
		inst = t->inst;
		
		if(inst->scale[0] < 0x800)
		{
			inst->scale[0] = inst->scale[0] << 1;
			inst->scale[1] = inst->scale[1] << 1;
			inst->scale[2] = inst->scale[2] << 1;
		}
		
		// midpoint between real mine position, and driver position
		inst->matrix.t[0] = (fObj->realPos[0] + (d->posCurr[0]>>8)) >> 1;
		inst->matrix.t[1] = (fObj->realPos[1] + (d->posCurr[1]>>8)) >> 1;
		inst->matrix.t[2] = (fObj->realPos[2] + (d->posCurr[2]>>8)) >> 1;
		
		return;
	}
	
	// kill thread
	t->flags |= 0x800;
}

void DECOMP_RB_Follower_Init(struct Driver* d, struct Thread* t)
{
  struct Instance* iVar1;
  struct Follower* fObj;
  struct Instance* iVar3;
  
  if (
		(0x1e00 < d->speedApprox) && 
			
		// if driver is not an AI
		((d->actionsFlagSet & 0x100000) == 0) &&
		
		(((sdata->gGT->cameraDC[d->driverID].flags) & 0x10000) == 0)
	) 
  {	 
	// create a thread and an Instance
    iVar1 = INSTANCE_BirthWithThread(
		t->modelIndex, 0, SMALL, FOLLOWER, 
		DECOMP_RB_Follower_ThTick, sizeof(struct Follower), 0);
    
	if (iVar1 == NULL) return; 

	// set the scale
    iVar1->scale[0] = 0x200;
    iVar1->scale[1] = 0x200;
    iVar1->scale[2] = 0x200;
	
	// get instance from thread
    iVar3 = t->inst;
	
	// copy position and rotation from one instance to another 
    *(int*)&iVar1->matrix.m[0][0] = *(int*)&iVar3->matrix.m[0][0];
    *(int*)&iVar1->matrix.m[0][2] = *(int*)&iVar3->matrix.m[0][2];
    *(int*)&iVar1->matrix.m[1][1] = *(int*)&iVar3->matrix.m[1][1];
    *(int*)&iVar1->matrix.m[2][0] = *(int*)&iVar3->matrix.m[2][0];
    iVar1->matrix.m[2][2] = iVar3->matrix.m[2][2];
    
    iVar1->matrix.t[0] = iVar3->matrix.t[0];
    iVar1->matrix.t[1] = iVar3->matrix.t[1];
    iVar1->matrix.t[2] = iVar3->matrix.t[2];
	
	// Get the thread
    t = iVar1->thread;
	t->funcThDestroy = THREAD_DestroyInstance;
    
	fObj = t->object;
    fObj->frameCount = FPS_DOUBLE(7);
    fObj->driver = d;
    fObj->mineTh = t;
    fObj->backupTimesDestroyed = t->timesDestroyed;
	
	// save mine position
	fObj->realPos[0] = iVar3->matrix.t[0];
	fObj->realPos[1] = iVar3->matrix.t[1];
	fObj->realPos[2] = iVar3->matrix.t[2];
  }
}