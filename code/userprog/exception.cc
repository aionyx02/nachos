// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
	int	type = kernel->machine->ReadRegister(2);
	int	val;

    switch (which) {
		case SyscallException:
	    	switch(type) {
				case SC_Halt:
		 		   	DEBUG(dbgAddr, "Shutdown, initiated by user program.\n");
   		    		kernel->interrupt->Halt();
		   		break;
				case SC_PrintInt:
					val=kernel->machine->ReadRegister(4);
					cout << "Print integer:" <<val << endl;
				return;
				case SC_Print: {
					int addr = kernel->machine->ReadRegister(4);	// r4 存放字串在 user 記憶體的位址
					int val;
					int length = 0;

					cout << "[B11415024_Print]";

					while (true) {
						// 一次從 user 記憶體讀一個字元
						if (!kernel->machine->ReadMem(addr, 1, &val)) {
							break;
						}

						char ch = (char)val;

						if (ch == '\0') {	// 讀到字串結尾就停止
							break;
						}

						if (ch == 'Y' || ch == 'y') {	// 遇到 landmine 字元改印 *
							cout << '*';
						}
						else {
							cout << ch;
						}

						length++;
						addr++;
					}
			
					kernel->machine->WriteRegister(2, length);	// 回傳字串長度
					break;
				}
/*				case SC_Exec:
					DEBUG(dbgAddr, "Exec\n");
					val = kernel->machine->ReadRegister(4);
					kernel->StringCopy(tmpStr, retVal, 1024);
					cout << "Exec: " << val << endl;
					val = kernel->Exec(val);
					kernel->machine->WriteRegister(2, val);
					return;
*/				case SC_Exit:
					DEBUG(dbgAddr, "Program exit\n");
					val=kernel->machine->ReadRegister(4);
					cout << "return value:" << val << endl;
					kernel->currentThread->Finish();
					break;
				case SC_Sleep:
					val=kernel->machine->ReadRegister(4);	// 從 r4 讀出要睡的時間
					cout << "Sleep Time:" << val << "(ms)" << endl;
					kernel->alarm->WaitUntil(val);	// 讓目前的 thread 進入睡眠
					break;
				// 四則運算從 r4、r5 讀出兩個數，結果寫回 r2
				case SC_Add: {
					int op1 = kernel->machine->ReadRegister(4);
					int op2 = kernel->machine->ReadRegister(5);

					int result = op1 + op2;

					kernel->machine->WriteRegister(2, result);
					break;
				}
				case SC_Sub: {
					int op1 = kernel->machine->ReadRegister(4);
					int op2 = kernel->machine->ReadRegister(5);

					int result = op1 - op2;
			
					kernel->machine->WriteRegister(2, result);
					break;
				}
				case SC_Mul: {
					int op1 = kernel->machine->ReadRegister(4);
					int op2 = kernel->machine->ReadRegister(5);

					int result = op1 * op2;

					kernel->machine->WriteRegister(2, result);
					break;
				}
				case SC_Div: {
					int op1 = kernel->machine->ReadRegister(4);
					int op2 = kernel->machine->ReadRegister(5);
					int result;
					if (op2 == 0) {	// 除數為 0 時不做除法，避免 NachOS crash
						cout << "Error: Divide by zero" << endl;

						result = 11415024;	// 回傳學號末 8 碼
					}
					else if (op1 == INT_MIN && op2 == -1) {	// 結果超出 int 範圍也會 crash
						result = INT_MIN;
					}
					else {
						result = op1 / op2;
					}

					kernel->machine->WriteRegister(2, result);
					break;
				}
        		case SC_Mod: {		
					int op1 = kernel->machine->ReadRegister(4);
					int op2 = kernel->machine->ReadRegister(5);
					int result;
					if (op2 == 0) {	// 除數為 0 時和 Div 一樣處理
						cout << "Error: Divide by zero" << endl;

						result = 11415024;
					}
					else if (op1 == INT_MIN && op2 == -1) {	// 和 Div 相同的情況，餘數為 0
						result = 0;
					}
					else {
						result = op1 % op2;
					}

					kernel->machine->WriteRegister(2, result);
					break;
				}
				default:
		    		cerr << "Unexpected system call " << type << "\n";
 		    		break;
	    		}
	    		return;
			default:
	   			 cerr << "Unexpected user mode exception" << which << "\n";
	   			 break;
   	}
    ASSERTNOTREACHED();
}
