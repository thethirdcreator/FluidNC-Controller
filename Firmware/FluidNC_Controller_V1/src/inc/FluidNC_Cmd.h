#ifndef __FLUID_CMDS_H__
#define __FLUID_CMDS_H__

#define GRBLCMD '$'
#define CMD_UNIT_MM "G21"
#define CMD_COORD_ABS "G53"
#define CMD_COORD_REL "G91"
#define CMD_FEED_RATE "F"

void setPosition(int dir, int b_isRel);
void jog(int dir);


#endif