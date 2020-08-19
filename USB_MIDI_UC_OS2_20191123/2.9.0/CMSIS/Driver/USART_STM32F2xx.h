/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2018 Arm Limited
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        12. March
 * $Revision:    V2.6
 *
 * Project:      USART Driver definitions for ST STM32F2xx
 * -------------------------------------------------------------------------- */

#ifndef __USART_STM32F2XX_H
#define __USART_STM32F2XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Driver_USART.h"
#include "stm32f2xx_hal.h"

#include "RTE_Components.h"
#if   defined(RTE_DEVICE_FRAMEWORK_CLASSIC)
#include "RTE_Device.h"
#elif defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #define  VM_ASYNC                      (1UL)
  #define  VM_SYNC                       (2UL)
  #define  VM_IRDA                       (3UL)
  #define  VM_SMARTCARD                  (4UL)
  #define  Asynchronous                  VM_ASYNC
  #define  IrDA                          VM_IRDA
#include "MX_Device.h"
#else
#error "::Device:STM32Cube Framework: not selected in RTE"
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  #if ((defined(RTE_Drivers_USART1) || \
        defined(RTE_Drivers_USART2) || \
        defined(RTE_Drivers_USART3) || \
        defined(RTE_Drivers_UART4 ) || \
        defined(RTE_Drivers_UART5 ) || \
        defined(RTE_Drivers_USART6))   \
       && (RTE_USART1 == 0)            \
       && (RTE_USART2 == 0)            \
       && (RTE_USART3 == 0)            \
       && (RTE_UART4  == 0)            \
       && (RTE_UART5  == 0)            \
       && (RTE_USART6 == 0))
    #error "USART not configured in RTE_Device.h!"
  #endif

// RTE macros
#define _DMA_CHANNEL_x(x)               DMA_CHANNEL_##x
#define  DMA_CHANNEL_x(x)              _DMA_CHANNEL_x(x)

#define  DMA_PRIORITY(x)              ((x == 0) ? DMA_PRIORITY_LOW    : \
                                       (x == 1) ? DMA_PRIORITY_MEDIUM : \
                                       (x == 2) ? DMA_PRIORITY_HIGH   : \
                                                  DMA_PRIORITY_VERY_HIGH)

#define _DMAx_STREAMy(x, y)             DMA##x##_Stream##y
#define  DMAx_STREAMy(x, y)            _DMAx_STREAMy(x, y)

#define _DMAx_STREAMy_IRQ(x, y)         DMA##x##_Stream##y##_IRQHandler
#define  DMAx_STREAMy_IRQ(x, y)        _DMAx_STREAMy_IRQ(x, y)

#define _DMAx_STREAMy_IRQn(x, y)        DMA##x##_Stream##y##_IRQn
#define  DMAx_STREAMy_IRQn(x, y)       _DMAx_STREAMy_IRQn(x, y)

