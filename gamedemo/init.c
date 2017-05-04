
#include "init.h"

void init(){
   configureClocks();
  enableWDTInterrupts();
  lcd_init();
  clearScreen(COLOR_BLUE);
  p2sw_init(15);
  or_sr(0x8);

}
