
#define OFFSETOF(TYPE, ELEMENT) ((unsigned int)&(((TYPE *)0)->ELEMENT))

#define NEW_VERTEX(posX, posY, posZ, flag, colR, colG, colB) \
{ \
	.pos = {posX, posY, posZ}, \
	.flags = flag, \
	.color_hi = {colR, colG, colB, 0}, \
	.color_lo = {colR, colG, colB, 0}, \
}

// can't change these, or else triNormalVec has to change
#define sizeX 0x300
#define sizeZ 0x300

#define NEW_BLOCK(qIndex, group4, posX, posZ, flagV, flagQ, colR, colG, colB) \
	.levVertex[9*qIndex+0] = NEW_VERTEX(posX-sizeX/2, 0, posZ-sizeZ/2, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+1] = NEW_VERTEX(posX+sizeX/2, 0, posZ-sizeZ/2, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+2] = NEW_VERTEX(posX-sizeX/2, 0, posZ+sizeZ/2, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+3] = NEW_VERTEX(posX+sizeX/2, 0, posZ+sizeZ/2, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+4] = NEW_VERTEX(posX, 0, posZ-sizeZ/2, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+5] = NEW_VERTEX(posX-sizeX/2, 0, posZ, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+6] = NEW_VERTEX(posX, 0, posZ, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+7] = NEW_VERTEX(posX+sizeX/2, 0, posZ, flagV, colR, colG, colB),\
	.levVertex[9*qIndex+8] = NEW_VERTEX(posX, 0, posZ+sizeZ/2, flagV, colR, colG, colB), \
	.quadBlock[qIndex] = \
	{ \
		.index = {9*qIndex+0,9*qIndex+1,9*qIndex+2,9*qIndex+3,9*qIndex+4,9*qIndex+5,9*qIndex+6,9*qIndex+7,9*qIndex+8}, \
		.quadFlags = flagQ, \
		.draw_order_low = 0x80000000, /* double-sided render */ \
		.draw_order_high = 0, \
		.ptr_texture_mid = \
		{ \
			OFFSETOF(struct LevelFile, group4)-4, \
			OFFSETOF(struct LevelFile, group4)-4, \
			OFFSETOF(struct LevelFile, group4)-4, \
			OFFSETOF(struct LevelFile, group4)-4 \
		}, \
		\
		.bbox = \
		{ \
			.min = {posX-sizeX/2, 0, posZ-sizeZ/2}, \
			.max = {posX+sizeX/2, 0, posZ+sizeZ/2} \
		}, \
		.terrain_type = 0, \
		.weather_intensity = 0, \
		.weather_vanishRate = 0, \
		.speedImpact = 0, \
		\
		.blockID = 0, \
		.respawnIndex = qIndex, \
		.triNormalVecBitShift = 0x12, \
		\
		.ptr_texture_low = OFFSETOF(struct LevelFile, group4)-4, \
		.pvs = OFFSETOF(struct LevelFile, pvs)-4, \
		\
		.triNormalVecDividend = \
		{ \
			/* hi 2 */ \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			0x1C71, \
			\
			/* lo 2 */ \
			0x1C71, \
			0x1C71, \
		} \
	}
	
// get vertex numbers from here:
// posted by DCxDemo, 4/25/2020 2:44pm, in #ctr-mod-ideas
// https://discord.com/channels/527135227546435584/534794647738908683/703677797440159744
#define MAKE_RAMP(qIndex, height, low1, low2, low3, mid1, mid2, mid3, hi1, hi2, hi3) \
	.levVertex[9*qIndex+low1].pos[1] = 0, \
	.levVertex[9*qIndex+low2].pos[1] = 0, \
	.levVertex[9*qIndex+low3].pos[1] = 0, \
	.levVertex[9*qIndex+mid1].pos[1] = height/2, \
	.levVertex[9*qIndex+mid2].pos[1] = height/2, \
	.levVertex[9*qIndex+mid3].pos[1] = height/2, \
	.levVertex[9*qIndex+hi1].pos[1] = height, \
	.levVertex[9*qIndex+hi2].pos[1] = height, \
	.levVertex[9*qIndex+hi3].pos[1] = height, \
	.quadBlock[qIndex].respawnIndex = -1, \
	.quadBlock[qIndex].bbox.max[1] = height, \
	.quadBlock[qIndex].draw_order_low = 0x80000000, \
	.quadBlock[qIndex].triNormalVecDividend = \
	{ \
		/* hi 2 */ \
		0x1971, \
		0x1971, \
		0x1971, \
		0x1971, \
		0x1971, \
		0x1971, \
		0x1971, \
		0x1971, \
		\
		/* lo 2 */ \
		0x1971,  \
		0x1971,  \
	}
	