// USART1 configuration definitions
#if (RTE_USART1 == 1)
  #define MX_USART1

  #if (RTE_USART1_RX_DMA == 1)
    #define MX_USART1_RX_DMA_Instance DMAx_STREAMy(RTE_USART1_RX_DMA_NUMBER, RTE_USART1_RX_DMA_STREAM)
    #define MX_USART1_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART1_RX_DMA_NUMBER, RTE_USART1_RX_DMA_STREAM)
    #define MX_USART1_RX_DMA_Channel  DMA_CHANNEL_x(RTE_USART1_RX_DMA_CHANNEL)
    #define MX_USART1_RX_DMA_Priority DMA_PRIORITY(RTE_USART1_RX_DMA_PRIORITY)

    #define USART1_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART1_RX_DMA_NUMBER, RTE_USART1_RX_DMA_STREAM)
  #endif
  #if (RTE_USART1_TX_DMA == 1)
    #define MX_USART1_TX_DMA_Instance DMAx_STREAMy(RTE_USART1_TX_DMA_NUMBER, RTE_USART1_TX_DMA_STREAM)
    #define MX_USART1_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART1_TX_DMA_NUMBER, RTE_USART1_TX_DMA_STREAM)
    #define MX_USART1_TX_DMA_Channel  DMA_CHANNEL_x(RTE_USART1_TX_DMA_CHANNEL)
    #define MX_USART1_TX_DMA_Priority DMA_PRIORITY(RTE_USART1_TX_DMA_PRIORITY)

    #define USART1_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART1_TX_DMA_NUMBER, RTE_USART1_TX_DMA_STREAM)
  #endif

  #ifndef USART1_GPIO_SPEED_FREQ
    #define USART1_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_USART1_TX == 1)
    #define MX_USART1_TX_Pin        1
    #define MX_USART1_TX_GPIOx      RTE_USART1_TX_PORT
    #define MX_USART1_TX_GPIO_Pin   (1U << RTE_USART1_TX_BIT)
    #define MX_USART1_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART1_TX_GPIO_Speed USART1_GPIO_SPEED_FREQ
    #define MX_USART1_TX_GPIO_AF    GPIO_AF7_USART1
  #endif

  #if (RTE_USART1_RX == 1)
    #define MX_USART1_RX_Pin        1
    #define MX_USART1_RX_GPIOx      RTE_USART1_RX_PORT
    #define MX_USART1_RX_GPIO_Pin   (1U << RTE_USART1_RX_BIT)
    #define MX_USART1_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART1_RX_GPIO_Speed USART1_GPIO_SPEED_FREQ
    #define MX_USART1_RX_GPIO_AF    GPIO_AF7_USART1
  #endif

  #if (RTE_USART1_CK == 1)
    #define MX_USART1_CK_Pin        1
    #define MX_USART1_CK_GPIOx      RTE_USART1_CK_PORT
    #define MX_USART1_CK_GPIO_Pin   (1U << RTE_USART1_CK_BIT)
    #define MX_USART1_CK_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART1_CK_GPIO_Speed USART1_GPIO_SPEED_FREQ
    #define MX_USART1_CK_GPIO_AF    GPIO_AF7_USART1
  #endif


  #if (RTE_USART1_RTS == 1)
    #define MX_USART1_RTS_Pin        1
    #define MX_USART1_RTS_GPIOx      RTE_USART1_RTS_PORT
    #define MX_USART1_RTS_GPIO_Pin   (1U << RTE_USART1_RTS_BIT)
    #define MX_USART1_RTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART1_RTS_GPIO_Speed USART1_GPIO_SPEED_FREQ
    #define MX_USART1_RTS_GPIO_AF    GPIO_AF7_USART1
  #endif

  #if (RTE_USART1_CTS == 1)
    #define MX_USART1_CTS_Pin        1
    #define MX_USART1_CTS_GPIOx      RTE_USART1_CTS_PORT
    #define MX_USART1_CTS_GPIO_Pin   (1U << RTE_USART1_CTS_BIT)
    #define MX_USART1_CTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART1_CTS_GPIO_Speed USART1_GPIO_SPEED_FREQ
    #define MX_USART1_CTS_GPIO_AF    GPIO_AF7_USART1
  #endif
#endif

// USART2 configuration definitions
#if (RTE_USART2 == 1)
  #define MX_USART2

  #if (RTE_USART2_RX_DMA == 1)
    #define MX_USART2_RX_DMA_Instance DMAx_STREAMy(RTE_USART2_RX_DMA_NUMBER, RTE_USART2_RX_DMA_STREAM)
    #define MX_USART2_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART2_RX_DMA_NUMBER, RTE_USART2_RX_DMA_STREAM)
    #define MX_USART2_RX_DMA_Channel  DMA_CHANNEL_x(RTE_USART2_RX_DMA_CHANNEL)
    #define MX_USART2_RX_DMA_Priority DMA_PRIORITY(RTE_USART2_RX_DMA_PRIORITY)

    #define USART2_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART2_RX_DMA_NUMBER, RTE_USART2_RX_DMA_STREAM)
  #endif
  #if (RTE_USART2_TX_DMA == 1)
    #define MX_USART2_TX_DMA_Instance DMAx_STREAMy(RTE_USART2_TX_DMA_NUMBER, RTE_USART2_TX_DMA_STREAM)
    #define MX_USART2_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART2_TX_DMA_NUMBER, RTE_USART2_TX_DMA_STREAM)
    #define MX_USART2_TX_DMA_Channel  DMA_CHANNEL_x(RTE_USART2_TX_DMA_CHANNEL)
    #define MX_USART2_TX_DMA_Priority DMA_PRIORITY(RTE_USART2_TX_DMA_PRIORITY)

    #define USART2_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART2_TX_DMA_NUMBER, RTE_USART2_TX_DMA_STREAM)
  #endif

  #ifndef USART2_GPIO_SPEED_FREQ
    #define USART2_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_USART2_TX == 1)
    #define MX_USART2_TX_Pin        1
    #define MX_USART2_TX_GPIOx      RTE_USART2_TX_PORT
    #define MX_USART2_TX_GPIO_Pin   (1U << RTE_USART2_TX_BIT)
    #define MX_USART2_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART2_TX_GPIO_Speed USART2_GPIO_SPEED_FREQ
    #define MX_USART2_TX_GPIO_AF    GPIO_AF7_USART2
  #endif

  #if (RTE_USART2_RX == 1)
    #define MX_USART2_RX_Pin        1
    #define MX_USART2_RX_GPIOx      RTE_USART2_RX_PORT
    #define MX_USART2_RX_GPIO_Pin   (1U << RTE_USART2_RX_BIT)
    #define MX_USART2_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART2_RX_GPIO_Speed USART2_GPIO_SPEED_FREQ
    #define MX_USART2_RX_GPIO_AF    GPIO_AF7_USART2
  #endif

  #if (RTE_USART2_CK == 1)
    #define MX_USART2_CK_Pin        1
    #define MX_USART2_CK_GPIOx      RTE_USART2_CK_PORT
    #define MX_USART2_CK_GPIO_Pin   (1U << RTE_USART2_CK_BIT)
    #define MX_USART2_CK_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART2_CK_GPIO_Speed USART2_GPIO_SPEED_FREQ
    #define MX_USART2_CK_GPIO_AF    GPIO_AF7_USART2
  #endif


  #if (RTE_USART2_RTS == 1)
    #define MX_USART2_RTS_Pin        1
    #define MX_USART2_RTS_GPIOx      RTE_USART2_RTS_PORT
    #define MX_USART2_RTS_GPIO_Pin   (1U << RTE_USART2_RTS_BIT)
    #define MX_USART2_RTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART2_RTS_GPIO_Speed USART2_GPIO_SPEED_FREQ
    #define MX_USART2_RTS_GPIO_AF    GPIO_AF7_USART2
  #endif

  #if (RTE_USART2_CTS == 1)
    #define MX_USART2_CTS_Pin        1
    #define MX_USART2_CTS_GPIOx      RTE_USART2_CTS_PORT
    #define MX_USART2_CTS_GPIO_Pin   (1U << RTE_USART2_CTS_BIT)
    #define MX_USART2_CTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART2_CTS_GPIO_Speed USART2_GPIO_SPEED_FREQ
    #define MX_USART2_CTS_GPIO_AF    GPIO_AF7_USART2
  #endif
