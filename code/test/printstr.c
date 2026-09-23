#include "syscall.h"

main() {
	int len;
	len = Print("Hello NachOS2026!\n");
	PrintInt(len);
	len = Print("Good Morning!\n");
	PrintInt(len);
}
