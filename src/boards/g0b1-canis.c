/*

The MIT License (MIT)

Copyright (c) 2023 Pengutronix,
              Marc Kleine-Budde <kernel@pengutronix.de>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "board.h"
#include "config.h"
#include "device.h"
#include "gpio.h"
#include "led.h"
#include "usbd_gs_can.h"

#define LEDRX1_GPIO_Port	  GPIOB
#define LEDRX1_Pin		  GPIO_PIN_12
#define LEDRX1_Mode		  GPIO_MODE_OUTPUT_PP
#define LEDRX1_Active_High 1

#define LEDTX1_GPIO_Port	  GPIOB
#define LEDTX1_Pin		  GPIO_PIN_11
#define LEDTX1_Mode		  GPIO_MODE_OUTPUT_PP
#define LEDTX1_Active_High 1

#define LEDRX2_GPIO_Port	  GPIOB
#define LEDRX2_Pin		  GPIO_PIN_10
#define LEDRX2_Mode		  GPIO_MODE_OUTPUT_PP
#define LEDRX2_Active_High 1

#define LEDTX2_GPIO_Port	  GPIOB
#define LEDTX2_Pin		  GPIO_PIN_2
#define LEDTX2_Mode		  GPIO_MODE_OUTPUT_PP
#define LEDTX2_Active_High 1

#define FDCAN1_GPIO_Port	  GPIOB
#define FDCAN1_RX_Pin		  GPIO_PIN_8
#define FDCAN1_TX_Pin		  GPIO_PIN_9

#define FDCAN1_TERM_EN_GPIO_Port	  GPIOB
#define FDCAN1_TERM_EN_Pin		  GPIO_PIN_3

#define FDCAN2_GPIO_Port	  GPIOB
#define FDCAN2_RX_Pin		  GPIO_PIN_5
#define FDCAN2_TX_Pin		  GPIO_PIN_6

#define FDCAN2_TERM_EN_GPIO_Port	  GPIOB
#define FDCAN2_TERM_EN_Pin		  GPIO_PIN_4

static void canis_setup(USBD_GS_CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	UNUSED(hcan);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* LEDs */

	HAL_GPIO_WritePin(LEDRX1_GPIO_Port, LEDRX1_Pin, GPIO_INIT_STATE(LEDRX1_Active_High));
	GPIO_InitStruct.Pin = LEDRX1_Pin;
	GPIO_InitStruct.Mode = LEDRX1_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDRX1_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LEDTX1_GPIO_Port, LEDTX1_Pin, GPIO_INIT_STATE(LEDTX1_Active_High));
	GPIO_InitStruct.Pin = LEDTX1_Pin;
	GPIO_InitStruct.Mode = LEDTX1_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDTX1_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LEDRX2_GPIO_Port, LEDRX2_Pin, GPIO_INIT_STATE(LEDRX2_Active_High));
	GPIO_InitStruct.Pin = LEDRX2_Pin;
	GPIO_InitStruct.Mode = LEDRX2_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDRX2_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LEDTX2_GPIO_Port, LEDTX2_Pin, GPIO_INIT_STATE(LEDTX2_Active_High));
	GPIO_InitStruct.Pin = LEDTX2_Pin;
	GPIO_InitStruct.Mode = LEDTX2_Mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LEDTX2_GPIO_Port, &GPIO_InitStruct);

	/* FDCAN */

	RCC_PeriphCLKInitTypeDef PeriphClkInit = {
		.PeriphClockSelection = RCC_PERIPHCLK_FDCAN,
		.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL,
	};

	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	__HAL_RCC_FDCAN_CLK_ENABLE();

	/* FDCAN1_RX, FDCAN1_TX */
	GPIO_InitStruct.Pin = FDCAN1_RX_Pin | FDCAN1_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_FDCAN1;
	HAL_GPIO_Init(FDCAN1_GPIO_Port, &GPIO_InitStruct);

	/* FDCAN1_TERM_EN */
	GPIO_InitStruct.Pin = FDCAN1_TERM_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(FDCAN1_TERM_EN_GPIO_Port, &GPIO_InitStruct);

	/* FDCAN1_RX, FDCAN1_TX */
	GPIO_InitStruct.Pin = FDCAN2_RX_Pin | FDCAN2_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF3_FDCAN2;
	HAL_GPIO_Init(FDCAN2_GPIO_Port, &GPIO_InitStruct);

	/* FDCAN2_TERM_EN */
	GPIO_InitStruct.Pin = FDCAN2_TERM_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(FDCAN2_TERM_EN_GPIO_Port, &GPIO_InitStruct);
}

static void
canis_phy_power_set(can_data_t *channel, bool enable)
{
	UNUSED(channel);
	UNUSED(enable);
}

static void
canis_termination_set(can_data_t *channel,
							  enum gs_can_termination_state enable)
{
	const uint8_t nr = channel->nr;

	if (nr == 0) {
		HAL_GPIO_WritePin(FDCAN1_TERM_EN_GPIO_Port, FDCAN1_TERM_EN_Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(FDCAN2_TERM_EN_GPIO_Port, FDCAN2_TERM_EN_Pin, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

const struct BoardConfig config = {
	.setup = canis_setup,
	.phy_power_set = canis_phy_power_set,
	.termination_set = canis_termination_set,
	.channels[0] = {
		.interface = FDCAN1,
		.leds = {
			[LED_RX] = {
				.port = LEDRX1_GPIO_Port,
				.pin = LEDRX1_Pin,
				.active_high = LEDRX1_Active_High,
			},
			[LED_TX] = {
				.port = LEDTX1_GPIO_Port,
				.pin = LEDTX1_Pin,
				.active_high = LEDTX1_Active_High,
			},
		},
	},
	.channels[1] = {
		.interface = FDCAN2,
		.leds = {
			[LED_RX] = {
				.port = LEDRX2_GPIO_Port,
				.pin = LEDRX2_Pin,
				.active_high = LEDRX2_Active_High,
			},
			[LED_TX] = {
				.port = LEDTX2_GPIO_Port,
				.pin = LEDTX2_Pin,
				.active_high = LEDTX2_Active_High,
			},
		},
	},
};
