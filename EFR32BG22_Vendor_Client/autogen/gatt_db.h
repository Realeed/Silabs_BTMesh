/********************************************************************
 * Autogenerated file, do not edit.
 *******************************************************************/

#ifndef __GATT_DB_H
#define __GATT_DB_H

#include "sli_bt_gattdb_def.h"

extern const sli_bt_gattdb_t gattdb;

#define gattdb_generic_attribute              1
#define gattdb_service_changed_char           3
#define gattdb_database_hash                  6
#define gattdb_client_support_features        8
#define gattdb_device_name                    11
#define gattdb_device_information             14
#define gattdb_manufacturer_name_string       16
#define gattdb_system_id                      18

typedef enum {
  mesh_default                   = 0x0001,
  gattdb_all_caps = 0x0001
} gattdb_cap_t;

#endif // __GATT_DB_H