#endif

// USART3 configuration definitions
#if (RTE_USART3 == 1)

  #ifndef USART3
    #error "USART3 not available for selected device!"
  #endif

  #define MX_USART3

  #if (RTE_USART3_RX_DMA == 1)
    #define MX_USART3_RX_DMA_Instance DMAx_STREAMy(RTE_USART3_RX_DMA_NUMBER, RTE_USART3_RX_DMA_STREAM)
    #define MX_USART3_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART3_RX_DMA_NUMBER, RTE_USART3_RX_DMA_STREAM)
    #define MX_USART3_RX_DMA_Channel  DMA_CHANNEL_x(RTE_USART3_RX_DMA_CHANNEL)
    #define MX_USART3_RX_DMA_Priority DMA_PRIORITY(RTE_USART3_RX_DMA_PRIORITY)

    #define USART3_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART3_RX_DMA_NUMBER, RTE_USART3_RX_DMA_STREAM)
  #endif
  #if (RTE_USART3_TX_DMA == 1)
    #define MX_USART3_TX_DMA_Instance DMAx_STREAMy(RTE_USART3_TX_DMA_NUMBER, RTE_USART3_TX_DMA_STREAM)
    #define MX_USART3_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART3_TX_DMA_NUMBER, RTE_USART3_TX_DMA_STREAM)
    #define MX_USART3_TX_DMA_Channel  DMA_CHANNEL_x(RTE_USART3_TX_DMA_CHANNEL)
    #define MX_USART3_TX_DMA_Priority DMA_PRIORITY(RTE_USART3_TX_DMA_PRIORITY)

    #define USART3_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART3_TX_DMA_NUMBER, RTE_USART3_TX_DMA_STREAM)
  #endif

  #ifndef USART3_GPIO_SPEED_FREQ
    #define USART3_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_USART3_TX == 1)
    #define MX_USART3_TX_Pin        1
    #define MX_USART3_TX_GPIOx      RTE_USART3_TX_PORT
    #define MX_USART3_TX_GPIO_Pin   (1U << RTE_USART3_TX_BIT)
    #define MX_USART3_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART3_TX_GPIO_Speed USART3_GPIO_SPEED_FREQ
    #define MX_USART3_TX_GPIO_AF    GPIO_AF7_USART3
  #endif

  #if (RTE_USART3_RX == 1)
    #define MX_USART3_RX_Pin        1
    #define MX_USART3_RX_GPIOx      RTE_USART3_RX_PORT
    #define MX_USART3_RX_GPIO_Pin   (1U << RTE_USART3_RX_BIT)
    #define MX_USART3_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART3_RX_GPIO_Speed USART3_GPIO_SPEED_FREQ
    #define MX_USART3_RX_GPIO_AF    GPIO_AF7_USART3
  #endif

  #if (RTE_USART3_CK == 1)
    #define MX_USART3_CK_Pin        1
    #define MX_USART3_CK_GPIOx      RTE_USART3_CK_PORT
    #define MX_USART3_CK_GPIO_Pin   (1U << RTE_USART3_CK_BIT)
    #define MX_USART3_CK_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART3_CK_GPIO_Speed USART3_GPIO_SPEED_FREQ
    #define MX_USART3_CK_GPIO_AF    GPIO_AF7_USART3
  #endif


  #if (RTE_USART3_RTS == 1)
    #define MX_USART3_RTS_Pin        1
    #define MX_USART3_RTS_GPIOx      RTE_USART3_RTS_PORT
    #define MX_USART3_RTS_GPIO_Pin   (1U << RTE_USART3_RTS_BIT)
    #define MX_USART3_RTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART3_RTS_GPIO_Speed USART3_GPIO_SPEED_FREQ
    #define MX_USART3_RTS_GPIO_AF    GPIO_AF7_USART3
  #endif

  #if (RTE_USART3_CTS == 1)
    #define MX_USART3_CTS_Pin        1
    #define MX_USART3_CTS_GPIOx      RTE_USART3_CTS_PORT
    #define MX_USART3_CTS_GPIO_Pin   (1U << RTE_USART3_CTS_BIT)
    #define MX_USART3_CTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART3_CTS_GPIO_Speed USART3_GPIO_SPEED_FREQ
    #define MX_USART3_CTS_GPIO_AF    GPIO_AF7_USART3
  #endif
