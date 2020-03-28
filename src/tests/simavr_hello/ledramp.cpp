/*
	ledramp.c
	
	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <pthread.h>

#include <time.h>  // pause msec
#include <errno.h> // pause msec

#include <stdbool.h>

extern "C"
{
#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"
#include "button.h"
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
	(byte & 0x80 ? '1' : '0'),     \
		(byte & 0x40 ? '1' : '0'), \
		(byte & 0x20 ? '1' : '0'), \
		(byte & 0x10 ? '1' : '0'), \
		(byte & 0x08 ? '1' : '0'), \
		(byte & 0x04 ? '1' : '0'), \
		(byte & 0x02 ? '1' : '0'), \
		(byte & 0x01 ? '1' : '0')

button_t button;
int do_button_press = 0;
avr_t *avr = NULL;
avr_vcd_t vcd_file;
uint8_t pin_state = 0; // current port B

float pixsize = 64;
int window;

/*
 * called when the AVR change any of the pins on port B
 * so lets update our buffer
 */
void pin_changed_hook(struct avr_irq_t *irq, uint32_t value, void *param)
{
	pin_state = (pin_state & ~(1 << irq->irq)) | (value << irq->irq);
}

void keyCB(unsigned char key, int x, int y) /* called on key press */
{
	if (key == 'q')
		exit(0);
	//static uint8_t buf[64];
	switch (key)
	{
	case 'q':
	case 0x1f: // escape
		exit(0);
		break;
	case ' ':
		do_button_press++; // pass the message to the AVR thread
		break;
	case 'r':
		printf("Starting VCD trace\n");
		avr_vcd_start(&vcd_file);
		break;
	case 's':
		printf("Stopping VCD trace\n");
		avr_vcd_stop(&vcd_file);
		break;
	}
}

int msleep(long msec)
{
	struct timespec ts;
	int res;

	if (msec < 0)
	{
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do
	{
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

	return res;
}

static void *interface_thread(void *param)
{
	while (true)
	{
		printf("leds : " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(pin_state));
		printf("\n");
		msleep(1000 / 64);
	}
	return NULL;
}

static void *avr_run_thread(void *param)
{
	int b_press = do_button_press;

	while (true)
	{
		avr_run(avr);
		if (do_button_press != b_press)
		{
			b_press = do_button_press;
			printf("Button pressed\n");
			button_press(&button, 1000000);
		}
	}
	return NULL;
}

int ledramp_init()
{
	elf_firmware_t f;
	const char *fname = "./src/atmega_firmware.elf";
	//char path[256];

	//	sprintf(path, "%s/%s", dirname(argv[0]), fname);
	//	printf("Firmware pathname is %s\n", path);
	elf_read_firmware(fname, &f);

	printf("firmware %s f=%d mmcu=%s\n", fname, (int)f.frequency, f.mmcu);

	avr = avr_make_mcu_by_name(f.mmcu);
	avr_init(avr);
	avr_load_firmware(avr, &f);

	// initialize our 'peripheral'
	button_init(avr, &button, "button");
	// "connect" the output irw of the button to the port pin of the AVR
	avr_connect_irq(
		button.irq + IRQ_BUTTON_OUT,
		avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0));

	// connect all the pins on port B to our callback
	for (int i = 0; i < 8; i++)
		avr_irq_register_notify(
			avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), i),
			pin_changed_hook,
			NULL);

	// even if not setup at startup, activate gdb if crashing
	avr->gdb_port = 1234;
	if (0)
	{
		//avr->state = cpu_Stopped;
		avr_gdb_init(avr);
	}

	/*
	 *	VCD file initialization
	 *	
	 *	This will allow you to create a "wave" file and display it in gtkwave
	 *	Pressing "r" and "s" during the demo will start and stop recording
	 *	the pin changes
	 */
	avr_vcd_init(avr, "gtkwave_output.vcd", &vcd_file, 100000 /* usec */);
	avr_vcd_add_signal(&vcd_file,
					   avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN_ALL), 8 /* bits */,
					   "portb");
	avr_vcd_add_signal(&vcd_file,
					   button.irq + IRQ_BUTTON_OUT, 1 /* bits */,
					   "button");

	// 'raise' it, it's a "pullup"
	avr_raise_irq(button.irq + IRQ_BUTTON_OUT, 1);

	printf("Demo launching: 'LED' bar is PORTB, updated every 1/64s by the AVR\n"
		   "   firmware using a timer. If you press 'space' this presses a virtual\n"
		   "   'button' that is hooked to the virtual PORTC pin 0 and will\n"
		   "   trigger a 'pin change interrupt' in the AVR core, and will 'invert'\n"
		   "   the display.\n"
		   "   Press 'q' to quit\n\n"
		   "   Press 'r' to start recording a 'wave' file\n"
		   "   Press 's' to stop recording\n");

	// the AVR run on it's own thread. it even allows for debugging!
	pthread_t run;
	pthread_create(&run, NULL, avr_run_thread, NULL);

	pthread_t run_interface;
	pthread_create(&run_interface, NULL, interface_thread, NULL);
}
