#ifndef _IO_H_
#define _IO_H_

/**
 * Reference: x86 Instruction Set Reference, IN, input from the port
 * description:
 *     we will implement these four functions in assembly code
 * */

#include <khienh/type.h>

/* read one byte from the port */
u8 insb(u16 port);

/* read one word (that is two byte in x86) from the port */
u16 insw(u16 port);

void outb(u16 port, u8 val);
void outw(u16 port, u8 val);

#endif