#define SET_POSY_FLAT(qIndex, posY) \
	.levVertex[9*qIndex+0].pos[1] = posY, \
	.levVertex[9*qIndex+1].pos[1] = posY, \
	.levVertex[9*qIndex+2].pos[1] = posY, \
	.levVertex[9*qIndex+3].pos[1] = posY, \
	.levVertex[9*qIndex+4].pos[1] = posY, \
	.levVertex[9*qIndex+5].pos[1] = posY, \
	.levVertex[9*qIndex+6].pos[1] = posY, \
	.levVertex[9*qIndex+7].pos[1] = posY, \
	.levVertex[9*qIndex+8].pos[1] = posY, \
	.quadBlock[qIndex].bbox.min[1] = posY, \
	.quadBlock[qIndex].bbox.max[1] = posY
	
#define SET_POSY_RAMP(qIndex, posY, height, low1, low2, low3, mid1, mid2, mid3, hi1, hi2, hi3) \
	.levVertex[9*qIndex+low1].pos[1] = posY, \
	.levVertex[9*qIndex+low2].pos[1] = posY, \
	.levVertex[9*qIndex+low3].pos[1] = posY, \
	.levVertex[9*qIndex+mid1].pos[1] = posY+(height/2), \
	.levVertex[9*qIndex+mid2].pos[1] = posY+(height/2), \
	.levVertex[9*qIndex+mid3].pos[1] = posY+(height/2), \
	.levVertex[9*qIndex+hi1].pos[1] = posY+height, \
	.levVertex[9*qIndex+hi2].pos[1] = posY+height, \
	.levVertex[9*qIndex+hi3].pos[1] = posY+height, \
	.quadBlock[qIndex].bbox.min[1] = posY, \
	.quadBlock[qIndex].bbox.max[1] = posY+height

// if clutX is 512 in VRAM, the clutX parameter is 32 (512>>4),
// bitshifting must be done for that variable, consistent to PNG names
#define ImageName_Blend(imgX, imgY, clutX, clutY, sizeX, sizeY, bpp, blend) \
	{.clut =  ((clutX >> 0) << 0) | (clutY << 6), \
	.tpage = ((imgX >> 6) << 0) | ((imgY >> 8) << 4) | (blend<<5) | (bpp<<7), \
	.u0 = ((imgX&0x3f)<<(2-bpp))+0, .v0 = (imgY&0xff)+0, \
	.u1 = ((imgX&0x3f)<<(2-bpp))+sizeX-1, .v1 = (imgY&0xff)+0, \
	.u2 = ((imgX&0x3f)<<(2-bpp))+0, .v2 = (imgY&0xff)+sizeY-1, \
	.u3 = ((imgX&0x3f)<<(2-bpp))+sizeX-1, .v3 = (imgY&0xff)+sizeY-1} \
	
// top/bottom left/right assuming you're rotation is 0,0,0
#define TEX_2X2(qIndex, BottomRight, BottomLeft, TopRight, TopLeft) \
	.quadBlock[qIndex].ptr_texture_mid = \
	{ \
		OFFSETOF(struct LevelFile, BottomRight)-4, \
		OFFSETOF(struct LevelFile, BottomLeft)-4, \
		OFFSETOF(struct LevelFile, TopRight)-4, \
		OFFSETOF(struct LevelFile, TopLeft)-4 \
	}

#define PTR_MAP_QUADBLOCK(x) \
	OFFSETOF(struct LevelFile, quadBlock[x].ptr_texture_mid[0])-4,\
	OFFSETOF(struct LevelFile, quadBlock[x].ptr_texture_mid[1])-4,\
	OFFSETOF(struct LevelFile, quadBlock[x].ptr_texture_mid[2])-4,\
	OFFSETOF(struct LevelFile, quadBlock[x].ptr_texture_mid[3])-4,\
	OFFSETOF(struct LevelFile, quadBlock[x].ptr_texture_low)-4,\
	OFFSETOF(struct LevelFile, quadBlock[x].pvs)-4