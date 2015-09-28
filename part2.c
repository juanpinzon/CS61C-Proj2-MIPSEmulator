#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"
  
int32_t SignExt(int16_t n) {
	return (int32_t)(int16_t)n;
}


void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {	
  
	/* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
	switch(instruction.opcode) {
		case 0x0: // opcode == 0x0(SPECIAL)
			switch(instruction.rtype.funct) {
				case 0x0: // funct == 0x0 (sll)       
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
					processor->PC += 4;   
					break;
				case 0x2: // funct == 0x2 (srl)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
					processor->PC += 4;
					break;
				case 0x3: // funct == 0x3 (sra)
					processor->R[instruction.rtype.rd] = (int32_t)(processor->R[instruction.rtype.rt]) << instruction.rtype.shamt;
					processor->PC += 4;  
					break;
				case 0x8: // funct == 0x8 (jr)
					processor->PC = processor->R[instruction.rtype.rs];
					break;
				case 0x9: // funct == 0x9 (jalr)
				{
					Word tmp = processor->PC + 4;
					processor->PC = processor->R[instruction.rtype.rs];
					processor->R[instruction.rtype.rd] = tmp;
					break;
				}
				case 0xc: // funct == 0xc (SYSCALL)
					execute_syscall(processor);
					processor->PC += 4;
					break;
				case 0x10: // funct == 0x10 (mfhi)
					processor->R[instruction.rtype.rd] = processor->RHI;
					processor->PC += 4;
					break;
				case 0x12: // funct == 0x12 (mflo)
					processor->R[instruction.rtype.rd] = processor->RLO;
					processor->PC += 4;
					break;
				case 0x18: // funct == 0x18 (mult)
				{
					sDouble tmp = ((int32_t)(processor->R[instruction.rtype.rs])) * ((int32_t)(processor->R[instruction.rtype.rt]));
					processor->RLO = ((tmp<<32)>>32);
					processor->RHI = (tmp>>32);					
					processor->PC += 4;
					break;
				}
				case 0x19: // funct == 0x19 (multu)
				{
					Double tmp = processor->R[instruction.rtype.rs] * processor->R[instruction.rtype.rt];
					processor->RLO = ((tmp<<32)>>32);
					processor->RHI = (tmp>>32);					
					processor->PC += 4;
					break;
				}
				case 0x21: // funct == 0x21 (addu)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
					processor->PC += 4;
					break;
				case 0x23: // funct == 0x23 (subu)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
					processor->PC += 4;
					break;					      				
				case 0x24: // funct == 0x24 (AND)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
					processor->PC += 4;
					break;
				case 0x25: // funct == 0x25 (or)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
					processor->PC += 4;
					break;			
				case 0x26: // funct == 0x26 (xor)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
					processor->PC += 4;
					break;
				case 0x27: // funct == 0x27 (nor)
					processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
					processor->PC += 4;
					break;
				case 0x2a: // funct == 0x2a (slt)
					processor->R[instruction.rtype.rd] = (int32_t)(processor->R[instruction.rtype.rs]) < (int32_t)(processor->R[instruction.rtype.rt]);
					processor->PC += 4;
					break;			
				case 0x2b: // funct == 0x2b (sltu)
					processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];					
					processor->PC += 4;
					break;					                
				default: // undefined funct
					fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
					exit(-1);
					break;
			}
			break;
		
		case 0x2: // opcode == 0x2 (J)
			processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
			break;
		case 0x3: // opcode == 0x3 (jal)
			processor->R[31] = processor->PC + 4;     
			processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
			break;
		case 0x4: // opcode == 0x4 (beq)
			if (processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt])
				processor->PC += 4 + (SignExt(instruction.itype.imm) << 2); 
			else
				processor->PC += 4;
			break;
		case 0x5: // opcode == 0x5 (bne)
			if (processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt])
				processor->PC += 4 + (SignExt(instruction.itype.imm) << 2); 
			else
				processor->PC += 4;	
			break;
		case 0x9: // opcode == 0x9 (addiu)		  	
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm);
			processor->PC += 4;
			break;
		case 0xa: // opcode == 0xa (slti)
			processor->R[instruction.itype.rt] = (int32_t)(processor->R[instruction.itype.rs]) < SignExt(instruction.itype.imm); 		
			processor->PC += 4;    		      		
			break;
		case 0xb: // opcode == 0xb (sltiu)
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] < SignExt(instruction.itype.imm); 		
			processor->PC += 4;    		      		
			break;
		case 0xc: // opcode == 0xc (andi)
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;		
			processor->PC += 4;    		      		 			  	
			break;
		case 0xd: // opcode == 0xd (ORI)
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
			processor->PC += 4;
			break;
		case 0xe: // opcode == 0xe (xori)
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
			processor->PC += 4;
			break;
		case 0xf: // opcode == 0xf (lui)
			processor->R[instruction.itype.rt] = instruction.itype.imm << 16;	
			processor->PC += 4;
			break;
		case 0x20: // opcode == 0x20 (lb)
			processor->R[instruction.itype.rt] = SignExt( load(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_BYTE) );
			processor->PC += 4;
			break;
		case 0x21: // opcode == 0x21 (lh)
			processor->R[instruction.itype.rt] = SignExt( load(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_HALF_WORD) );
			processor->PC += 4;
			break;
		case 0x23: // opcode == 0x23 (lw)
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_WORD );
			processor->PC += 4;
			break; 
		case 0x24: // opcode == 0x24 (lbu)
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_BYTE );
			processor->PC += 4;
			break;
		case 0x25: // opcode == 0x25 (lhu)
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_HALF_WORD );
			processor->PC += 4;
			break;  
		case 0x28: // opcode == 0x28 (sb)
			store(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt] );		
			processor->PC += 4;
			break;		  
		case 0x29: // opcode == 0x29 (sh)
			store(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt] );		
			processor->PC += 4;
			break;		  
		case 0x2b: // opcode == 0x2b (sw)
			store(memory, processor->R[instruction.itype.rs] + SignExt(instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt] );		
			processor->PC += 4;
			break; 
		default: // undefined opcode
			fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
			exit(-1);
			break;
	}	
}


int check(Address address,Alignment alignment) {	
    /* YOUR CODE HERE */
	if((address%alignment) != 0) 																	//memory access is misaligned
  		return 0; 
    if(alignment!=LENGTH_BYTE && alignment!=LENGTH_HALF_WORD && alignment!=LENGTH_WORD) 			//misaligned loads and stores
		return 0; 
	if(address>=MEMORY_SPACE || address<1) 														//memory outside of the range [1,MEM_SPACE). 
		return 0; 
    if(address == 0x0)																				//NULL address 0
		return 0;
  	
  	return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */  
	if (alignment == LENGTH_WORD) 
		*(uint32_t*)(memory + address) = (uint32_t)value;
	else if (alignment == LENGTH_HALF_WORD)
		*(uint16_t*)(memory + address) = (uint16_t)value;
	else if (alignment == LENGTH_BYTE)
		*(uint8_t*)(memory + address) = (uint8_t)value;           
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    if (alignment == LENGTH_WORD)
		return *(uint32_t*)(memory + address);
	else if (alignment == LENGTH_HALF_WORD)
		return *(uint16_t*)(memory + address);
	else if (alignment == LENGTH_BYTE)
		return *(uint8_t*)(memory + address);
	
    exit(-1);
}
