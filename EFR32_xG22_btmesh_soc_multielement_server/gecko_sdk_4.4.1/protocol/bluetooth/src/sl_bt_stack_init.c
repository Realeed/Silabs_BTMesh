/***************************************************************************//**
 * @brief Initialization APIs for Bluetooth stack
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// Include the catalog first to make sure everything below can see it
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"

#define SLI_BT_INIT_MODE init_always

#include "sl_status.h"
#include "sl_bt_api.h"
#include "sl_bt_stack_config.h"
#include "sl_bt_stack_init.h"
#include "sl_bluetooth.h"
#include "sl_bluetooth_config.h"
#include "sl_btctrl_linklayer.h"
#include "sli_bt_gattdb_def.h"
#include "sli_bt_config_defs.h"

#ifdef SL_CATALOG_GATT_CONFIGURATION_PRESENT
extern const sli_bt_gattdb_t gattdb;
#endif // SL_CATALOG_GATT_CONFIGURATION_PRESENT

// Forward declaration of BGAPI classes
SLI_BT_DECLARE_BGAPI_CLASS(bt, system);
SLI_BT_DECLARE_BGAPI_CLASS(bt, nvm);
SLI_BT_DECLARE_BGAPI_CLASS(bt, ota);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gap);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sm);
SLI_BT_DECLARE_BGAPI_CLASS(bt, external_bondingdb);
SLI_BT_DECLARE_BGAPI_CLASS(bt, accept_list);
SLI_BT_DECLARE_BGAPI_CLASS(bt, resolving_list);
SLI_BT_DECLARE_BGAPI_CLASS(bt, advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, legacy_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, extended_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, periodic_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, scanner);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, pawr_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync_scanner);
SLI_BT_DECLARE_BGAPI_CLASS(bt, periodic_sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, pawr_sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, past_receiver);
SLI_BT_DECLARE_BGAPI_CLASS(bt, advertiser_past);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync_past);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cs);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cs_test);
SLI_BT_DECLARE_BGAPI_CLASS(bt, l2cap);
SLI_BT_DECLARE_BGAPI_CLASS(bt, connection);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gatt);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gattdb);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gatt_server);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cte_receiver);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cte_transmitter);
SLI_BT_DECLARE_BGAPI_CLASS(bt, test);
SLI_BT_DECLARE_BGAPI_CLASS(bt, coex);
SLI_BT_DECLARE_BGAPI_CLASS(bt, resource);
SLI_BT_DECLARE_BGAPI_CLASS(bt, connection_analyzer);

// Forward declaration of the internal Bluetooth stack init function
sl_status_t sli_bt_init_stack(const sl_bt_configuration_t *config,
                              const struct sli_bt_feature_use *features,
                              const struct sli_bgapi_class * const *bgapi_classes);

// The scanner event handler is included if the legacy or extended scanner
// component is used.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_SCANNER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_SCANNER_PRESENT)
#define SLI_BT_ENABLE_SCANNER_BASE
#endif

// If the build configuration needs a specific feature, we pick it for inclusion
// in the feature and BGAPI lists, as applicable.

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYSTEM_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, system);
#define SLI_BT_FEATURE_SYSTEM SLI_BT_USE_FEATURE(bt, system),
#define SLI_BT_BGAPI_SYSTEM SLI_BT_USE_BGAPI_CLASS(bt, system),
#endif

#define SLI_BT_FEATURE_ON_DEMAND_START

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_NVM_PRESENT)
#define SLI_BT_BGAPI_NVM SLI_BT_USE_BGAPI_CLASS(bt, nvm),
#endif

#define SLI_BT_BGAPI_OTA_CONFIG

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_GAP_PRESENT)
#define SLI_BT_BGAPI_GAP SLI_BT_USE_BGAPI_CLASS(bt, gap),
#endif

#define SLI_BT_FEATURE_SM
#define SLI_BT_BGAPI_SM

#define SLI_BT_FEATURE_BUILTIN_BONDING_DATABASE

#define SLI_BT_FEATURE_EXTERNAL_BONDING_DATABASE
#define SLI_BT_BGAPI_EXTERNAL_BONDINGDB


#define SLI_BT_FEATURE_ACCEPT_LIST
#define SLI_BT_BGAPI_ACCEPT_LIST


#define SLI_BT_FEATURE_RESOLVING_LIST
#define SLI_BT_BGAPI_RESOLVING_LIST

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, advertiser);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, advertiser);
#define SLI_BT_FEATURE_ADVERTISER SLI_BT_USE_FEATURE_WITH_CONFIG(bt, advertiser, SLI_BT_FEATURE_CONFIG_NAME(bt, advertiser)),
#define SLI_BT_BGAPI_ADVERTISER   SLI_BT_USE_BGAPI_CLASS(bt, advertiser),
#endif

#define SLI_BT_FEATURE_ADVERTISER_COMPATIBILITY

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_ADVERTISER_PRESENT)
#define SLI_BT_BGAPI_LEGACY_ADVERTISER SLI_BT_USE_BGAPI_CLASS(bt, legacy_advertiser),
#endif

#define SLI_BT_FEATURE_EXTENDED_ADVERTISER

#define SLI_BT_BGAPI_EXTENDED_ADVERTISER

#define SLI_BT_FEATURE_PERIODIC_ADV

#define SLI_BT_FEATURE_PERIODIC_ADVERTISER
#define SLI_BT_BGAPI_PERIODIC_ADVERTISER

#define SLI_BT_BGAPI_PAWR_ADVERTISER

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, scanner);
#define SLI_BT_FEATURE_SCANNER SLI_BT_USE_FEATURE(bt, scanner),
#define SLI_BT_BGAPI_SCANNER SLI_BT_USE_BGAPI_CLASS(bt, scanner),
#endif

#define SLI_BT_FEATURE_SCANNER_COMPATIBILITY

#if defined(SLI_BT_ENABLE_SCANNER_BASE)
SLI_BT_DECLARE_FEATURE(bt, scanner_base);
#define SLI_BT_FEATURE_SCANNER_BASE SLI_BT_USE_FEATURE(bt, scanner_base),
#endif

#define SLI_BT_FEATURE_EXTENDED_SCANNER

#define SLI_BT_FEATURE_SYNC
#define SLI_BT_BGAPI_SYNC

#define SLI_BT_FEATURE_SYNC_SCANNER

#define SLI_BT_BGAPI_SYNC_SCANNER

#define SLI_BT_FEATURE_SYNC_COMPATIBILITY

#define SLI_BT_BGAPI_PERIODIC_SYNC

#define SLI_BT_BGAPI_PAWR_SYNC

#define SLI_BT_BGAPI_PAST_RECEIVER

#define SLI_BT_BGAPI_ADVERTISER_PAST

#define SLI_BT_BGAPI_SYNC_PAST

#define SLI_BT_FEATURE_CS
#define SLI_BT_BGAPI_CS

#define SLI_BT_FEATURE_CS_TEST
#define SLI_BT_BGAPI_CS_TEST

#define SLI_BT_FEATURE_L2CAP
#define SLI_BT_BGAPI_L2CAP

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, connection);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, connection);
#define SLI_BT_FEATURE_CONNECTION SLI_BT_USE_FEATURE_WITH_CONFIG(bt, connection, SLI_BT_FEATURE_CONFIG_NAME(bt, connection)),
#define SLI_BT_BGAPI_CONNECTION SLI_BT_USE_BGAPI_CLASS(bt, connection),
#endif

#define SLI_BT_FEATURE_CONNECTION_STATISTICS


#define SLI_BT_FEATURE_POWER_CONTROL

#define SLI_BT_FEATURE_USER_POWER_CONTROL

#define SLI_BT_FEATURE_GATT
#define SLI_BT_BGAPI_GATT

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_DYNAMIC_GATTDB_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, dynamic_gattdb);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, dynamic_gattdb);
#define SLI_BT_FEATURE_DYNAMIC_GATTDB SLI_BT_USE_FEATURE_WITH_CONFIG(bt, dynamic_gattdb, SLI_BT_FEATURE_CONFIG_NAME(bt, dynamic_gattdb)),
#define SLI_BT_BGAPI_DYNAMIC_GATTDB SLI_BT_USE_BGAPI_CLASS(bt, gattdb),
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_GATT_SERVER_PRESENT)
#define SLI_BT_BGAPI_GATT_SERVER SLI_BT_USE_BGAPI_CLASS(bt, gatt_server),
#endif

#define SLI_BT_FEATURE_CTE_RECEIVER
#define SLI_BT_BGAPI_CTE_RECEIVER
#endif

#define SLI_BT_FEATURE_CTE_TRANSMITTER
#define SLI_BT_BGAPI_CTE_TRANSMITTER

#define SLI_BT_FEATURE_TEST
#define SLI_BT_BGAPI_TEST

#define SLI_BT_FEATURE_WHITELISTING

#define SLI_BT_BGAPI_COEX

#define SLI_BT_BGAPI_RESOURCE

#define SLI_BT_FEATURE_ACCURATE_API_ADDRESS_TYPES

#define SLI_BT_BGAPI_CONNECTION_ANALYZER

/** @brief Structure that specifies the Bluetooth configuration */
static const sl_bt_configuration_t bt_config = SL_BT_CONFIG_DEFAULT;

