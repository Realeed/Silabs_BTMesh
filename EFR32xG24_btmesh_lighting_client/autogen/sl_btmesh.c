

#include <sl_common.h>
#include "sl_btmesh.h"
#include "sl_component_catalog.h"
#include "sl_btmesh_generic_base.h"
#include "sl_btmesh_generic_base.h"
#include "sl_btmesh_provisionee.h"

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const btmesh_class_table[] =
{
  SL_BTMESH_BGAPI_CLASS(generic_client),
  SL_BTMESH_BGAPI_CLASS(health_server),
  SL_BTMESH_BGAPI_CLASS(proxy),
  SL_BTMESH_BGAPI_CLASS(proxy_server),
  SL_BTMESH_BGAPI_CLASS(node),
  NULL
};
void mesh_advertiser_legacy();

void sl_btmesh_init(void)
{
  sl_btmesh_init_classes(btmesh_class_table);
  sl_btmesh_generic_base_init();
  mesh_advertiser_legacy();
}

SL_WEAK void sl_btmesh_on_event(sl_btmesh_msg_t* evt)
{
  (void)(evt);
}

void sl_btmesh_process_event(sl_btmesh_msg_t *evt)
{
  sl_btmesh_generic_base_on_event(evt);
  sl_btmesh_provisionee_on_event(evt);
  sl_btmesh_on_event(evt);
}

#if !defined(SL_CATALOG_KERNEL_PRESENT)
// When running in an RTOS, the stack events are processed in a dedicated
// event processing task, and these functions are not used at all.
SL_WEAK bool sl_btmesh_can_process_event(uint32_t len)
{
  (void)(len);
  return true;
}

void sl_btmesh_step(void)
{
  sl_btmesh_msg_t evt;
  uint32_t event_len = sl_btmesh_event_pending_len();
  if ((event_len == 0) || (!sl_btmesh_can_process_event(event_len))) {
    return;
  }
  sl_status_t status = sl_btmesh_pop_event(&evt);
  if(status != SL_STATUS_OK){
    return;
  }
  sl_btmesh_process_event(&evt);
}
#endif // !defined(SL_CATALOG_KERNEL_PRESENT)
