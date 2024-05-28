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
#include "sl_bluetooth.h"
#include "sl_btmesh.h"
#include "sl_bt_stack_init.h"
#include "sl_btmesh_generic_base.h"
#include "sl_btmesh_generic_base_config.h"
#include "sl_btmesh_generic_model_capi_types.h"
#include "sl_btmesh_lib.h"

#include "sl_btmesh_api.h"
#include "sl_bt_api.h"
#include "sl_btmesh_api.h"
#include <stdio.h>
#include "sl_btmesh_lighting_server.h"
#include "sl_btmesh_model_specification_defs.h"
#include "sl_udelay.h"

extern sl_status_t sli_bt_system_start_bluetooth();
void mesh_advertiser_legacy();

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const btmesh_class_table[] =
{
  SL_BTMESH_BGAPI_CLASS(generic_server),
  SL_BTMESH_BGAPI_CLASS(health_server),
  SL_BTMESH_BGAPI_CLASS(proxy),
  SL_BTMESH_BGAPI_CLASS(proxy_server),
  SL_BTMESH_BGAPI_CLASS(node),
  NULL
};

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

  while (1) {
      sl_bt_step();
      sl_btmesh_step();
  }
}

void sl_btmesh_on_event(sl_btmesh_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_btmesh_evt_generic_server_client_request_id:
      printf("Generic Server Client Request\n");
      switch (evt->data.evt_generic_server_client_request.model_id) {
        case MESH_LIGHTING_LIGHTNESS_SERVER_MODEL_ID:
          struct mesh_generic_request lightness_req;
          uint8_t lightArr[2] = {0};
          lightArr[0] = evt->data.evt_generic_server_client_request.parameters.data[0];
          lightArr[1] = evt->data.evt_generic_server_client_request.parameters.data[1];
          uint16_t lightnessValue = (lightArr[1] << 8) | lightArr[0];

          lightness_req.kind = evt->data.evt_generic_server_client_request.type;
          lightness_req.lightness = lightnessValue;
          lightness_request(evt->data.evt_generic_server_client_request.model_id,
                            evt->data.evt_generic_server_client_request.elem_index,
                            evt->data.evt_generic_server_client_request.client_address,
                            evt->data.evt_generic_server_client_request.server_address,
                            evt->data.evt_generic_server_client_request.appkey_index,
                            &lightness_req,
                            evt->data.evt_generic_server_client_request.transition_ms,
                            evt->data.evt_generic_server_client_request.delay_ms,
                            evt->data.evt_generic_server_client_request.flags);
          printf("Model: Light Lightness\n");
          printf("Lightness: %u\n", sl_btmesh_get_lightness_current());
          break;
        case MESH_GENERIC_ON_OFF_SERVER_MODEL_ID:
          struct mesh_generic_request onoff_req;
          onoff_req.on_off = evt->data.evt_generic_server_client_request.parameters.data[0];
          onoff_request(evt->data.evt_generic_server_client_request.model_id,
                            evt->data.evt_generic_server_client_request.elem_index,
                            evt->data.evt_generic_server_client_request.client_address,
                            evt->data.evt_generic_server_client_request.server_address,
                            evt->data.evt_generic_server_client_request.appkey_index,
                            &onoff_req,
                            evt->data.evt_generic_server_client_request.transition_ms,
                            evt->data.evt_generic_server_client_request.delay_ms,
                            evt->data.evt_generic_server_client_request.flags);
          printf("Lightness: %u\n", sl_btmesh_get_lightness_current());
          printf("Model: Generic ON/OFF\n");
          break;
      }
      break;
    case sl_btmesh_evt_node_provisioned_id:
      printf("Node Provisioned\n");
      break;
    default:
      break;
  }
}
