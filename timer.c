#include "idt.h"
#include "io.h"
#include "serial.h"
#include "timer.h"

static u32int _tick = 0;
static u32int _frequency = 0;

#define TIMER_INTERRUPT (IRQ_INTERRUPT_BASE + 0)
#define MAX_PIT_FREQUENCY 1193180
#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define PIT_OPERATING_MODE_ONE_SHOT 0x02
#define PIT_OPERATING_MODE_SQUARE_WAVE 0x06
#define PIT_ACCESS_MODE_LOBYTE_HIBYTE 0x30

static void timer_callback(registers_t regs) {
   (void)regs;
   _tick++;
   if (_tick % _frequency == 0) {
      serial_print(SERIAL_COM1_BASE, "tick\n");
   }
}

void timer_init(u32int frequency) {
   register_interrupt_handler(TIMER_INTERRUPT, &timer_callback);

   // The value we send to the PIT is the value to divide its input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   _frequency = frequency;
   u32int divisor = MAX_PIT_FREQUENCY / _frequency;

   outb(PIT_COMMAND_PORT, PIT_OPERATING_MODE_SQUARE_WAVE | PIT_ACCESS_MODE_LOBYTE_HIBYTE);
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)((divisor >> 8) & 0xFF);

   outb(PIT_CHANNEL0_DATA_PORT, l);
   outb(PIT_CHANNEL0_DATA_PORT, h);
}
