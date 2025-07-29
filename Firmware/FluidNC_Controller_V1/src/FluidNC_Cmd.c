#include "inc/FluidNC_Cmd.h"

// void setPosition(int dir, int b_isRel)
// {

//   if (Fence.inputPos.length() == 0)
//     return;

//   Serial1.print("$J=");
//   Serial1.print("G21");

//   if (b_isRel)
//     Serial1.print("G91");
//   else
//     Serial1.print("G53");

//   if (dir)
//     Serial1.print("X-");
//   else
//     Serial1.print("X");

//   Serial1.print(Fence.inputPos);

//   Serial1.print("F2800");
//   Serial1.write(0x0A);

//   Fence.inputPos = "";
// }

// void jog(int dir)
// {
//   Serial1.print("$J=");
//   Serial1.print("G21");
//   Serial1.print("G91");

//   if (dir)
//     Serial1.print("X-");
//   else
//     Serial1.print("X");

//   Serial1.print("10");

//   Serial1.print("F1000");
//   Serial1.write(0x0A);
// }