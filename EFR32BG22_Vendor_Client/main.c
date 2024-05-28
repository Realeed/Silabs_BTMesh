/***************************************************************************//**
 * @file
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "sl_event_handler.h"
#include "pa_conversions_efr32.h"
#include "sl_bt_stack_init.h"
#include "sl_btmesh_stack_init.h"
#include "sl_bluetooth.h"
#include "sl_btmesh.h"

#include "em_cmu.h"
#include "em_gpio.h"

void PendSV_Handler()
{
  sl_bt_priority_handle();
}

extern sl_status_t sli_bt_system_start_bluetooth();

static const struct sli_bgapi_class * const btmesh_class_table[] =
{
  SL_BTMESH_BGAPI_CLASS(health_server),
  SL_BTMESH_BGAPI_CLASS(proxy),
  SL_BTMESH_BGAPI_CLASS(proxy_server),
  SL_BTMESH_BGAPI_CLASS(vendor_model),
  SL_BTMESH_BGAPI_CLASS(node),
  NULL
};
void mesh_advertiser_legacy();

bool provisioned = false;
bool interrupt = false;
uint8_t tx[2] = {0};

void GPIO_ODD_IRQHandler(void)
{
  GPIO_IntClear(GPIO_IntGet());
  interrupt = true;
}

int main(void)
{
  sl_platform_init();
  sl_rail_util_pa_init();
  NVIC_ClearPendingIRQ(PendSV_IRQn);
  NVIC_EnableIRQ(PendSV_IRQn);
  sl_bt_stack_init();
  sli_bt_system_start_bluetooth();
  sl_btmesh_init_classes(btmesh_class_table);
  mesh_advertiser_legacy();

  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(gpioPortB, 1, gpioModeInput, 1);
  GPIO_ExtIntConfig(gpioPortB, 1, 1, false, true, true);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  while (1) {
    sl_bt_step();
    sl_btmesh_step();
    if (provisioned && interrupt) {
        tx[0] = 0xFE;
        tx[1] = 0xD8;
        sl_btmesh_vendor_model_set_publication(0, 0x02ff, 0x6666, 0, 1, 2, tx);
        sl_btmesh_vendor_model_publish(0, 0x02ff, 0x6666);
        interrupt = false;
    }
  }
}

void sl_btmesh_on_event(sl_btmesh_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_btmesh_evt_node_provisioned_id:
      provisioned = true;
      break;
    default:
      break;
  }
}
