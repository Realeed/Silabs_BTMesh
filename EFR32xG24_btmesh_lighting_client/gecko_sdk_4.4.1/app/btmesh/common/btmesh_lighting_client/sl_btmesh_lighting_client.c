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

//#include "app_assert.h"
#include "app_timer.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#ifdef SL_CATALOG_APP_LOG_PRESENT
#include "app_log.h"
#endif // SL_CATALOG_APP_LOG_PRESENT

#include "sl_btmesh_lighting_client_config.h"
#include "sl_btmesh_lighting_client.h"

//#include "app_timer.h"
//#include "sl_sleeptimer.h"

// Warning! The app_btmesh_util shall be included after the component configuration
// header file in order to provide the component specific logging macro.
#include "app_btmesh_util.h"

/***************************************************************************//**
 * @addtogroup Lighting
 * @{
 ******************************************************************************/

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
static app_timer_t onoff_retransmission_timer;

/// periodic timer callback
static void onoff_retransmission_timer_cb(app_timer_t *handle,
                                          void *data);
/// periodic timer handle
static app_timer_t light_retransmission_timer;


/// periodic timer callback
static void light_retransmission_timer_cb(app_timer_t *handle,
                                          void *data);

/// current position of the switch
static uint8_t switch_pos = 0;
/// number of on/off requests to be sent
static uint8_t onoff_request_count;
/// on/off transaction identifier
static uint8_t onoff_trid = 0;
/// lightness level when switching ON
static uint16_t lightness_level_switch_on = UINT16_MAX;
/// lightness level converted from percentage to actual value, range 0..65535
static uint16_t lightness_level = 0;
/// number of lightness requests to be sent
static uint8_t lightness_request_count;
/// lightness transaction identifier
static uint8_t lightness_trid = 0;

uint8_t lightnessNumberOfRequests = 0;
uint8_t onOffNumberOfRequests = 0;

/***************************************************************************//**
 * This function publishes one generic on/off request to change the state
 * of light(s) in the group. Global variable switch_pos holds the latest
 * desired light state, possible values are:
 * switch_pos = 1 -> PB1 was pressed long (above 1s), turn lights on
 * switch_pos = 0 -> PB0 was pressed long (above 1s), turn lights off
 *
 * @param[in] retrans Indicates if this is the first request or a retransmission,
 *                    possible values are 0 = first request, 1 = retransmission.
 *
 * @note This application sends multiple generic on/off requests for each
 *       long button press to improve reliability.
 *       The transaction ID is not incremented in case of a retransmission.
 ******************************************************************************/
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
                                       REQ_DELAY_MS,
                                       NO_FLAGS   // flags
                                       );
  onOffNumberOfRequests--;
}

/***************************************************************************//**
 * This function publishes one light lightness request to change the lightness
 * level of light(s) in the group. Global variable lightness_level holds
 * the latest desired light level.
 *
 * @param[in] retrans Indicates if this is the first request or a retransmission,
 *                    possible values are 0 = first request, 1 = retransmission.
 *
 * @note This application sends multiple lightness requests for each
 *       short button press to improve reliability.
 *       The transaction ID is not incremented in case of a retransmission.
 ******************************************************************************/


static void send_lightness_request(uint8_t retrans)
{
  struct mesh_generic_request req;
  req.kind = mesh_lighting_request_lightness_actual;
  req.lightness = lightness_level;
  if (retrans == 0) {
    lightness_trid++;
  }
  mesh_lib_generic_client_publish(MESH_LIGHTING_LIGHTNESS_CLIENT_MODEL_ID,
                                       BTMESH_LIGHTING_CLIENT_MAIN,
                                       lightness_trid,
                                       &req,
                                       IMMEDIATE,     // transition
                                       REQ_DELAY_MS,
                                       NO_FLAGS       // flags
                                       );
  lightnessNumberOfRequests--;
}

void setLightness()
{
  send_lightness_request(1);
}

bool getLightnessNumberOfRequests()
{
  return lightnessNumberOfRequests;
}

void setOnOff()
{
  send_onoff_request(1);
}

bool getOnOffNumberOfRequests()
{
  return onOffNumberOfRequests;
}

/*******************************************************************************
 * This function change the lightness and send it to the server.
 *
 * @param[in] lightness_percent  Defines new lightness value as percentage
 *    Valid values 0-100 %
 *
 ******************************************************************************/
void sl_btmesh_set_lightness(uint8_t lightness_percent)
{
  if (lightness_percent > LIGHTNESS_PCT_MAX) {
    return;
  }
  lightness_level = lightness_percent * LIGHTNESS_VALUE_MAX / LIGHTNESS_PCT_MAX;
  if (lightness_level != 0) {
    lightness_level_switch_on = lightness_level;
  }
  lightnessNumberOfRequests = OPTIMAL_RETRANSMISSION_CNT;
  send_lightness_request(0);  // Send the first request
}

/*******************************************************************************
 * This function change the switch position and send it to the server.
 *
 * @param[in] position Defines switch position change, possible values are:
 *                       - SL_BTMESH_LIGHTING_CLIENT_OFF
 *                       - SL_BTMESH_LIGHTING_CLIENT_ON
 *                       - SL_BTMESH_LIGHTING_CLIENT_TOGGLE
 *
 ******************************************************************************/
void sl_btmesh_change_switch_position(uint8_t position)
{
  if (position != SL_BTMESH_LIGHTING_CLIENT_TOGGLE) {
    switch_pos = position;
  } else {
    switch_pos = 1 - switch_pos;
  }
  if (switch_pos) {
    log("Turn light(s) on" NL);
    lightness_level = lightness_level_switch_on;
  } else {
    log("Turn light(s) off" NL);
    lightness_level = 0;
  }
  onOffNumberOfRequests = OPTIMAL_RETRANSMISSION_CNT;
  send_onoff_request(0);  // Send the first request
}

uint16_t sl_btmesh_get_lightness(void)
{
  return lightness_level;
}
/** @} (end addtogroup btmesh_light_clnt_tim_cb) */
/** @} (end addtogroup Lighting) */
