/*************************************************************
File: typedef.h
Author:Juntham
Discriptor:
    数据类型重定义
Version:
Date：
*************************************************************/
#ifndef __typedef_h__
#define __typedef_h__

#define BIT(bit) ((unsigned long)1<<bit)

#define true  1
#define false 0
#define TRUE  1
#define FALSE 0	

#define _CONST_ const
	
//typedef unsigned char	bool,  	BOOL;

//typedef char BYTE;


typedef unsigned char u8;  	
//typedef unsigned char uint8;	
typedef unsigned char U8;

typedef signed char	s8; 	
//typedef signed char	int8;  
typedef signed char S8;

typedef unsigned short u16;
//typedef unsigned short uint16;	
typedef unsigned short U16;

typedef signed short s16;
//typedef signed short int16;
typedef signed short S16;

typedef unsigned long u32;
//typedef unsigned int uint32;	
typedef unsigned long U32;

typedef signed long s32;
//typedef signed int	int32;

typedef unsigned long	u64;
//typedef unsigned long	uint64;
typedef signed long		s64;
//typedef unsigned long	int64;






//-----------------------------------------------
typedef unsigned char       u1t; 
typedef signed char         s1t; 
typedef unsigned short int  u2t; 
typedef signed short int    s2t; 
typedef unsigned long  int  u4t; 
typedef signed long int     s4t; 
typedef float               f4t; 
typedef double              f8t; 




/* union */
typedef struct
{
  u1t b0 : 1;                          //0
  u1t b1 : 1;                          //1
  u1t b2 : 1;                          //2     
  u1t b3 : 1;                          //3
  u1t b4 : 1;                          //4
  u1t b5 : 1;                          //5
  u1t b6 : 1;                          //6
  u1t b7 : 1;                          //7
}structbit8_t;

typedef struct
{
  u1t b0 : 1;                         //0
  u1t b1 : 1;                         //1
  u1t b2 : 1;                         //2     
  u1t b3 : 1;                         //3
  u1t b4 : 1;                         //4
  u1t b5 : 1;                         //5
  u1t b6 : 1;                         //6
  u1t b7 : 1;                         //7

  u1t b8 : 1;                         //8
  u1t b9 : 1;                         //9
  u1t b10 : 1;                         //10     
  u1t b11 : 1;                         //11
  u1t b12 : 1;                         //12
  u1t b13 : 1;                         //13
  u1t b14 : 1;                         //14
  u1t b15 : 1;                         //15
}structbit16_t;


typedef struct
{
  u1t u1l : 4;                          //0
  u1t u1h : 4;                          //1
}structbit4x2_t;

typedef struct
{
  u1t ll : 4;                          //0
  u1t lh : 4;                          //1    

  u1t hl : 4;                          //0
  u1t hh : 4;                          //1    
}structbit4x4_t;

typedef struct
{
  u1t l1l : 4;                          //1
  u1t l1h : 4;                          //0

  u1t l0l : 4;                          //1
  u1t l0h : 4;                          //0

  u1t h0l : 4;                          //1
  u1t h0h : 4;                          //0

  u1t h1l : 4;                          //1    
  u1t h1h : 4;                          //0
}structbit4x8_t;


typedef union
{
  structbit8_t structbit8;
  s1t s1;
  u1t u1;
  structbit4x2_t structbit4x2;
}union8_t;


typedef struct
{
  u1t l;                               
  u1t h;                               
}structint8x2_t;

typedef union
{
  s2t s2;
  u2t u2;
  s1t s1[2];
  u1t u1[2];                        //u1[1]????
  structint8x2_t u1x2;
  //structbit16_t structbit16;
  //structbit4x4_t structbit4x4;
}union16_t; 

   
typedef struct
{
  union16_t lWord;
  union16_t hWord;
}structint16x2_t;     //For C++


typedef struct
{
  u1t ll;
  u1t l;
  u1t h;
  u1t hh;
}structint8x4_t;

typedef union
{
  s4t s4;    
  u4t u4;    
  f4t f4;
  s1t s1[4];
  u1t u1[4];                        //u1[3]?????
  structint16x2_t u2x2;
  structint8x4_t u1x4;
  structbit4x8_t structbit4x8;
}union32_t;


typedef struct
{
  union32_t lWord;
  union32_t hWord;
}structint32x2_t;         //For C++

typedef struct
{
  u1t l3;
  u1t l2;
  u1t l1;
  u1t l0;
  u1t h0;
  u1t h1;
  u1t h2;
  u1t h3;
}structint8x8_t;
/*
typedef union
{
  s8t int64s;    
  u8t int64u;    
  f8t flot64;
  s1t s1[8];    
  u1t u1[8];                 //u1[7]?????       
  structint32x2_t u4x2;
  structint8x8_t u1x8;
}union64_t;
*/

#endif  /* __typedef_h__ */
