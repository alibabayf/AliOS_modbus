/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */
 
#define CONFIG_LOGMACRO_DETAILS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <be_osal.h>
#include "gpio.h"
#include "be_jse_module.h"
#include "utils/be_common.h"
#include "board-mgr/board_mgr.h"


#define  GPIO_IRQ_RISING_EDGE	"rising"
#define  GPIO_IRQ_FALLING_EDGE	"falling"
#define  GPIO_IRQ_BOTH_EDGE		"both"



static be_jse_symbol_t * gpio_open(void){
	int32_t len = -1;
	char * data = NULL;
	int8_t ret = -1;
	int8_t result = -1;
	item_handle_t gpio_handle;
	gpio_handle.handle = 0xFFFFFFFF;
	be_jse_symbol_t * arg0 = NULL;
	gpio_dev_t * gpio_device = NULL;
	
	be_jse_handle_function(0,&arg0,NULL,NULL,NULL);
	if(!arg0 || !symbol_is_string(arg0)){
		goto out;
	}
	
	len = symbol_str_len(arg0);
	data = calloc(1,sizeof(char)*(len+1));
	if(NULL==data){
		goto out;
	}
	symbol_to_str(arg0,data,len);
	
	ret = board_attach_item(MODULE_GPIO,data,&gpio_handle);
	if(0 != ret){
		be_error("gpio","board_attach_item fail!\n");
		goto out;
	}
	be_debug("gpio","gpio handle:%u\n",gpio_handle.handle);
	
	gpio_device = board_get_node_by_handle(MODULE_GPIO,&gpio_handle);
	if(NULL == gpio_device){
		be_error("gpio","board_get_node_by_handle fail!\n");
		goto out;
	}
	ret = hal_gpio_init(gpio_device);
	if(0 != ret){
		be_error("gpio","hal_gpio_init fail!\n");
		goto out;
	}
	symbol_unlock(gpio_device->priv);
	gpio_device->priv = NULL;
	result = 0;
	
out:

	if(NULL != data){
		free(data);
		data = NULL;
	}
	symbol_unlock(arg0);
	if(0 != result){
		board_disattach_item(MODULE_GPIO,&gpio_handle);
		return new_int_symbol(-1);
	}
	return new_int_symbol(gpio_handle.handle);
}



static be_jse_symbol_t * gpio_close(void){
	int8_t ret = -1;
	int8_t result = -1;
	item_handle_t gpio_handle;
	be_jse_symbol_t * arg0 = NULL;
	gpio_dev_t * gpio_device = NULL;
	
	be_jse_handle_function(0,&arg0,NULL,NULL,NULL);
	if(!arg0 || !symbol_is_int(arg0)){
		goto out;
	}
	gpio_handle.handle = get_symbol_value_int(arg0);
	gpio_device = board_get_node_by_handle(MODULE_GPIO,&gpio_handle);
	if(NULL == gpio_device){
		be_error("gpio","board_get_node_by_handle fail!\n");
		goto out;
	}
	/* for esp32,hal_gpio_finalize will uninstall isr !
	ret = hal_gpio_finalize(gpio_device);
	if(0 != ret){
		be_error("gpio","hal_gpio_finalize fail!\n");
		goto out;
	}*/
	symbol_unlock(gpio_device->priv);
	gpio_device->priv = NULL;
	board_disattach_item(MODULE_GPIO,&gpio_handle);
	result = 0;
out:
	symbol_unlock(arg0);
	return new_int_symbol(result);
}


static be_jse_symbol_t * gpio_write(void){
	int8_t ret = -1;
	int8_t result = -1;
	int8_t level = 0;
	item_handle_t gpio_handle;
	be_jse_symbol_t * arg0 = NULL;
	be_jse_symbol_t * arg1 = NULL;
	gpio_dev_t * gpio_device = NULL;
	
	be_jse_handle_function(0,&arg0,&arg1,NULL,NULL);
	if(!arg0 || !symbol_is_int(arg0)){
		goto out;
	}
	gpio_handle.handle = get_symbol_value_int(arg0);
	gpio_device = board_get_node_by_handle(MODULE_GPIO,&gpio_handle);
	if(NULL == gpio_device){
		be_error("gpio","board_get_node_by_handle fail!\n");
		goto out;
	}
	if(!arg1 || !symbol_is_int(arg1)){
		goto out;
	}
	level = get_symbol_value_int(arg1);
	
	if(level){
		ret = hal_gpio_output_high(gpio_device);
	}else{
		ret = hal_gpio_output_low(gpio_device);
	}
	
	if(-1 == ret){
		be_error("gpio","gpio output set fail!\n");
		goto out;
	}
	result = 0;
out:
	symbol_unlock(arg0);
	symbol_unlock(arg1);
	
	return new_int_symbol(result);
}


