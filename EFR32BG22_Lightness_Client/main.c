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


#include <stdio.h>
#include "sl_system_process_action.h"

#include "pa_conversions_efr32.h"
#include "sl_bluetooth.h"
#include "sl_btmesh.h"
#include "sl_btmesh_stack_init.h"
#include "sl_bt_stack_init.h"
#include "sl_event_handler.h"
#include "sl_btmesh_api.h"
#include "sl_btmesh_model_specification_defs.h"
#include "sl_btmesh_generic_model_capi_types.h"

#include "sl_udelay.h"

#include "em_cmu.h"
#include "em_gpio.h"

bool press = false;
uint8_t elementIndex = 0;
uint16_t lightness = 0;
uint8_t lightbuf[2] = {0};
uint8_t tid = 0;
uint8_t counter = 0;

extern sl_status_t sli_bt_system_start_bluetooth();

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const btmesh_class_table[] =
{
  SL_BTMESH_BGAPI_CLASS(generic_client),
  SL_BTMESH_BGAPI_CLASS(health_server),
  SL_BTMESH_BGAPI_CLASS(proxy),
  SL_BTMESH_BGAPI_CLASS(proxy_server),
  SL_BTMESH_BGAPI_CLASS(node),
  NULL
};
void mesh_advertiser_legacy();

void setLight()
{
  sl_btmesh_generic_client_publish(elementIndex,
                                       MESH_LIGHTING_LIGHTNESS_CLIENT_MODEL_ID,
                                                tid,
                                                0,
                                                0,
                                                0,
                                                mesh_lighting_request_lightness_actual,
                                                2,
                                                lightbuf);
  tid++;
  if (counter > 0) {
      counter--;
  }
}

void GPIO_ODD_IRQHandler(void)
{
  GPIO_IntClear(GPIO_IntGet());
  if (press) {
      elementIndex = 1;
      press = false;
  }
  else {
      elementIndex = 0;
      press = true;
  }
  if (lightness < 65535) {
      lightness+=4369;
  }
  else {
      lightness = 0;
  }

  lightbuf[0] = lightness & 0xFF;
  lightbuf[1] = (lightness >> 8) & 0xFF;
  counter = 2;
  setLight();
  printf("______________________\n");
  printf("Element index: %u\n", elementIndex);
  printf("Lightness: %d\n", lightness);
  printf("______________________\n\n");
}

int main(void)
{
  sl_platform_init();
  sl_driver_init();
  sl_service_init();
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
      if (counter) {
          setLight();
      }
      sl_bt_step();
      sl_btmesh_step();
  }
}

void sl_btmesh_on_event(sl_btmesh_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
      ///////////////////////////////////////////////////////////////////////////
      // Add additional event handlers here as your application requires!      //
      ///////////////////////////////////////////////////////////////////////////
      case sl_btmesh_evt_node_initialized_id:
        printf("Node Initalized\n");
        break;
      case sl_btmesh_evt_node_provisioning_started_id:
        printf("Node Provisioning Started\n");
        break;
      case sl_btmesh_evt_health_server_attention_id:
        printf("Health Server Attention\n");
        break;
      case sl_btmesh_evt_node_start_received_id:
        printf("Node Start Received\n");
        break;
      case sl_btmesh_evt_node_key_added_id:
        printf("Node Key Added\n");
        break;
      case sl_btmesh_evt_node_provisioned_id:
        printf("Node Provisioned Successfully\n");
        break;
      case sl_btmesh_evt_proxy_connected_id:
        printf("Proxy Connected\n");
        break;
      case sl_btmesh_evt_node_config_set_id:
        printf("Config Set\n");
        break;
      case sl_btmesh_evt_node_model_config_changed_id:
        switch(evt->data.evt_node_model_config_changed.model_id) {
          case MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID:
            printf("Node Model Config Changed to Generic ON/OFF Client\n");
            break;
          case MESH_LIGHTING_LIGHTNESS_CLIENT_MODEL_ID:
            printf("Node Model Config Changed to Light Lightness Client\n");
            break;
        }
        break;
      case sl_btmesh_evt_generic_client_server_status_id:
        printf("Server Status Update\n");
        break;
      case sl_btmesh_evt_proxy_disconnected_id:
        printf("Proxy Disconnected\n");
        break;
      case sl_btmesh_evt_node_reset_id:
        printf("Node Reset\n");
        sl_udelay_wait(100000);
        sl_bt_system_reset(0);
        break;
      default:
        break;
    }
}