/** @brief Table of used Bluetooth features */
static const struct sli_bt_feature_use bt_used_features[] =
{
  // Invoke the feature inclusion macro for each feature. Depending on the build
  // configuration, the feature inclusion rules above have defined the macro to
  // either empty or the relevant feature use declaration.
  SLI_BT_FEATURE_ON_DEMAND_START
  SLI_BT_FEATURE_SYSTEM
  SLI_BT_FEATURE_SM
  SLI_BT_FEATURE_BUILTIN_BONDING_DATABASE
  SLI_BT_FEATURE_EXTERNAL_BONDING_DATABASE
  SLI_BT_FEATURE_ACCEPT_LIST
  SLI_BT_FEATURE_RESOLVING_LIST
  SLI_BT_FEATURE_SCANNER
  SLI_BT_FEATURE_SCANNER_COMPATIBILITY
  SLI_BT_FEATURE_SCANNER_BASE
  SLI_BT_FEATURE_EXTENDED_SCANNER
  SLI_BT_FEATURE_SYNC
  SLI_BT_FEATURE_SYNC_SCANNER
  SLI_BT_FEATURE_SYNC_COMPATIBILITY
  SLI_BT_FEATURE_ADVERTISER
  SLI_BT_FEATURE_ADVERTISER_COMPATIBILITY
  SLI_BT_FEATURE_EXTENDED_ADVERTISER
  SLI_BT_FEATURE_PERIODIC_ADV
  SLI_BT_FEATURE_PERIODIC_ADVERTISER
  SLI_BT_FEATURE_CS
  SLI_BT_FEATURE_CS_TEST
  SLI_BT_FEATURE_L2CAP
  SLI_BT_FEATURE_CONNECTION
  SLI_BT_FEATURE_CONNECTION_STATISTICS
  SLI_BT_FEATURE_DYNAMIC_GATTDB
  SLI_BT_FEATURE_CTE_RECEIVER
  SLI_BT_FEATURE_CTE_TRANSMITTER
  SLI_BT_FEATURE_TEST
  SLI_BT_FEATURE_POWER_CONTROL
  SLI_BT_FEATURE_USER_POWER_CONTROL
  SLI_BT_FEATURE_GATT
  SLI_BT_FEATURE_WHITELISTING
  SLI_BT_FEATURE_ACCURATE_API_ADDRESS_TYPES
  { NULL, NULL }
};

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const bt_bgapi_classes[] =
{
  // Invoke the BGAPI class inclusion macro for each feature that provides a
  // BGAPI class. Depending on the build configuration, the feature inclusion
  // rules above have defined the macro to either empty or the relevant BGAPI
  // class declaration.
  SLI_BT_BGAPI_SYSTEM
  SLI_BT_BGAPI_NVM
  SLI_BT_BGAPI_OTA_CONFIG
  SLI_BT_BGAPI_GAP
  SLI_BT_BGAPI_SM
  SLI_BT_BGAPI_EXTERNAL_BONDINGDB
  SLI_BT_BGAPI_ACCEPT_LIST
  SLI_BT_BGAPI_RESOLVING_LIST
  SLI_BT_BGAPI_ADVERTISER
  SLI_BT_BGAPI_LEGACY_ADVERTISER
  SLI_BT_BGAPI_EXTENDED_ADVERTISER
  SLI_BT_BGAPI_PERIODIC_ADVERTISER
  SLI_BT_BGAPI_PAWR_ADVERTISER
  SLI_BT_BGAPI_SCANNER
  SLI_BT_BGAPI_SYNC
  SLI_BT_BGAPI_SYNC_SCANNER
  SLI_BT_BGAPI_PERIODIC_SYNC
  SLI_BT_BGAPI_PAWR_SYNC
  SLI_BT_BGAPI_PAST_RECEIVER
  SLI_BT_BGAPI_ADVERTISER_PAST
  SLI_BT_BGAPI_SYNC_PAST
  SLI_BT_BGAPI_CS
  SLI_BT_BGAPI_CS_TEST
  SLI_BT_BGAPI_L2CAP
  SLI_BT_BGAPI_CONNECTION
  SLI_BT_BGAPI_GATT
  SLI_BT_BGAPI_DYNAMIC_GATTDB
  SLI_BT_BGAPI_GATT_SERVER
  SLI_BT_BGAPI_CTE_RECEIVER
  SLI_BT_BGAPI_CTE_TRANSMITTER
  SLI_BT_BGAPI_TEST
  SLI_BT_BGAPI_COEX
  SLI_BT_BGAPI_RESOURCE
  SLI_BT_BGAPI_CONNECTION_ANALYZER
  NULL
};

