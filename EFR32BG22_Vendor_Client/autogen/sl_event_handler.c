#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_device_init_nvic.h"
#include "sl_board_init.h"
#include "sl_device_init_dcdc.h"
#include "sl_device_init_lfxo.h"
#include "sl_device_init_hfxo.h"
#include "sl_device_init_lfrco.h"
#include "sl_device_init_clocks.h"
#include "sl_device_init_emu.h"
#include "pa_conversions_efr32.h"
#include "sl_rail_util_pti.h"
#include "btl_interface.h"
#include "sl_board_control.h"
#include "sl_sleeptimer.h"
#include "app_timer.h"
#include "sl_bluetooth.h"
#include "sl_mbedtls.h"
#include "sl_mpu.h"
#include "nvm3_default.h"
#include "sl_btmesh.h"
#include "psa/crypto.h"
#include "sli_protocol_crypto.h"
#include "sl_power_manager.h"
#include "sl_rail_util_power_manager_init.h"

void sl_platform_init(void)
{
  sl_device_init_lfxo();
  sl_device_init_clocks();
  sl_power_manager_init();
}

void sl_driver_init(void)
{
}

void sl_service_init(void)
{
  sl_board_configure_vcom();
  sl_sleeptimer_init();
  sl_mbedtls_init();
  sl_mpu_disable_execute_from_ram();
  psa_crypto_init();
  sli_aes_seed_mask();
}

void sl_stack_init(void)
{

}

void sl_internal_app_init(void)
{
}

void sl_platform_process_action(void)
{
}

void sl_service_process_action(void)
{
  sli_app_timer_step();
}

void sl_stack_process_action(void)
{
  sl_bt_step();
  sl_btmesh_step();
}

void sl_internal_app_process_action(void)
{
}

