/***************************************************************************//**
 * @file
 * @brief Mesh Generic Base library
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "app_assert.h"

#include "sl_bt_api.h"
#include "sl_btmesh_api.h"

#include "sl_btmesh_generic_model_capi_types.h"
#include "sl_btmesh_lib.h"

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif // SL_COMPONENT_CATALOG_PRESENT

#include "sl_btmesh_generic_base.h"
#include "sl_btmesh_generic_base_config.h"

sl_status_t sl_btmesh_generic_base_init(void)
{
  return mesh_lib_init(SL_BTMESH_GENERIC_BASE_REGISTRY_INIT_SIZE,
                       SL_BTMESH_GENERIC_BASE_INCREMENT_CFG_VAL);
}

void sl_btmesh_generic_base_on_event(sl_btmesh_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_btmesh_evt_node_provisioned_id:
    case sl_btmesh_evt_node_initialized_id:
      sl_btmesh_generic_server_init_lightness();
      sl_btmesh_generic_server_init_common();
      break;
  }
}