static be_jse_symbol_t * gpio_read(void){
	int8_t ret = -1;
	item_handle_t gpio_handle;
	uint32_t level = 0;
	be_jse_symbol_t * arg0 = NULL;
	gpio_dev_t * gpio_device = NULL;
	
	be_jse_handle_function(0,&arg0,NULL,NULL,NULL);
	if(!arg0 || !symbol_is_int(arg0)){
		goto out;
	}
	gpio_handle.handle = get_symbol_value_int(arg0);
	gpio_device = board_get_node_by_handle(MODULE_GPIO,&gpio_handle);
	if(NULL == gpio_device){
		be_error("gpio","board_get_node_by_handle fail!\n");
		goto out;
	}
	hal_gpio_input_get(gpio_device,&level);
	
out:
	symbol_unlock(arg0);
	return new_int_symbol(level);
}



static void gpio_irq(void * arg){
	be_jse_symbol_t* fun_symbol = NULL;
	uint32_t value = 0;
	gpio_dev_t * gpio = (gpio_dev_t*)arg;

	if(NULL == gpio){
		be_error("gpio","param error!\n");
		return;
	}
	fun_symbol = (be_jse_symbol_t*)gpio->priv;
	if(!fun_symbol || !symbol_is_function(fun_symbol)){
		be_error("gpio","fun_symbol error!\n");
		return;
	}
	hal_gpio_input_get(gpio,&value);

	be_jse_symbol_t* argv[1];
	argv[0] = new_int_symbol(value);
	be_jse_execute_func(bone_engine_get_executor(),fun_symbol, 1, argv);

}


static be_jse_symbol_t * gpio_on(void){
	int32_t len = -1;
	char * data = NULL;
	int8_t ret = -1;
	int8_t result = -1;
	int8_t irq_edge = 0;
	item_handle_t gpio_handle;
	gpio_handle.handle = 0xFFFFFFFF;
	be_jse_symbol_t * arg0 = NULL;
	be_jse_symbol_t * arg1 = NULL;
	be_jse_symbol_t * arg2 = NULL;
	gpio_dev_t * gpio_device = NULL;
	
	be_jse_handle_function(0,&arg0,&arg1,&arg2,NULL);
	if(!arg0 || !symbol_is_int(arg0)){
		goto out;
	}
	gpio_handle.handle = get_symbol_value_int(arg0);
	gpio_device = board_get_node_by_handle(MODULE_GPIO,&gpio_handle);
	if(NULL == gpio_device){
		be_error("gpio","board_get_node_by_handle fail!\n");
		goto out;
	}
	if(!arg1 || !symbol_is_string(arg1)){
		goto out;
	}
	
	if(!arg2 || !symbol_is_function(arg2)){
		goto out;
	}
	len = symbol_str_len(arg1);
	data = calloc(1,sizeof(char)*(len+1));
	if(NULL==data){
		goto out;
	}
	symbol_to_str(arg1,data,len);

	
	if(0==strcmp(GPIO_IRQ_RISING_EDGE,data)){
		
		irq_edge = IRQ_TRIGGER_RISING_EDGE;
	}
	else if(0==strcmp(GPIO_IRQ_FALLING_EDGE,data)){
		
		irq_edge = IRQ_TRIGGER_FALLING_EDGE;
	}
	else if(0==strcmp(GPIO_IRQ_BOTH_EDGE,data)){
		irq_edge = IRQ_TRIGGER_BOTH_EDGES;
	}
	else{
		be_error("gpio","irq edge wrong!\n");
		goto out;
	}
	be_debug("gpio","irq_edge:%d port:%d\n",irq_edge,gpio_device->port);
	ret = hal_gpio_enable_irq(gpio_device,irq_edge,gpio_irq,gpio_device);
	if(ret<0){
		be_error("gpio","hal_gpio_enable_irq fail!\n");
		goto out;
	}
	symbol_unlock(gpio_device->priv);	
	gpio_device->priv = arg2;
	result = 0;
out:

	if(NULL != data){
		free(data);
		data = NULL;
	}
	symbol_unlock(arg0);
	symbol_unlock(arg1);
	if(0 != result){
		symbol_unlock(arg2);
	}
	return new_int_symbol(result);
}



static be_jse_symbol_t * gpio_module_handle_cb(be_jse_vm_ctx_t *execInfo, be_jse_symbol_t *var, const char *name){

	if(0 == strcmp(name,"open")){
		return gpio_open();
	}
	if(0 == strcmp(name,"read")){
		return gpio_read();
	}
	if(0 == strcmp(name,"write")){
		return gpio_write();
	}
	if(0 == strcmp(name,"on")){
		return gpio_on();
	}
	if(0 == strcmp(name,"close")){
		return gpio_close();
	}
	return (BE_JSE_FUNC_UNHANDLED);
}


void module_gpio_register(void){

    be_jse_module_load("GPIO", gpio_module_handle_cb);
}

