#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

/*
    --> Instruction table <--
Opcode	Name	    Description
0x01	LOAD	    RD = MEM[RS1 + IMM]
0x02	STORE	    MEM[RS1 + IMM] = RD
0x03	MOV	        RD = RS1
0x04	ADDI	    RD = RS1 + IMM
0x05	ADD	        RD = RS1 + RS2
0x06	SUB	        RD = RS1 - RS2
0x07	JMP	        PC = IMM
0x08	BEQ	        if (RS1 == RS2) PC = IMM
0x09	SYSCALL	    Trap to kernel; ID in R0, args in R1+
0xFF	HALT	    Stop execution
*/

typedef enum: int {
    LOAD    = 0x01,
    STORE   = 0x02,
    MOV     = 0x03,
    ADDI    = 0x04,
    ADD     = 0x05,
    SUB     = 0x06,
    JMP     = 0x07,
    BEQ     = 0x08,
    SYSCALL = 0x09,
    HALT    = 0xFF
} Instruction;


#endif  // __INSTRUCTION_H__