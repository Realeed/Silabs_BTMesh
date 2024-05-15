

#include <sl_common.h>
#include "sl_bluetooth.h"
#include "sl_assert.h"
#include "sl_bt_stack_init.h"
#include "sl_component_catalog.h"
#include "sl_btmesh_provisionee.h"
#if !defined(SL_CATALOG_KERNEL_PRESENT)
/**
 * Override @ref PendSV_Handler for the Link Layer task when Bluetooth runs
 * in baremetal mode. The override must not exist when Bluetooth runs in an RTOS
 * where the link layer task runs in a thread.
 */
#include <em_device.h>
void PendSV_Handler()
{
  sl_bt_priority_handle();
}
#endif

/**
 * Internal stack function to start the Bluetooth stack.
 *
 * @return SL_STATUS_OK if the stack was successfully started
 */
extern sl_status_t sli_bt_system_start_bluetooth();

void sl_bt_init(void)
{
  sl_bt_stack_init();
  sli_bt_system_start_bluetooth();
}

SL_WEAK void sl_bt_on_event(sl_bt_msg_t* evt)
{
  (void)(evt);
}

void sl_bt_process_event(sl_bt_msg_t *evt)
{
  sl_bt_provisionee_on_event(evt);
  sl_btmesh_bgapi_listener(evt);
  sl_bt_on_event(evt);
}

#if !defined(SL_CATALOG_KERNEL_PRESENT)
// When running in an RTOS, the stack events are processed in a dedicated
// event processing task, and these functions are not used at all.

SL_WEAK bool sl_bt_can_process_event(uint32_t len)
{
  (void)(len);
  return true;
}

void sl_bt_step(void)
{
  sl_bt_msg_t evt;
  sl_bt_run();
  uint32_t event_len = sl_bt_event_pending_len();
  if ((event_len == 0) || (!sl_bt_can_process_event(event_len))) {
    return;
  }
  sl_status_t status = sl_bt_pop_event(&evt);
  if(status != SL_STATUS_OK){
    return;
  }
  sl_bt_process_event(&evt);
}
#endif // !defined(SL_CATALOG_KERNEL_PRESENT)
