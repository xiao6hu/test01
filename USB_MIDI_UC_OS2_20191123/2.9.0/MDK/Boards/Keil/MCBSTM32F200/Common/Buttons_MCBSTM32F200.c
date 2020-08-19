/*-----------------------------------------------------------------------------
 * Name:    Buttons_MCBSTM32F200.c
 * Purpose: Buttons interface for MCBSTM32F200 evaluation board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2013 - 2018 Arm Limited

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "stm32f2xx_hal.h"
#include "Board_Buttons.h"

/* Buttons for MCBSTM32F200/400 */
#define BUTTONS_COUNT                  (3)
#define BUTTONS_USER_MASK              (1<<0)
#define BUTTONS_USER_PORT               GPIOG
#define BUTTONS_USER_PIN                GPIO_PIN_15
#define BUTTONS_TAMPER_MASK            (1<<1)
#define BUTTONS_TAMPER_PORT             GPIOC
#define BUTTONS_TAMPER_PIN              GPIO_PIN_13
#define BUTTONS_WAKEUP_MASK            (1<<2)
#define BUTTONS_WAKEUP_PORT             GPIOA
#define BUTTONS_WAKEUP_PIN              GPIO_PIN_0


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();

  /* Configure GPIO pin: PA0 (WAKEUP) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure GPIO pin: PC13 (TAMPER) */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure GPIO pin: PG15 (USER) */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  return 0;
}

/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) {

  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_13);
  HAL_GPIO_DeInit(GPIOI, GPIO_PIN_15);

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {
  uint32_t val = 0;

  if (HAL_GPIO_ReadPin(BUTTONS_USER_PORT,   BUTTONS_USER_PIN)   == GPIO_PIN_RESET) {
    val |= BUTTONS_USER_MASK;
  }
  if (HAL_GPIO_ReadPin(BUTTONS_TAMPER_PORT, BUTTONS_TAMPER_PIN) == GPIO_PIN_RESET) {
    val |= BUTTONS_TAMPER_MASK;
  }
  if (HAL_GPIO_ReadPin(BUTTONS_WAKEUP_PORT, BUTTONS_WAKEUP_PIN) == GPIO_PIN_SET) {
    val |= BUTTONS_WAKEUP_MASK;
  }

  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return BUTTONS_COUNT;
}
