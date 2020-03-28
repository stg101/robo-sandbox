
#ifndef __LEDRAMP_H__
#define __LEDRAMP_H__

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"
#include "button.h"

void pin_changed_hook(struct avr_irq_t *irq, uint32_t value, void *param);
void keyCB(unsigned char key, int x, int y);
int msleep(long msec);

static void *avr_run_thread(void *param);
int ledramp_init();

#endif /* __BUTTON_H__*/