#endif

// UART4 configuration definitions
#if (RTE_UART4 == 1)

  #ifndef UART4
    #error "UART4 not available for selected device!"
  #endif

  #define MX_UART4

  #if (RTE_UART4_RX_DMA == 1)
    #define MX_UART4_RX_DMA_Instance DMAx_STREAMy(RTE_UART4_RX_DMA_NUMBER, RTE_UART4_RX_DMA_STREAM)
    #define MX_UART4_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_UART4_RX_DMA_NUMBER, RTE_UART4_RX_DMA_STREAM)
    #define MX_UART4_RX_DMA_Channel  DMA_CHANNEL_x(RTE_UART4_RX_DMA_CHANNEL)
    #define MX_UART4_RX_DMA_Priority DMA_PRIORITY(RTE_UART4_RX_DMA_PRIORITY)

    #define UART4_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_UART4_RX_DMA_NUMBER, RTE_UART4_RX_DMA_STREAM)
  #endif
  #if (RTE_UART4_TX_DMA == 1)
    #define MX_UART4_TX_DMA_Instance DMAx_STREAMy(RTE_UART4_TX_DMA_NUMBER, RTE_UART4_TX_DMA_STREAM)
    #define MX_UART4_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_UART4_TX_DMA_NUMBER, RTE_UART4_TX_DMA_STREAM)
    #define MX_UART4_TX_DMA_Channel  DMA_CHANNEL_x(RTE_UART4_TX_DMA_CHANNEL)
    #define MX_UART4_TX_DMA_Priority DMA_PRIORITY(RTE_UART4_TX_DMA_PRIORITY)

    #define UART4_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_UART4_TX_DMA_NUMBER, RTE_UART4_TX_DMA_STREAM)
  #endif

  #ifndef UART4_GPIO_SPEED_FREQ
    #define UART4_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_UART4_TX == 1)
    #define MX_UART4_TX_Pin        1
    #define MX_UART4_TX_GPIOx      RTE_UART4_TX_PORT
    #define MX_UART4_TX_GPIO_Pin   (1U << RTE_UART4_TX_BIT)
    #define MX_UART4_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_UART4_TX_GPIO_Speed UART4_GPIO_SPEED_FREQ
    #define MX_UART4_TX_GPIO_AF    GPIO_AF8_UART4
  #endif

  #if (RTE_UART4_RX == 1)
    #define MX_UART4_RX_Pin        1
    #define MX_UART4_RX_GPIOx      RTE_UART4_RX_PORT
    #define MX_UART4_RX_GPIO_Pin   (1U << RTE_UART4_RX_BIT)
    #define MX_UART4_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_UART4_RX_GPIO_Speed UART4_GPIO_SPEED_FREQ
    #define MX_UART4_RX_GPIO_AF    GPIO_AF8_UART4
  #endif
#endif

