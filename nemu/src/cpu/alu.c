#include "cpu/cpu.h"

/*
 * this function has been in alu.h 
uint32_t sign(uint32_t temp)
{ 
    uint32_t result=temp>>31;
    return result;
}
*/
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

// this is wrong
/*
void set_CF_adc(uint32_t& result,uint32_t src,uint32_t dest)
{
    if(cpu.eflags.CF==1)
    {
        result+=1;
        set_CF_add(result,1,result-1);
    }
    else
        set_CF_add(result,src,dest);
   
}
*/


//this is no need
/*
void set_OF_adc(uint32_t result,uint32_t src,uint32_t dest)
{
    dest+=cpu.eflags.CF;
    //if funcion has a function , so the effiency is low???
    //maybe this place can have a better choice
    set_OF_add(result,src,dest);
}
*/


uint32_t alu_adc(uint32_t src, uint32_t dest) {
    //represent add with cin
    //cin is what? 
	uint32_t result=src+dest;
    if(cpu.eflags.CF==1)
    {
        //three numbers sum 
        //their CF is different
        //for example 1111 1111 1  1111
        //the raw solution is wrong
        set_CF_add(result,src,dest);
        result+=1;
        if(cpu.eflags.CF==0)
        {
            set_CF_add(result,1,result-1);
        }
    }
    else
        set_CF_add(result,src,dest);
    set_PF(result);
    set_ZF(result);
    set_SF(result);
    set_OF_add(result,src,dest+cpu.eflags.CF);
    return result;
}

void set_CF_sub(uint32_t result,uint32_t src,uint32_t dest)
{
    //not sure downhere is right
//    set_CF_add(result,-src,dest);
    uint32_t temp=~src;
    uint32_t temp_result=temp+dest;
    set_CF_add(temp_result,temp,dest);
    //divide three numbers sum to two numbers
    if(cpu.eflags.CF!=1)
    {
        set_CF_add(result,temp_result,1);
    }
    cpu.eflags.CF=!cpu.eflags.CF;
}


void set_OF_sub(uint32_t result,uint32_t src,uint32_t dest)
{
    uint32_t sign_dest=dest>>31;
    uint32_t sign_src=src>>31;
    uint32_t sign_result=result>>31;
    if(sign_dest!=sign_src&&sign_dest!=sign_result)
        cpu.eflags.OF=1;
    else
        cpu.eflags.OF=0;
}
uint32_t alu_sub(uint32_t src, uint32_t dest)
{
	uint32_t result=dest-src;
    set_CF_sub(result,src,dest);
    set_PF(result);
    set_ZF(result);
    set_SF(result);
    set_OF_sub(result,src,dest);
    return result;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	//if CF is 1 , result-1
    uint32_t result=dest-src;

    //divide the sub-work
    if(cpu.eflags.CF==1)
    {
        set_CF_sub(result,src,dest);
        set_OF_sub(result,src,dest); 
        uint32_t temp=result;
        result=result-1;
        if(cpu.eflags.CF!=1)
        {
            set_CF_sub(result,1,temp);
            set_OF_sub(result,1,temp);
        }
    }
    else
    {
        set_CF_sub(result,src,dest);
        set_OF_sub(result,src,dest);
    }
    set_PF(result);
    set_ZF(result);
    set_SF(result);
    return result;
}

// here we don't care about the result puts to which register
// note that
//              OF  SF ZF PF CF
// imul         M   -  -  -  M
// mul          M   -  -  -  M
// div          -   -  -  -  -
// idiv         -   -  -  -  -              "-"represents undefined ,"M"represents determined by op 
uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	uint64_t result=(uint64_t)src*(uint64_t)dest;
    uint64_t temp=result>>data_size;
    cpu.eflags.OF=cpu.eflags.CF=(temp!=0);
    return result;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	int64_t result=(int32_t)src*(int32_t)dest;
//    uint32_t sign_result_lowHalf=(result>>(data_size-1))&0x1;
    
	return result;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	uint32_t result= dest/src;
	return result;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	uint32_t result=dest/src;
	return ;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
    uint32_t result=dest%src;
	return result;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	int32_t result=dest%src;
	return result;
}

void set_flags_of_logic_Function(uint32_t result)
{
    cpu.eflags.OF=cpu.eflags.CF=0;
    cpu.eflags.ZF=(result==0);
    cpu.eflags.SF=(sign(result)==1);
    set_PF(result);
}

