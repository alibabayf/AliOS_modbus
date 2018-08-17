/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef SENSOR_DRV_API_H
#define SENSOR_DRV_API_H

#include <aos/aos.h>
#include "hal/soc/soc.h"
#include "hal/soc/i2c.h"
#include "hal/sensor.h"
#include "hal/soc/spi.h"

typedef struct _sensor_io_func_st
{
    int (*read)(uint16_t reg, uint8_t *data, uint16_t size);
    int (*write)(uint16_t reg, uint8_t *data, uint16_t size);
} sensor_io_func_st;

extern int32_t sensor_i2c_read(i2c_dev_t *i2c, uint16_t reg, uint8_t *data,
                               uint16_t size, uint32_t timeout);
extern int32_t sensor_i2c_write(i2c_dev_t *i2c, uint16_t reg, uint8_t *data,
                                uint16_t size, uint32_t timeout);
extern int32_t sensor_spi_read(spi_dev_t *spi, uint8_t *tx, uint8_t *rx,
                               uint16_t size, uint32_t timeout);
extern int32_t sensor_spi_write(spi_dev_t *spi, uint8_t *data, uint16_t size,
                                uint32_t timeout);

#endif /* SENSOR_DRV_API_H */