// UART5 configuration definitions
#if (RTE_UART5 == 1)

  #ifndef UART5
    #error "UART5 not available for selected device!"
  #endif

  #define MX_UART5

  #if (RTE_UART5_RX_DMA == 1)
    #define MX_UART5_RX_DMA_Instance DMAx_STREAMy(RTE_UART5_RX_DMA_NUMBER, RTE_UART5_RX_DMA_STREAM)
    #define MX_UART5_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_UART5_RX_DMA_NUMBER, RTE_UART5_RX_DMA_STREAM)
    #define MX_UART5_RX_DMA_Channel  DMA_CHANNEL_x(RTE_UART5_RX_DMA_CHANNEL)
    #define MX_UART5_RX_DMA_Priority DMA_PRIORITY(RTE_UART5_RX_DMA_PRIORITY)

    #define UART5_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_UART5_RX_DMA_NUMBER, RTE_UART5_RX_DMA_STREAM)
  #endif
  #if (RTE_UART5_TX_DMA == 1)
    #define MX_UART5_TX_DMA_Instance DMAx_STREAMy(RTE_UART5_TX_DMA_NUMBER, RTE_UART5_TX_DMA_STREAM)
    #define MX_UART5_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_UART5_TX_DMA_NUMBER, RTE_UART5_TX_DMA_STREAM)
    #define MX_UART5_TX_DMA_Channel  DMA_CHANNEL_x(RTE_UART5_TX_DMA_CHANNEL)
    #define MX_UART5_TX_DMA_Priority DMA_PRIORITY(RTE_UART5_TX_DMA_PRIORITY)

    #define UART5_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_UART5_TX_DMA_NUMBER, RTE_UART5_TX_DMA_STREAM)
  #endif

  #ifndef UART5_GPIO_SPEED_FREQ
    #define UART5_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_UART5_TX == 1)
    #define MX_UART5_TX_Pin        1
    #define MX_UART5_TX_GPIOx      RTE_UART5_TX_PORT
    #define MX_UART5_TX_GPIO_Pin   (1U << RTE_UART5_TX_BIT)
    #define MX_UART5_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_UART5_TX_GPIO_Speed UART5_GPIO_SPEED_FREQ
    #define MX_UART5_TX_GPIO_AF    GPIO_AF8_UART5
  #endif

  #if (RTE_UART5_RX == 1)
    #define MX_UART5_RX_Pin        1
    #define MX_UART5_RX_GPIOx      RTE_UART5_RX_PORT
    #define MX_UART5_RX_GPIO_Pin   (1U << RTE_UART5_RX_BIT)
    #define MX_UART5_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_UART5_RX_GPIO_Speed UART5_GPIO_SPEED_FREQ
    #define MX_UART5_RX_GPIO_AF    GPIO_AF8_UART5
  #endif
#endif

// USART6 configuration definitions
#if (RTE_USART6 == 1)
  #define MX_USART6

  #ifndef USART6
    #error "USART6 not available for selected device!"
  #endif

  #if (RTE_USART6_RX_DMA == 1)
    #define MX_USART6_RX_DMA_Instance DMAx_STREAMy(RTE_USART6_RX_DMA_NUMBER, RTE_USART6_RX_DMA_STREAM)
    #define MX_USART6_RX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART6_RX_DMA_NUMBER, RTE_USART6_RX_DMA_STREAM)
    #define MX_USART6_RX_DMA_Channel  DMA_CHANNEL_x(RTE_USART6_RX_DMA_CHANNEL)
    #define MX_USART6_RX_DMA_Priority DMA_PRIORITY(RTE_USART6_RX_DMA_PRIORITY)

    #define USART6_RX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART6_RX_DMA_NUMBER, RTE_USART6_RX_DMA_STREAM)
  #endif
  #if (RTE_USART6_TX_DMA == 1)
    #define MX_USART6_TX_DMA_Instance DMAx_STREAMy(RTE_USART6_TX_DMA_NUMBER, RTE_USART6_TX_DMA_STREAM)
    #define MX_USART6_TX_DMA_IRQn     DMAx_STREAMy_IRQn(RTE_USART6_TX_DMA_NUMBER, RTE_USART6_TX_DMA_STREAM)
    #define MX_USART6_TX_DMA_Channel  DMA_CHANNEL_x(RTE_USART6_TX_DMA_CHANNEL)
    #define MX_USART6_TX_DMA_Priority DMA_PRIORITY(RTE_USART6_TX_DMA_PRIORITY)

    #define USART6_TX_DMA_Handler     DMAx_STREAMy_IRQ(RTE_USART6_TX_DMA_NUMBER, RTE_USART6_TX_DMA_STREAM)
  #endif

  #ifndef USART6_GPIO_SPEED_FREQ
    #define USART6_GPIO_SPEED_FREQ    GPIO_SPEED_FREQ_LOW
  #endif

  #if (RTE_USART6_TX == 1)
    #define MX_USART6_TX_Pin        1
    #define MX_USART6_TX_GPIOx      RTE_USART6_TX_PORT
    #define MX_USART6_TX_GPIO_Pin   (1U << RTE_USART6_TX_BIT)
    #define MX_USART6_TX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART6_TX_GPIO_Speed USART6_GPIO_SPEED_FREQ
    #define MX_USART6_TX_GPIO_AF    GPIO_AF8_USART6
  #endif

  #if (RTE_USART6_RX == 1)
    #define MX_USART6_RX_Pin        1
    #define MX_USART6_RX_GPIOx      RTE_USART6_RX_PORT
    #define MX_USART6_RX_GPIO_Pin   (1U << RTE_USART6_RX_BIT)
    #define MX_USART6_RX_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART6_RX_GPIO_Speed USART6_GPIO_SPEED_FREQ
    #define MX_USART6_RX_GPIO_AF    GPIO_AF8_USART6
  #endif

  #if (RTE_USART6_CK == 1)
    #define MX_USART6_CK_Pin        1
    #define MX_USART6_CK_GPIOx      RTE_USART6_CK_PORT
    #define MX_USART6_CK_GPIO_Pin   (1U << RTE_USART6_CK_BIT)
    #define MX_USART6_CK_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART6_CK_GPIO_Speed USART6_GPIO_SPEED_FREQ
    #define MX_USART6_CK_GPIO_AF    GPIO_AF8_USART6
  #endif

  #if (RTE_USART6_RTS == 1)
    #define MX_USART6_RTS_Pin        1
    #define MX_USART6_RTS_GPIOx      RTE_USART6_RTS_PORT
    #define MX_USART6_RTS_GPIO_Pin   (1U << RTE_USART6_RTS_BIT)
    #define MX_USART6_RTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART6_RTS_GPIO_Speed USART6_GPIO_SPEED_FREQ
    #define MX_USART6_RTS_GPIO_AF    GPIO_AF8_USART6
  #endif

  #if (RTE_USART6_CTS == 1)
    #define MX_USART6_CTS_Pin        1
    #define MX_USART6_CTS_GPIOx      RTE_USART6_CTS_PORT
    #define MX_USART6_CTS_GPIO_Pin   (1U << RTE_USART6_CTS_BIT)
    #define MX_USART6_CTS_GPIO_PuPd  GPIO_NOPULL
    #define MX_USART6_CTS_GPIO_Speed USART6_GPIO_SPEED_FREQ
    #define MX_USART6_CTS_GPIO_AF    GPIO_AF8_USART6
  #endif