uint32_t alu_and(uint32_t src, uint32_t dest) {

    uint32_t result=src&dest;
    set_flags_of_logic_Function(result);
    return result;

}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t result=src^dest;
    set_flags_of_logic_Function(result);
    return result;
}

uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t result=src|dest;
    set_flags_of_logic_Function(result);
	return result;
}

void set_flags_of_shift_Functions(uint32_t result,size_t data_size)
{
    set_PF(result);
    set_ZF(result);
    if(data_size==8)
        cpu.eflags.SF=(result>>7)&0x1;
    else if(data_size==16)
        cpu.eflags.SF=(result>>15)&0x1;
    else if(data_size==32)
        cpu.eflags.SF=(result>>31)&0x1;
}

/*
 * in insturctions 
 * 
 *
 *                          OF  SF  ZF  PF  CF
 * SAL/SAR/SHL/SHR 1        M   M   M   M   M
 * ASL/ASR/SHL/SHR COUNT    -   M   M   M   M
 *
 * M represents determines by op
 * - represents undefined 
 * notes that this OF flag here is not tested in case
 */
uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	//represent dest logically left shift src
    //data_size represent the length of the op
    //so OF is not cared here!!
    uint32_t result=dest;
    uint32_t temp=dest;
    if(data_size==8)
        temp=temp&0xff;
    else if(data_size==16)
        temp=temp&0xffff;
    

    uint32_t shift_sign=0;
    for(uint32_t i=0;i<src;i++)
    {
        if(data_size==8)
            shift_sign=temp&0x80;
        else if(data_size==16)
            shift_sign=temp&0x8000;
        else if(data_size==32)
            shift_sign=sign(temp);
        temp=temp<<1;
    }
    cpu.eflags.CF=(shift_sign!=0);
    
    if(data_size==8)
    {
        temp=temp&0xff;
        result=result&0xffffff00;
    }
    else if(data_size==16)
    {
        temp=temp&0xffff;
        result=result&0xffff0000;
    }
    else if(data_size==32)
    {
        result=result&0x0;
    }


    result=result|temp;
    set_flags_of_shift_Functions(temp,data_size);    
    return result;
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	
    uint32_t result=dest;
    uint32_t temp=dest;
    if(data_size==8)
         temp=temp&0xff;
    else if(data_size==16)
        temp=temp&0xffff;
    
// vim 's operations to delete       esc : line number , line number d   represents 
// delete these lines
// esc: line number, line number copy line number 
// represents copy these lines to the line

    uint32_t shift_sign=0;
    for(uint32_t i=0;i<src;i++)
    {
        shift_sign=temp&0x1;
        temp=temp>>1;
    }
    cpu.eflags.CF=(shift_sign!=0);
    
    if(data_size==8)
    {
        temp=temp&0xff;
        result=result&0xffffff00;
    }
    else if(data_size==16)
    {
        temp=temp&0xffff;
        result=result&0xffff0000;
    }
    else if(data_size==32)
    {
        result=result&0x0;
    }

    result=result|temp;
    set_flags_of_shift_Functions(temp,data_size);
    return result;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	
    uint32_t result=dest;
    uint32_t temp=dest;
    uint32_t hign_sign=0;
    if(data_size==8)
    {
         temp=temp&0xff;
         hign_sign=temp&0x80;
    }
    else if(data_size==16)
    {
        temp=temp&0xffff;
        hign_sign=temp&0x8000;
    }
    else if(data_size==32)
        hign_sign=temp&0x80000000;
    
    uint32_t shift_sign=0;
    for(uint32_t i=0;i<src;i++)
    {
        shift_sign=temp&0x1;
        uint32_t val=hign_sign;
        hign_sign=(hign_sign>>1)|val;
        temp=temp>>1;
    }
    if(hign_sign>0)     // note we need return his sign
    {
        temp=temp|hign_sign;
    }
    cpu.eflags.CF=(shift_sign!=0);
    
    if(data_size==8)
    {
        temp=temp&0xff;
        result=result&0xffffff00;
    }
    else if(data_size==16)
    {
        temp=temp&0xffff;
        result=result&0xffff0000;
    }
    else if(data_size==32)
    {
        result=result&0x0;
    }


    result=result|temp;
    set_flags_of_shift_Functions(temp,data_size);
    return result;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	return alu_shl(src,dest,data_size);
}



