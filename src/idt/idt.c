#include <idt/idt.h>
#include <khienh/config.h>

struct idt_desc idt_descriptors[KHIENHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

void idt_init()
{

}