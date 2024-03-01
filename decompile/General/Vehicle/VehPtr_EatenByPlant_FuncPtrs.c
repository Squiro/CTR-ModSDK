#include <common.h>

void DECOMP_VehPtr_EatenByPlant_Update(struct Thread *t, struct Driver *d)
{
    short noInput;
    struct Instance *inst = t->inst;

    d->NoInputTimer -= sdata->gGT->elapsedTimeMS;

    if (d->NoInputTimer < 0)
        d->NoInputTimer = 0;

    if (d->NoInputTimer != 0) return;

	// === respawn driver ===

    // respawn driver at last valid quadblock
    VehPtr_MaskGrab_FindDestPos(d, d->lastValid);
    VehInit_TeleportSelf(d, 0, 0x80);

    // enable collision, make visible
    t->flags &= ~(0x1000);
    inst->flags &= ~(HIDE_MODEL);

    // this lets you rev engine while falling
    VehPtr_EngineRevving_Init(t, d);
}

void DECOMP_VehPtr_EatenByPlant_PhysLinear(struct Thread *t, struct Driver *d)
{
    VehPtr_Driving_PhysLinear(t, d);

    d->simpTurnState = 0;

    // reset two speed variables
    d->fireSpeed = 0;
    d->baseSpeed = 0;

    // reset jump variable
    d->jump_TenBuffer = 0;

    // acceleration prevention,
    // drop bits for jump button, 0x20?, reversing engine
    d->actionsFlagSet = ((d->actionsFlagSet & ~(0x20000 | 0x20 | 4)) | 8);

    // increment time spent in mask grab
    d->timeSpentMaskGrabbed += sdata->gGT->elapsedTimeMS;
}

void DECOMP_VehPtr_EatenByPlant_Animate(struct Thread *t, struct Driver *d)
{
    short sVar1;
    long dist;
    struct Instance* inst;
    SVECTOR plantVector;
    VECTOR camVec;
    long alStack32[2];

    struct GameTracker *gGT = sdata->gGT;

    struct Thread* plant = d->plantEatingMe;

    // if any plant is eating me
    if (((plant != NULL) &&

        // if not initialized
        (d->KartStates.EatenByPlant.boolInited == false)) &&

        // if more than 0.5s since player death
        (d->NoInputTimer < 2880))
    {
        // get instance from thread
        inst = plant->inst;

        // initialized, player eaten
        d->KartStates.EatenByPlant.boolInited = true;

        plantVector.vx = (((struct Plant *)plant->object)->LeftOrRight == 0) ? 250 : -250;
        plantVector.vy = 0;
        plantVector.vz = 750;

        SetRotMatrix(&inst->matrix);

        SetTransMatrix(&inst->matrix);

        RotTrans(&plantVector, &camVec, alStack32);

        struct TileView * view = &gGT->tileView[d->driverID];

        view->pos[0] = camVec.vx;
        view->pos[1] = inst->matrix.t[1] + 0xc0;
        view->pos[2] = camVec.vz;

        int camX = camVec.vx - inst->matrix.t[0];
        int camZ = camVec.vz - inst->matrix.t[2];

        view->rot[1] = (short)ratan2(camX, camZ);

        // get distance between car and camera
        dist = SquareRoot0_stub(camX * camX + camZ * camZ);

        view->rot[0] = (short) 0x800 - ratan2(view->rot[0] - inst->matrix.t[2], dist);

        view->rot[2] = 0;
    }
}
