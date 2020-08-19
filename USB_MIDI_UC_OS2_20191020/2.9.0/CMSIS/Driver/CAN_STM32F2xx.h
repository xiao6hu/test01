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
 * $Date:        12. March 2018
 * $Revision:    V1.3
 *
 * Driver:       Driver_CAN1/2
 * Configured:   via RTE_Device.h configuration file
 * Project:      CAN Driver Header for ST STM32F2xx
 * -------------------------------------------------------------------------- */

#ifndef __CAN_STM32F2XX_H
#define __CAN_STM32F2XX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cmsis_compiler.h"

#ifdef      __CC_ARM
  #ifndef   __PACKED__
    #define __PACKED__                           __packed
  #endif
#else
  #ifndef   __PACKED__
    #define __PACKED__
  #endif
#endif

#if  (defined ( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 ))
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpacked"
#elif defined ( __GNUC__ )
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
#endif
#ifndef   __UNALIGNED_UINT32_WRITE
  __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void*)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT32_READ
  __PACKED_STRUCT T_UINT32_READ  { uint32_t v; };
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ  *)(const void*)(addr))->v)
#endif
#if   defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined ( __GNUC__ )
  #pragma GCC diagnostic pop
#endif

#include "Driver_CAN.h"
#include "stm32f2xx_hal.h"

#include "RTE_Components.h"
#if    (defined(RTE_DEVICE_FRAMEWORK_CLASSIC))
#include "RTE_Device.h"
#elif  (defined(RTE_DEVICE_FRAMEWORK_CUBE_MX))
#include "MX_Device.h"
#else
#error "::Device:STM32Cube Framework: not selected in RTE"
#endif

#if    (defined(RTE_DEVICE_FRAMEWORK_CLASSIC))

#if   ((defined(RTE_Drivers_CAN1) ||  defined(RTE_Drivers_CAN2)) && \
      ((RTE_CAN1 == 0)            && (RTE_CAN2 == 0)))
#error "No CAN configured in RTE_Device.h!"
#endif

#if    (RTE_CAN1 != 0)
#define MX_CAN1                         1

/* Pin CAN1_RX */
#define MX_CAN1_RX_Pin                  1
#define MX_CAN1_RX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN1_RX_GPIOx                RTE_CAN1_RX_PORT
#define MX_CAN1_RX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN1_RX_GPIO_Pin             RTE_CAN1_RX_BIT
#define MX_CAN1_RX_GPIO_AF              GPIO_AF9_CAN1
#define MX_CAN1_RX_GPIO_Mode            GPIO_MODE_AF_PP

/* Pin CAN1_TX */
#define MX_CAN1_TX_Pin                  1
#define MX_CAN1_TX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN1_TX_GPIOx                RTE_CAN1_TX_PORT
#define MX_CAN1_TX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN1_TX_GPIO_Pin             RTE_CAN1_TX_BIT
#define MX_CAN1_TX_GPIO_AF              GPIO_AF9_CAN1
#define MX_CAN1_TX_GPIO_Mode            GPIO_MODE_AF_PP
#endif

#if    (RTE_CAN2 != 0)
#define MX_CAN2                         1

/* Pin CAN2_RX */
#define MX_CAN2_RX_Pin                  1
#define MX_CAN2_RX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN2_RX_GPIOx                RTE_CAN2_RX_PORT
#define MX_CAN2_RX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN2_RX_GPIO_Pin             RTE_CAN2_RX_BIT
#define MX_CAN2_RX_GPIO_AF              GPIO_AF9_CAN2
#define MX_CAN2_RX_GPIO_Mode            GPIO_MODE_AF_PP

/* Pin CAN2_TX */
#define MX_CAN2_TX_Pin                  1
#define MX_CAN2_TX_GPIO_Speed           GPIO_SPEED_HIGH
#define MX_CAN2_TX_GPIOx                RTE_CAN2_TX_PORT
#define MX_CAN2_TX_GPIO_PuPd            GPIO_NOPULL
#define MX_CAN2_TX_GPIO_Pin             RTE_CAN2_TX_BIT
#define MX_CAN2_TX_GPIO_AF              GPIO_AF9_CAN2
#define MX_CAN2_TX_GPIO_Mode            GPIO_MODE_AF_PP
#endif

#elif  (defined(RTE_DEVICE_FRAMEWORK_CUBE_MX))

#if   ((defined(RTE_Drivers_CAN1) ||   defined(RTE_Drivers_CAN2)) && \
      (!defined(MX_CAN1))         && (!defined(MX_CAN2)))
#error "No CAN configured in STM32CubeMX!"
#endif

#endif

#if    !defined(MX_CAN1)
#define MX_CAN1                         (0U)
#endif
#if    !defined(MX_CAN2)
#define MX_CAN2                         (0U)
#endif

#if    (MX_CAN2 == 1U)
#define CAN_CTRL_NUM                    (2U)
#else
#define CAN_CTRL_NUM                    (1U)
#endif

// Global functions and variables exported by driver .c module
#if (MX_CAN1 == 1U)
extern void CAN1_TX_IRQHandler  (void);
extern void CAN1_RX0_IRQHandler (void);
extern void CAN1_RX1_IRQHandler (void);
extern void CAN1_SCE_IRQHandler (void);

extern ARM_DRIVER_CAN Driver_CAN1;
#endif

#if (MX_CAN2 == 1U)
extern void CAN2_TX_IRQHandler  (void);
extern void CAN2_RX0_IRQHandler (void);
extern void CAN2_RX1_IRQHandler (void);
extern void CAN2_SCE_IRQHandler (void);

extern ARM_DRIVER_CAN Driver_CAN2;
#endif

#endif // __CAN_STM32F4XX_H
