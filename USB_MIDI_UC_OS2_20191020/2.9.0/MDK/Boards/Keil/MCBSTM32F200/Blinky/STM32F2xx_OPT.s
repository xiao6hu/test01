;/*****************************************************************************/
;/* STM32F2xx_OPT.s: STM32F2xx Flash Option Bytes                             */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2012 Keil Software. All rights reserved.                    */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e> Flash Option Bytes
FLASH_OPT       EQU     1

;// <h> Flash Read Protection
;//     <i> Read protection is used to protect the software code stored in Flash memory
;//   <o0> Read Protection Level
;//     <i> Level 0: No Protection 
;//     <i> Level 1: Read Protection of Memories (debug features limited)
;//     <i> Level 2: Chip Protection (debug and boot in RAM features disabled)
;//          <0xAA=> Level 0 (No Protection) 
;//          <0x00=> Level 1 (Read Protection of Memories)
;//          <0xCC=> Level 2 (Chip Protection)
;// </h>
RDP            EQU     0xAA
nRDP           EQU     RDP:EOR:0xFF

;// <h> Flash Write Protection
;//   <o0.0> Sector 0
;//   <o0.1> Sector 1
;//   <o0.2> Sector 2
;//   <o0.3> Sector 3
;//   <o0.4> Sector 4
;//   <o0.5> Sector 5
;//   <o0.6> Sector 6
;//   <o0.7> Sector 7
;//   <o1.0> Sector 8
;//   <o1.1> Sector 9
;//   <o1.2> Sector 10
;//   <o1.3> Sector 11
;// </h>
WRP0           EQU     0x00
WRP1           EQU     0x00
nWRP0          EQU     WRP0:EOR:0xFF
nWRP1          EQU     WRP1:EOR:0xFF

;// <h> User Configuration
;//   <o0.2..3> BOR_LEV     
;//          <0=> BOR Level 3 (VBOR3). Reset threshold level from 2.70 to 3.60 V
;//          <1=> BOR Level 2 (VBOR2). Reset threshold level from 2.40 to 2.70 V
;//          <2=> BOR Level 1 (VBOR1). Reset threshold level from 2.10 to 2.40 V
;//          <3=> BOR off     (VBOR0). Reset threshold level from 1.80 to 2.10 V
;//   <o0.5> WDG_SW     
;//          <0=> HW Watchdog <1=> SW Watchdog
;//   <o0.6> nRST_STOP  <i> Generate Reset when entering STOP Mode
;//          <0=> Enabled <1=> Disabled
;//   <o0.7> nRST_STDBY <i> Generate Reset when entering Standby Mode
;//          <0=> Enabled <1=> Disabled
;// </h>
USER           EQU     0xFF
nUSER          EQU     USER:EOR:0xFF

;// </e>


                IF      FLASH_OPT <> 0
                AREA    |.ARM.__AT_0x1FFFC000|, CODE, READONLY
                DCB      USER,   RDP,  nUSER, nRDP,   USER,   RDP,  nUSER, nRDP
                DCB     nWRP0,  nWRP1,  WRP0,  WRP1, nWRP0,  nWRP1,  WRP0,  WRP1
                ENDIF

                END