#endif
#endif /* RTE_DEVICE_FRAMEWORK_CLASSIC */

#if defined(RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #if ((defined(RTE_Drivers_USART1) || \
        defined(RTE_Drivers_USART2) || \
        defined(RTE_Drivers_USART3) || \
        defined(RTE_Drivers_UART4 ) || \
        defined(RTE_Drivers_UART5 ) || \
        defined(RTE_Drivers_USART6))   \
        && (!defined (MX_USART1))      \
        && (!defined (MX_USART2))      \
        && (!defined (MX_USART3))      \
        && (!defined (MX_UART4 ))      \
        && (!defined (MX_UART5 ))      \
        && (!defined (MX_USART6)))
    #error "USART not configured in STM32CubeMX!"
  #endif

// MX macros
#ifdef MX_USART1

#if (MX_USART1_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_USART1_VM == VM_SYNC)
#ifndef USART_SYNC
#define USART_SYNC                       (1UL)
#endif
#elif (MX_USART1_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#elif (MX_USART1_VM == VM_SMARTCARD)
#ifndef USART_SMARTCARD
#define USART_SMARTCARD                  (1UL)
#endif
#endif
#endif


#ifdef MX_USART2

#if (MX_USART2_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_USART2_VM == VM_SYNC)
#ifndef USART_SYNC
#define USART_SYNC                       (1UL)
#endif
#elif (MX_USART2_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#elif (MX_USART2_VM == VM_SMARTCARD)
#ifndef USART_SMARTCARD
#define USART_SMARTCARD                  (1UL)
#endif
#endif
#endif


#ifdef MX_USART3

#if (MX_USART3_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_USART3_VM == VM_SYNC)
#ifndef USART_SYNC
#define USART_SYNC                       (1UL)
#endif
#elif (MX_USART3_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#elif (MX_USART3_VM == VM_SMARTCARD)
#ifndef USART_SMARTCARD
#define USART_SMARTCARD                  (1UL)
#endif
#endif
#endif


#ifdef MX_UART4

#if (MX_UART4_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_UART4_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#endif
#endif


#ifdef MX_UART5

#if (MX_UART5_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_UART5_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#endif
#endif


#ifdef MX_USART6

#if (MX_USART6_VM == VM_ASYNC)
#ifndef USART_ASYNC
#define USART_ASYNC                      (1UL)
#endif
#elif (MX_USART6_VM == VM_SYNC)
#ifndef USART_SYNC
#define USART_SYNC                       (1UL)
#endif
#elif (MX_USART6_VM == VM_IRDA)
#ifndef USART_IRDA
#define USART_IRDA                       (1UL)
#endif
#elif (MX_USART6_VM == VM_SMARTCARD)
#ifndef USART_SMARTCARD
#define USART_SMARTCARD                  (1UL)
#endif
#endif
#endif

#endif  /* RTE_DEVICE_FRAMEWORK_CUBE_MX */

