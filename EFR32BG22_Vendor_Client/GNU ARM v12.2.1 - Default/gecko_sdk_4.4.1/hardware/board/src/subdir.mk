################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.c \
../gecko_sdk_4.4.1/hardware/board/src/sl_board_init.c 

OBJS += \
./gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.o \
./gecko_sdk_4.4.1/hardware/board/src/sl_board_init.o 

C_DEPS += \
./gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.d \
./gecko_sdk_4.4.1/hardware/board/src/sl_board_init.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.o: ../gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.c gecko_sdk_4.4.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32BG22C224F512IM40=1' '-DSL_APP_PROPERTIES=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=38400000' '-DSL_BOARD_NAME="BRD4184A"' '-DSL_BOARD_REV="A01"' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config\btconf" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config\btmeshconf" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\Device\SiliconLabs\EFR32BG22\Include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\common\util\app_assert" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\btmesh\common\app_btmesh_util" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\common\util\app_timer" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\common\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\protocol\bluetooth\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\protocol\bluetooth\bgstack\ll\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\hardware\board\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\bootloader" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\bootloader\api" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\btmesh\common\btmesh_provisionee" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\CMSIS\Core\Include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_cryptoacc_library\include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_cryptoacc_library\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\device_init\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emdrv\common\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emlib\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_mbedtls_support\config" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_mbedtls_support\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\third_party\mbedtls\include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\third_party\mbedtls\library" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\mpu\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\hardware\driver\mx25_flash_shutdown\inc\sl_mx25_flash_shutdown_usart" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emdrv\nvm3\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\power_manager\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_psa_driver\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\common" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\ble" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\wmbus" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\zwave" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\chip\efr32\efr32xg2x" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\sidewalk" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\pa-conversions" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\pa-conversions\efr32xg22" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\rail_util_power_manager_init" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\rail_util_pti" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\se_manager\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\se_manager\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\silicon_labs\silabs_core\memory_manager" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\common\toolchain\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\system\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\sleeptimer\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_protocol_crypto\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\udelay\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\autogen" -O0 -Wall -Wextra -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.1/hardware/board/src/sl_board_control_gpio.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_4.4.1/hardware/board/src/sl_board_init.o: ../gecko_sdk_4.4.1/hardware/board/src/sl_board_init.c gecko_sdk_4.4.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32BG22C224F512IM40=1' '-DSL_APP_PROPERTIES=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=38400000' '-DSL_BOARD_NAME="BRD4184A"' '-DSL_BOARD_REV="A01"' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config\btconf" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\config\btmeshconf" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\Device\SiliconLabs\EFR32BG22\Include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\common\util\app_assert" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\btmesh\common\app_btmesh_util" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\common\util\app_timer" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\common\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\protocol\bluetooth\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\protocol\bluetooth\bgstack\ll\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\hardware\board\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\bootloader" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\bootloader\api" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\app\btmesh\common\btmesh_provisionee" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\CMSIS\Core\Include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_cryptoacc_library\include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_cryptoacc_library\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\device_init\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emdrv\common\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emlib\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_mbedtls_support\config" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_mbedtls_support\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\third_party\mbedtls\include" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\third_party\mbedtls\library" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\mpu\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\hardware\driver\mx25_flash_shutdown\inc\sl_mx25_flash_shutdown_usart" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\emdrv\nvm3\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\power_manager\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_psa_driver\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\common" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\ble" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\wmbus" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\zwave" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\chip\efr32\efr32xg2x" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\protocol\sidewalk" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\pa-conversions" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\pa-conversions\efr32xg22" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\rail_util_power_manager_init" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\radio\rail_lib\plugin\rail_util_pti" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\se_manager\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\se_manager\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\util\silicon_labs\silabs_core\memory_manager" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\common\toolchain\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\system\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\sleeptimer\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\security\sl_component\sl_protocol_crypto\src" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\gecko_sdk_4.4.1\platform\service\udelay\inc" -I"C:\Users\hsarg\SimplicityStudio\BTMesh\EFR32BG22_Vendor_Client\autogen" -O0 -Wall -Wextra -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.1/hardware/board/src/sl_board_init.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


