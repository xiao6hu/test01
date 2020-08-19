
#ifndef __CUSTOM_TYPE_H
#define __CUSTOM_TYPE_H




    /* exact-width signed integer types */
typedef   signed          char s8,S8;
typedef   signed short     int s16,S16;
typedef   signed           long s32,S32;
//typedef   signed       __INT64 s64,S64;

    /* exact-width unsigned integer types */
typedef unsigned          char u8,U8;
typedef unsigned short     int u16,U16;
typedef unsigned           long u32,U32;
//typedef unsigned       __INT64 u64,U64;


#define BIT(bit) ((unsigned long)1<<bit)




#endif