#ifdef MX_USART1
#if (defined(MX_USART1_RX_DMA_Instance) || defined(MX_USART1_TX_DMA_Instance))
#ifndef MX_USART1_RX_DMA_Instance
  #error "USART1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART1_TX_DMA_Instance
  #error "USART1 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_USART2
#if (defined(MX_USART2_RX_DMA_Instance) || defined(MX_USART2_TX_DMA_Instance))
#ifndef MX_USART2_RX_DMA_Instance
  #error "USART2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART2_TX_DMA_Instance
  #error "USART2 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_USART3
#if (defined(MX_USART3_RX_DMA_Instance) || defined(MX_USART3_TX_DMA_Instance))
#ifndef MX_USART3_RX_DMA_Instance
  #error "USART3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART3_TX_DMA_Instance
  #error "USART3 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_UART4
#if (defined(MX_UART4_RX_DMA_Instance) || defined(MX_UART4_TX_DMA_Instance))
#ifndef MX_UART4_RX_DMA_Instance
  #error "UART4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_UART4_TX_DMA_Instance
  #error "UART4 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_UART5
#if (defined(MX_UART5_RX_DMA_Instance) || defined(MX_UART5_TX_DMA_Instance))
#ifndef MX_UART5_RX_DMA_Instance
  #error "UART5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_UART5_TX_DMA_Instance
  #error "UART5 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#ifdef MX_USART6
#if (defined(MX_USART6_RX_DMA_Instance) || defined(MX_USART6_TX_DMA_Instance))
#ifndef MX_USART6_RX_DMA_Instance
  #error "USART6 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#ifndef MX_USART6_TX_DMA_Instance
  #error "USART6 using DMA requires Rx and Tx DMA channel enabled in RTE_Device.h or MX_Device.h!"
#endif
#endif
#endif

#if (defined(MX_USART1_RX_DMA_Instance) || \
     defined(MX_USART2_RX_DMA_Instance) || \
     defined(MX_USART3_RX_DMA_Instance) || \
     defined(MX_UART4_RX_DMA_Instance ) || \
     defined(MX_UART5_RX_DMA_Instance ) || \
     defined(MX_USART6_RX_DMA_Instance))
#define __USART_DMA_RX
#endif
#if (defined(MX_USART1_TX_DMA_Instance) || \
     defined(MX_USART2_TX_DMA_Instance) || \
     defined(MX_USART3_TX_DMA_Instance) || \
     defined(MX_UART4_TX_DMA_Instance ) || \
     defined(MX_UART5_TX_DMA_Instance ) || \
     defined(MX_USART6_TX_DMA_Instance))
#define __USART_DMA_TX
#endif
#if (defined(__USART_DMA_RX) && defined(__USART_DMA_TX))
#define __USART_DMA
#endif

// USART BRR macro
#define USART_DIVIDER(_PCLK_, _BAUD_)           (((_PCLK_)*25)/(4*(_BAUD_)))
#define USART_DIVIDER_MANTISA(_PCLK_, _BAUD_)     (USART_DIVIDER((_PCLK_), (_BAUD_))/100)
#define USART_DIVIDER_FRACTION(_PCLK_, _BAUD_)  (((USART_DIVIDER((_PCLK_), (_BAUD_)) - (USART_DIVIDER_MANTISA((_PCLK_), (_BAUD_)) * 100)) * 16 + 50) / 100)
#define USART_BAUDRATE_DIVIDER(_PCLK_, _BAUD_)   ((USART_DIVIDER_MANTISA((_PCLK_), (_BAUD_)) << 4)|(USART_DIVIDER_FRACTION((_PCLK_), (_BAUD_)) & 0x0F))

// USART flags
#define USART_FLAG_INITIALIZED      ((uint8_t)(1U))
#define USART_FLAG_POWERED          ((uint8_t)(1U << 1))
#define USART_FLAG_CONFIGURED       ((uint8_t)(1U << 2))
#define USART_FLAG_TX_ENABLED       ((uint8_t)(1U << 3))
#define USART_FLAG_RX_ENABLED       ((uint8_t)(1U << 4))

// USART synchronous xfer modes
#define USART_SYNC_MODE_TX           ( 1UL )
#define USART_SYNC_MODE_RX           ( 2UL )
#define USART_SYNC_MODE_TX_RX        (USART_SYNC_MODE_TX | \
                                      USART_SYNC_MODE_RX)

// DMA Callback functions
typedef void (*DMA_Callback_t) (DMA_HandleTypeDef *hdma);

// USART DMA
typedef const struct _USART_DMA {
  DMA_HandleTypeDef    *hdma;           // DMA handle
  DMA_Callback_t        cb_complete;    // DMA complete callback
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  DMA_Stream_TypeDef   *stream;         // Stream register interface
  uint32_t              channel;        // DMA channel
  uint32_t              priority;       // DMA channel priority
  IRQn_Type             irq_num;        // Stream IRQ number
  uint8_t               reserved[3];    // Reserved
#endif
} USART_DMA;