// Forward declaration of Bluetooth controller init functions
extern void ll_addrEnable();
extern sl_status_t sl_bt_ll_deinit();
#include "sl_bt_ll_config.h"
extern sl_status_t ll_connPowerControlEnable(const sl_bt_ll_power_control_config_t *);
extern void sl_bt_init_app_controlled_tx_power();
extern sl_status_t sl_btctrl_init_sniff(uint8_t);
extern void sl_btctrl_deinit_sniff(void);

/**
 * @brief Initialize controller features according to the feature selection.
 *
 * This function is called by the Bluetooth host stack when Bluetooth is started.
 */
sl_status_t sli_bt_init_controller_features()
{
  sl_status_t status = SL_STATUS_OK;


#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
  sl_btctrl_init_adv();
#endif


#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
  sl_btctrl_init_scan();
#endif


#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
  sl_btctrl_init_conn();
#if !defined(SL_CATALOG_BLUETOOTH_CONNECTION_PHY_UPDATE_INCOMPATIBLE_PRESENT)
  sl_btctrl_init_phy();
#endif
#endif

  return status;
}

/**
 * @brief De-initialize controller features according to the feature selection.
 *
 * This function is called by the Bluetooth host stack when Bluetooth is stopped.
 */
void sli_bt_deinit_controller_features()
{
  (void) sl_bt_ll_deinit();
}

// Initialize the Bluetooth stack.
sl_status_t sl_bt_stack_init()
{
  // Initialize the Bluetooth stack with the given configuration, features, and BGAPI classes
  return sli_bt_init_stack(&bt_config, bt_used_features, bt_bgapi_classes);
}
