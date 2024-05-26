/***************************************************************************//**
 * @file
 * @brief Lighting Server module
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

// C Standard Library headers
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "em_common.h"
#include "sl_status.h"
#include "sl_btmesh_api.h"
#include "sl_bt_api.h"
#include "sl_btmesh_dcd.h"

#include "sl_component_catalog.h"

// Bluetooth stack headers
#include "sl_btmesh_generic_model_capi_types.h"
#include "sl_btmesh_lib.h"

#include "sl_btmesh_lighting_server.h"
#include "sl_btmesh_lighting_server_config.h"


#define scene_server_reset_register(elem_index)

/// No flags used for message
#define NO_FLAGS              0
/// Immediate transition time is 0 seconds
#define IMMEDIATE             0
/// Callback has no parameters
#define NO_CALLBACK_DATA      NULL
/// High Priority
#define HIGH_PRIORITY         0
/// Values greater than max 37200000 are treated as unknown remaining time
#define UNKNOWN_REMAINING_TIME      40000000
/// Difference between Generic Level and Lightness Level to convert the ranges
#define GENERIC_TO_LIGHTNESS_LEVEL_SHIFT    32768

/**
 * @brief Default value of Lightness Last
 *
 * Stores the last known non-zero value of lightness actual.
 */
#define LIGHTNESS_LAST_DEFAULT      0xFFFF
/**
 * @brief Default value of Lightness Default
 *
 * Representing a default lightness level for lightness actual.
 */
#define LIGHTNESS_DEFAULT_DEFAULT   0x0000

/// Lightbulb state
static PACKSTRUCT(struct lightbulb_state {
  // On/Off Server state
  uint8_t onoff_current;          /**< Current generic on/off value */
  uint8_t onoff_target;           /**< Target generic on/off value */
  // Lightness server
  uint16_t lightness_current;     /**< Current lightness value */
  uint16_t lightness_target;      /**< Target lightness value */
  uint16_t lightness_last;        /**< Last lightness value */
  uint16_t lightness_default;     /**< Default lightness value */
  uint16_t lightness_min;         /**< Minimum lightness value */
  uint16_t lightness_max;         /**< Maximum lightness value */
}) lightbulb_state;

/// copy of lightness request kind, needed for delayed lightness request
static mesh_generic_state_t lightness_kind = mesh_generic_state_last;
/// copy of transition delay parameter, needed for

static sl_status_t lightness_update(uint16_t element_index,
                                    uint32_t remaining_ms,
                                    mesh_generic_state_t kind);


uint16_t sl_btmesh_get_lightness_current(void)
{
  return lightbulb_state.lightness_current;
}

uint16_t sl_btmesh_get_lightness_target(void)
{
  return lightbulb_state.lightness_target;
}

static sl_status_t onoff_update(uint16_t element_index, uint32_t remaining_ms)
{
  struct mesh_generic_state current, target;

  current.kind = mesh_generic_state_on_off;
  current.on_off.on = lightbulb_state.onoff_current;

  target.kind = mesh_generic_state_on_off;
  target.on_off.on = lightbulb_state.onoff_target;

  return mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                               element_index,
                               &current,
                               &target,
                               remaining_ms);
}

void onoff_request(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{
  (void)model_id;
  (void)client_addr;
  (void)server_addr;
  (void)appkey_index;
  (void)request_flags;

  if (lightbulb_state.onoff_current == request->on_off) {

  } else {
    if (transition_ms == 0 && delay_ms == 0) { // Immediate change
      lightbulb_state.onoff_current = request->on_off;
      lightbulb_state.onoff_target = request->on_off;
      if (lightbulb_state.onoff_current == MESH_GENERIC_ON_OFF_STATE_OFF) {
        lightbulb_state.lightness_current = 0;
        lightbulb_state.lightness_target = 0;
      } else {
        lightbulb_state.lightness_current = lightbulb_state.lightness_last;
        lightbulb_state.lightness_target = lightbulb_state.lightness_last;
      }
    } else {
      lightbulb_state.onoff_target = request->on_off;
      if (lightbulb_state.onoff_target == MESH_GENERIC_ON_OFF_STATE_ON) {
        lightbulb_state.onoff_current = MESH_GENERIC_ON_OFF_STATE_ON;
      }

      onoff_update(element_index, transition_ms);

      if (request->on_off == MESH_GENERIC_ON_OFF_STATE_OFF) {
        lightbulb_state.lightness_target = 0;
      } else {
        lightbulb_state.lightness_target = lightbulb_state.lightness_last;
      }
    }
  }

  onoff_update(element_index, 0);
}

static uint16_t actual2linear(uint16_t actual)
{
  uint32_t linear = ((uint32_t)actual * actual + 65534) / 65535;
  return (uint16_t)linear;
}

static uint16_t linear2actual(uint16_t linear)
{
  uint32_t actual = (uint32_t)sqrt(65535 * (uint32_t)linear);
  return (uint16_t)actual;
}


static sl_status_t lightness_update(uint16_t element_index,
                                    uint32_t remaining_ms,
                                    mesh_generic_state_t kind)
{
  struct mesh_generic_state current, target;

  current.kind = kind;
  if (kind == mesh_lighting_state_lightness_actual) {
    current.lightness.level = lightbulb_state.lightness_current;
  } else {
    current.lightness.level = actual2linear(lightbulb_state.lightness_current);
  }

  target.kind = kind;
  if (kind == mesh_lighting_state_lightness_actual) {
    target.lightness.level = lightbulb_state.lightness_target;
  } else {
    target.lightness.level = actual2linear(lightbulb_state.lightness_target);
  }
  return mesh_lib_generic_server_update(MESH_LIGHTING_LIGHTNESS_SERVER_MODEL_ID,
                               element_index,
                               &current,
                               &target,
                               remaining_ms);
}

void lightness_request(uint16_t model_id,
                              uint16_t element_index,
                              uint16_t client_addr,
                              uint16_t server_addr,
                              uint16_t appkey_index,
                              const struct mesh_generic_request *request,
                              uint32_t transition_ms,
                              uint16_t delay_ms,
                              uint8_t request_flags)
{
  (void)model_id;
  (void)client_addr;
  (void)server_addr;
  (void)appkey_index;
  (void)request_flags;

  uint16_t actual_request = 0;

  switch (request->kind) {
    case mesh_lighting_request_lightness_actual:
      lightness_kind = mesh_lighting_state_lightness_actual;
      actual_request = request->lightness;
      break;

    case mesh_lighting_request_lightness_linear:
      lightness_kind = mesh_lighting_state_lightness_linear;
      actual_request = linear2actual(request->lightness);
      break;

    default:
      break;
  }

  if (lightbulb_state.lightness_current == actual_request) {

  } else {
    if (transition_ms == 0 && delay_ms == 0) { // Immediate change
      lightbulb_state.lightness_current = actual_request;
      lightbulb_state.lightness_target = actual_request;
      if (actual_request != 0) {
        lightbulb_state.lightness_last = actual_request;
      }
    } else {
      lightbulb_state.lightness_target = actual_request;
    }
  }
  if (lightbulb_state.lightness_current > 0) {
      lightbulb_state.onoff_current = MESH_GENERIC_ON_OFF_STATE_ON;
      lightbulb_state.onoff_target = MESH_GENERIC_ON_OFF_STATE_ON;
  } else {
      lightbulb_state.onoff_current = MESH_GENERIC_ON_OFF_STATE_OFF;
      lightbulb_state.onoff_target = MESH_GENERIC_ON_OFF_STATE_OFF;
  }
  lightness_update(element_index, 0, lightness_kind);
}