// USART pin
typedef const struct _USART_PIN {
  GPIO_TypeDef         *port;           // Port
  uint16_t              pin;            // Pin
  uint16_t              af;             // Alternate function
  uint16_t              pupd;           // Pull up/down
  uint16_t              speed;          // Speed
} USART_PIN;

// USART Input/Output Configuration
typedef const struct _USART_IO {
  USART_PIN            *tx;             // TX  Pin identifier
  USART_PIN            *rx;             // RX  Pin identifier
  USART_PIN            *ck;             // CLK Pin identifier
  USART_PIN            *rts;            // RTS Pin identifier
  USART_PIN            *cts;            // CTS Pin identifier
} USART_IO;

// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
  uint32_t              rx_num;         // Total number of receive data
  uint32_t              tx_num;         // Total number of transmit data
  uint8_t              *rx_buf;         // Pointer to in data buffer
  uint8_t              *tx_buf;         // Pointer to out data buffer
  uint32_t              rx_cnt;         // Number of data received
  uint32_t              tx_cnt;         // Number of data sent
  uint16_t              dump_val;       // Variable for dumping DMA data
  uint16_t              def_val;        // Default transfer value
  uint32_t              sync_mode;      // Synchronous mode flag
  uint8_t               break_flag;     // Transmit break flag
  uint8_t               send_active;    // Send active flag
  uint16_t              reserved;       // Reserved
} USART_TRANSFER_INFO;

typedef struct _USART_STATUS {
  uint8_t tx_busy;                      // Transmitter busy flag
  uint8_t rx_busy;                      // Receiver busy flag
  uint8_t tx_underflow;                 // Transmit data underflow detected (cleared on start of next send operation)
  uint8_t rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
  uint8_t rx_break;                     // Break detected on receive (cleared on start of next receive operation)
  uint8_t rx_framing_error;             // Framing error detected on receive (cleared on start of next receive operation)
  uint8_t rx_parity_error;              // Parity error detected on receive (cleared on start of next receive operation)
} USART_STATUS;

// USART Information (Run-time)
typedef struct _USART_INFO {
  ARM_USART_SignalEvent_t cb_event;            // Event Callback
  USART_STATUS            status;              // Status flags
  uint8_t                 flags;               // Current USART flags
  uint32_t                mode;                // Current USART mode
  uint32_t                flow_control;        // Flow control
} USART_INFO;

// USART Resources definition
typedef const struct {
#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
  void                   *h;                   // USART Handle
  uint32_t                vmode;               // Virtual mode
#endif
  ARM_USART_CAPABILITIES  capabilities;        // Capabilities
  USART_TypeDef          *reg;                 // USART peripheral pointer
  uint32_t              (*periph_clock)(void); // Peripheral bus clock
  USART_IO                io;                  // USART Input/Output pins
  IRQn_Type               irq_num;             // USART IRQ Number
  uint8_t                 reserved[3];         // Reserved
  USART_DMA               *dma_tx;             // Transmit stream register interface
  USART_DMA               *dma_rx;             // Receive stream register interface
  USART_INFO              *info;               // Run-Time Information
  USART_TRANSFER_INFO     *xfer;               // USART transfer information
} USART_RESOURCES;

// Global functions and variables exported by driver .c module
#ifdef MX_USART1
extern void USART1_IRQHandler     (void);
extern void USART1_TX_DMA_Handler (void);
extern void USART1_RX_DMA_Handler (void);

extern ARM_DRIVER_USART Driver_USART1;
#endif

#ifdef MX_USART2
extern void USART2_IRQHandler     (void);
extern void USART2_TX_DMA_Handler (void);
extern void USART2_RX_DMA_Handler (void);

extern ARM_DRIVER_USART Driver_USART2;
#endif

#ifdef MX_USART3
extern void USART3_IRQHandler     (void);
extern void USART3_TX_DMA_Handler (void);
extern void USART3_RX_DMA_Handler (void);

extern ARM_DRIVER_USART Driver_USART3;
#endif

#ifdef MX_UART4
extern void UART4_IRQHandler      (void);
extern void UART4_TX_DMA_Handler  (void);
extern void UART4_RX_DMA_Handler  (void);

extern ARM_DRIVER_USART Driver_USART4;
#endif

#ifdef MX_UART5
extern void UART5_IRQHandler      (void);
extern void UART5_TX_DMA_Handler  (void);
extern void UART5_RX_DMA_Handler  (void);

extern ARM_DRIVER_USART Driver_USART5;
#endif

#ifdef MX_USART6
extern void USART6_IRQHandler     (void);
extern void USART6_TX_DMA_Handler (void);
extern void USART6_RX_DMA_Handler (void);

extern ARM_DRIVER_USART Driver_USART6;
#endif

#endif /* __USART_STM32F4XX_H */
