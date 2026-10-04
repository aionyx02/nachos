#include "syscall.h"

main() {
	int op1, op2;
	int a, b, c, d,e, div0;
	op1 = 2023;
	op2 = 112;
	a = Add(op1, op2);
	b = Sub(op1, op2);
	c = Mul(op1, op2);
	d = Div(op1, op2);
	e = Mod(op1, op2);
	PrintInt(a);
	PrintInt(b);
	PrintInt(c);
	PrintInt(d);
	PrintInt(e);
	div0 = Div(1, 0);
	PrintInt(div0);
	div0 = Mod(1, 0);
	PrintInt(div0);
}
