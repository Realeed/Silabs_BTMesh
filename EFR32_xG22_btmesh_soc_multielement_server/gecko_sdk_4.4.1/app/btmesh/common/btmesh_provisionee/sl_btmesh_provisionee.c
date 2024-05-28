/***************************************************************************//**
 * @file
 * @brief
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

#include "sl_status.h"
#include "sl_common.h"

#include "sl_bt_api.h"
#include "sl_btmesh_api.h"

#include <string.h>

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#ifdef SL_CATALOG_APP_LOG_PRESENT
#include "app_log.h"
#endif // SL_CATALOG_APP_LOG_PRESENT

#ifdef SL_CATALOG_BTMESH_FACTORY_RESET_PRESENT
#include "sl_btmesh_factory_reset.h"
#endif // SL_CATALOG_BTMESH_FACTORY_RESET_PRESENT

#include <stdio.h>
#include "sl_udelay.h"
#include "sl_btmesh_provisionee.h"
#include "sl_btmesh_provisionee_config.h"

#include "app_btmesh_util.h"

#include "sl_btmesh_lighting_server_config.h"

void sl_bt_provisionee_on_event(sl_bt_msg_t* evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      sl_btmesh_node_init();
      sl_btmesh_generic_server_init_lightness();
      sl_btmesh_generic_server_init_common();
      break;

    default:
      break;
  }
}

void sl_btmesh_provisionee_on_event(sl_btmesh_msg_t* evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_btmesh_evt_node_initialized_id:
      if (!(evt->data.evt_node_initialized.provisioned)) {
        sl_btmesh_node_start_unprov_beaconing(SL_BTMESH_PROVISIONEE_AUTO_UNPROV_BEACONING_CFG_VAL);
      }
      break;
    case sl_btmesh_evt_node_reset_id:
      sl_bt_nvm_erase(SL_BTMESH_LIGHTING_SERVER_PS_KEY_CFG_VAL);
      printf("Node reset\n");
      sl_udelay_wait(100000);
      sl_bt_system_reset(0);
      break;
    default:
      break;
  }
}

/** @} (end addtogroup provisionee) */
