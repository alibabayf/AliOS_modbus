#ifndef _BOARD_MGR_H
#define _BOARD_MGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
typedef enum addon_module{

	MODULE_GPIO = 0x1324,
	MODULE_UART,
	MODULE_I2C,
	MODULE_PWM,
	MODULE_ADC,
	MODULE_DAC,
	MODULE_SPI,
	MODULE_RTC,
	MODULE_BT,
	MODULE_IR,
	MODULE_I2S,
	MODULE_CAN,
	MODULE_SDIO,
	MODULE_USB,
	MODULE_I2C_GPIO,
	MODULE_NUMS,
}addon_module_m;


typedef struct item_handle{
	uint32_t handle;
}item_handle_t;


/**
 * initialize mgr system
 *
 * @param[in]  json_path  the path of boar-mgr file
 * @return  the operation status, 0 is OK, others is error
 */
int8_t 	 board_mgr_init(const char * json_path);

/**
 * initialize driver system
 *
 * @param[in]  driver_dir  the path of boar-driver file
 * @return  the operation status, 0 is OK, others is error
 */
int8_t	 board_drivers_init(const char * driver_dir);

/**
 * attach a  driver resource
 *
 * @param[in]  module   the module type of a driver
 * @param[in]  name_id  the name of a driver
 * @param[out] name_id  the resource of a driver
 * @return  the operation status, 0 is OK, others is error
 */
int8_t 	 board_attach_item(addon_module_m  module,const char * name_id,item_handle_t * out);

/**
 * release a  driver resource
 *
 * @param[in]  module   the module type of a driver
 * @param[in]  handle  the resource of a driver
 * @return  the operation status, 0 is OK, others is error
 */
int8_t 	 board_disattach_item(addon_module_m  module,item_handle_t* handle);

/**
 * the attach status of the driver and resource 
 *
 * @param[in]  module   the module type of a driver
 * @param[in]  handle  the resource of a driver
 * @return  the attach status, 1 is attach, others is dis-attach
 */
int8_t 	 board_check_attach_status(addon_module_m module,item_handle_t* handle);

/**
 * get the resource of a driver by name 
 *
 * @param[in]  module   the module type of a driver
 * @param[in]  name_id  the name of a driver
 * @return  driver resource,  null if not exist,otherwise it's right
 */
void * 	 board_get_node_by_name(addon_module_m module,const char * name_id);

/**
 * get the resource of a driver by the handle of a driver resource 
 *
 * @param[in]  module   the module type of a driver
 * @param[in]  handle  the resource of a driver
 * @return  driver resource,  null if not exist,otherwise it's right
 */
void * 	 board_get_node_by_handle(addon_module_m module,item_handle_t* handle);

#ifdef __cplusplus
}
#endif

#endif  /*_BOARD_MGR_H*/