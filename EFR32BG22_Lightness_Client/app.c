/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "em_common.h"
#include "app_assert.h"
#include "sl_status.h"
#include "app.h"

#include "sl_btmesh_api.h"
#include "sl_bt_api.h"

#include <stdio.h>
#include "sl_btmesh_api.h"
#include "sl_btmesh_model_specification_defs.h"
#include "sl_btmesh_lighting_client.h"
#include "sl_udelay.h"

#include "em_cmu.h"
#include "em_gpio.h"


bool press = false;
uint8_t elementIndex = 0;
uint8_t lightness = 0;

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
  if (lightness < 100) {
      lightness++;
  }
  else {
      lightness = 0;
  }
  sl_btmesh_set_lightness(lightness, elementIndex);
  printf("______________________\n");
  printf("Element index: %u\n", elementIndex);
  printf("Lightness: %d\n", lightness * 65535 / 100);
  printf("______________________\n\n");
}

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(gpioPortB, 1, gpioModeInput, 1);
  GPIO_ExtIntConfig(gpioPortB, 1, 1, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}
/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(struct sl_bt_msg *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

/**************************************************************************//**
 * Bluetooth Mesh stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth Mesh stack.
 *****************************************************************************/
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
      // -------------------------------
      // Default event handler.
      default:
        break;
    }
}
