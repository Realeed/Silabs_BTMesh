/***************************************************************************//**
 * @file
 * @brief Bt Mesh Lighting Client module
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

/* C Standard Library headers */
#include <stdlib.h>
#include <stdio.h>

#include "em_common.h"
#include "sl_status.h"

#include "sl_btmesh_api.h"
#include "sl_bt_api.h"
#include "sl_btmesh_dcd.h"

#include "sl_btmesh_generic_model_capi_types.h"
#include "sl_btmesh_lib.h"

#include "app_assert.h"
#include "app_timer.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#include "sl_btmesh_lighting_client_config.h"
#include "sl_btmesh_lighting_client.h"


/// Parameter ignored for publishing
#define IGNORED                        0
/// No flags used for message
#define NO_FLAGS                       0
/// Immediate transition time is 0 seconds
#define IMMEDIATE                      0
/// High Priority
#define HIGH_PRIORITY                  0
/// Callback has no parameters
#define NO_CALLBACK_DATA               NULL
/// Maximum lightness percentage value
#define LIGHTNESS_PCT_MAX              100
/// Maximum lightness value
#define LIGHTNESS_VALUE_MAX            0xFFFF
/// Delay unit value for request for ctl messages in millisecond
#define REQ_DELAY_MS                   50

#define OPTIMAL_RETRANSMISSION_CNT     5
/// periodic timer handle

// current position of the switch
static uint8_t switch_pos = 0;
/// on/off transaction identifier
static uint8_t onoff_trid = 0;
/// lightness level when switching ON
static uint16_t lightness_level_switch_on = UINT16_MAX;
/// lightness level converted from percentage to actual value, range 0..65535
static uint16_t lightness_level = 0;
/// lightness transaction identifier
static uint8_t lightness_trid = 0;

uint8_t lightnessRequestCnt = 0;
uint8_t onOffRequestCnt = 0;

uint8_t elemIndex = 0;

//uint8_t lightbuf[2] = {0};

static void send_onoff_request(uint8_t retrans)
{
  struct mesh_generic_request req;
  req.kind = mesh_generic_request_on_off;
  req.on_off = switch_pos ? MESH_GENERIC_ON_OFF_STATE_ON : MESH_GENERIC_ON_OFF_STATE_OFF;
  if (retrans == 0) {
    onoff_trid++;
  }
  mesh_lib_generic_client_publish(MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID,
                                       BTMESH_LIGHTING_CLIENT_MAIN,
                                       onoff_trid,
                                       &req,
                                       IMMEDIATE, // transition time in ms
                                       0,
                                       NO_FLAGS   // flags
                                       );
  if (onOffRequestCnt > 0) {
      onOffRequestCnt--;
  }
}

static void send_lightness_request(uint8_t retrans)
{
//  lightbuf[0] = lightness_level & 0xFF;
//  lightbuf[1] = (lightness_level >> 8) & 0xFF;
//  sl_btmesh_generic_client_publish(elemIndex,
//                                   MESH_LIGHTING_LIGHTNESS_CLIENT_MODEL_ID,
//                                            0,
//                                            0,
//                                            0,
//                                            0,
//                                            mesh_lighting_request_lightness_actual,
//                                            2,
//                                            lightbuf);
//  if (lightnessRequestCnt > 0) {
//      lightnessRequestCnt--;
//  }
}

void setGenericOnOff()
{
  send_onoff_request(1);
}

bool getOnOffRequestCnt()
{
  return onOffRequestCnt;
}

void setLightness()
{
  send_lightness_request(1);
}

bool getLightnessRequestCnt()
{
  return lightnessRequestCnt;
}

/*******************************************************************************
 * This function change the lightness and send it to the server.
 *
 * @param[in] lightness_percent  Defines new lightness value as percentage
 *    Valid values 0-100 %
 *
 ******************************************************************************/
void sl_btmesh_set_lightness(uint8_t lightness_percent, uint8_t elementInd)
{
//  elemIndex = elementInd;
//  lightness_level = lightness_percent * LIGHTNESS_VALUE_MAX / LIGHTNESS_PCT_MAX;
//  lightnessRequestCnt = 2;
//  send_lightness_request(0);
}

void sl_btmesh_change_switch_position(uint8_t position)
{
  if (position != SL_BTMESH_LIGHTING_CLIENT_TOGGLE) {
    switch_pos = position;
  } else {
    switch_pos = 1 - switch_pos; // Toggle switch state
  }
  if (switch_pos) {
    lightness_level = lightness_level_switch_on;
  } else {
    lightness_level = 0;
  }
  onOffRequestCnt = OPTIMAL_RETRANSMISSION_CNT;
  send_onoff_request(0);
}
