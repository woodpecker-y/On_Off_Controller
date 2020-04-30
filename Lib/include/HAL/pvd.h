#ifndef _PVD_H
#define _PVD_H

#include "stm32f10x.h"
/*! \brief
*       PVD Init
*/
void pvd_init(void);

void pvd_isr(void);
    
u8 pvd_get_state(void);
    

#endif
