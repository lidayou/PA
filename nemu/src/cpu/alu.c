#include "cpu/cpu.h"

uint32_t sign(uint32_t temp)
{ 
    uint32_t result=temp>>31;
    return result;
}

void set_CF_add(uint32_t result,uint32_t src,uint32_t dest)
{
    cpu.eflags.CF=result<src||result<dest;
}

void set_PF(uint32_t result)
{
    uint32_t temp=result&0xff;
    uint32_t my_PF=0;
    for(int i=0;i<8;i++)
    {
        my_PF=temp^my_PF;
        temp=temp>>1;
    }
    //after up_doings, my_PF presents the number of '1' in temp, odd is 1 even is 0
    //so..
    //PF is odd inspect
    my_PF^=1;
    uint32_t result_PF=my_PF&0x1;
    cpu.eflags.PF=result_PF;
}

void set_ZF(uint32_t result)
{
    cpu.eflags.ZF=(result==0);
}

void set_SF(uint32_t result)
{
    cpu.eflags.SF=sign(result);
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest)
{
    uint32_t sign_src=sign(src);
    uint32_t sign_dest=sign(dest);
    uint32_t sign_result=sign(result);
    if(sign_src!=sign_dest)
    {
        cpu.eflags.OF=0;
    }
    else
    {
        if(sign_src==sign_result)
            cpu.eflags.OF=0;
        else
            cpu.eflags.OF=1;
    }
}

uint32_t alu_add(uint32_t src, uint32_t dest) {
	uint32_t result=src+dest;
	//setFlags
    set_CF_add(result,src,dest);
    set_PF(result);
    set_ZF(result);
    set_SF(result);
    set_OF_add(result,src,dest);
    return result;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}


uint32_t alu_sub(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
}



