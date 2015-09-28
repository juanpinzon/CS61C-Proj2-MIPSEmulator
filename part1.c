#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"

void decode_instruction(Instruction instruction) {
  
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
	switch(instruction.opcode) {
		case 0x0: // opcode == 0x0 (SPECIAL)
			switch(instruction.rtype.funct) {                
				case 0x0: // funct == 0x0 (sll)
					printf("sll\t$%d,$%d,%u\n", instruction.rtype.rd, instruction.rtype.rt, instruction.rtype.shamt);
				break;
				case 0x2: // funct == 0x2 (srl)
					printf("srl\t$%d,$%d,%u\n", instruction.rtype.rd, instruction.rtype.rt, instruction.rtype.shamt);
					break;
				case 0x3: // funct == 0x3 (sra)
					printf("sra\t$%d,$%d,%u\n", instruction.rtype.rd, instruction.rtype.rt, instruction.rtype.shamt);
					break;
				case 0x8: // funct == 0x8 (jr)
					printf("jr\t$%d\n", instruction.rtype.rs);
					break;
				case 0x9: // funct == 0x9 (jalr)
					printf("jalr\t$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs);					
					break;
				case 0xc: // funct == 0xc (SYSCALL)
					printf("syscall\n");
					break;
				case 0x10: // funct == 010 (mfhi)
					printf("mfhi\t$%d\n", instruction.rtype.rd);
					break;
				case 0x12: // funct == 012 (mflo)
					printf("mflo\t$%d\n", instruction.rtype.rd);
					break;
				case 0x18: // funct == 0x18 (mult)
					printf("mult\t$%d,$%d\n", instruction.rtype.rs, instruction.rtype.rt);					
					break;
				case 0x19: // funct == 0x19 (multu)
					printf("multu\t$%d,$%d\n", instruction.rtype.rs, instruction.rtype.rt);					
					break;
				case 0x21: // funct == 0x21 (addu)
					printf("addu\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;
				case 0x23: // funct == 0x23 (subu)
					printf("subu\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;					      				
				case 0x24: // funct == 0x24 (AND)
					printf("and\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;
				case 0x25: // funct == 0x25 (or)
					printf("or\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;			
				case 0x26: // funct == 0x26 (xor)
					printf("xor\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;
				case 0x27: // funct == 0x27 (nor)
					printf("nor\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;
				case 0x2a: // funct == 0x2a (slt)
					printf("slt\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;			
				case 0x2b: // funct == 0x2b (sltu)
					printf("sltu\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
					break;  
				default: // undefined funct
					fprintf(stderr,"%s: illegal function: %08x\n",__FUNCTION__, instruction.bits);
					exit(-1);
					break;
			}
			break;
			
		case 0x2: // opcode == 0x2 (J)
			printf("j\t0x%x\n", instruction.jtype.addr*4);
			break;
		case 0x3: // opcode == 0x3 (jal)
			printf("jal\t0x%x\n", instruction.jtype.addr*4);
			break;
		case 0x4: // opcode == 0x4 (beq)
			printf("beq\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, (int16_t)instruction.itype.imm*4);
			break;
		case 0x5: // opcode == 0x5 (bne)
			printf("bne\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, (int16_t)instruction.itype.imm*4);
			break;
		case 0x9: // opcode == 0x9 (addiu)
			printf("addiu\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, (int16_t)instruction.itype.imm);
			break;
		case 0xa: // opcode == 0xa (slti)
			printf("slti\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
			break;
		case 0xb: // opcode == 0xb (sltiu)
			printf("sltiu\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
			break; 
		case 0xc: // opcode == 0xc (andi)
			printf("andi\t$%d,$%d,0x%x\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
			break;		  	            
		case 0xd: // opcode == 0xd (ORI)
			printf("ori\t$%d,$%d,0x%x\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
			break;  
		case 0xe: // opcode == 0xe (xori)
			printf("xori\t$%d,$%d,0x%x\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
			break;  
		case 0xf: // opcode == 0xf (lui)
			printf("lui\t$%d,0x%x\n", instruction.itype.rt, instruction.itype.imm);
			break;
		case 0x20: // opcode == 0x20 (lb)
			printf("lb\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;
		case 0x21: // opcode == 0x21 (lh)
			printf("lh\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;
		case 0x23: // opcode == 0x23 (lw)
			printf("lw\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;		  
		case 0x24: // opcode == 0x24 (lbu)
			printf("lbu\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;
		case 0x25: // opcode == 0x25 (lhu)
			printf("lhu\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;  
		case 0x28: // opcode == 0x28 (sb)
			printf("sb\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;		  
		case 0x29: // opcode == 0x29 (sh)
			printf("sh\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;		  
		case 0x2b: // opcode == 0x2b (sw)
			printf("sw\t$%d,%d($%d)\n", instruction.itype.rt, (int16_t)instruction.itype.imm, instruction.itype.rs);
			break;                        
		default: // undefined opcode
			fprintf(stderr, "%s: illegal instruction: %08x\n",__FUNCTION__, instruction.bits);
			exit(-1);
			break;
	}
}
