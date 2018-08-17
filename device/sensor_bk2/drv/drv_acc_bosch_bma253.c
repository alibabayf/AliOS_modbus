/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aos/aos.h>
#include <vfs_conf.h>
#include <vfs_err.h>
#include <vfs_register.h>
#include <hal/base.h>
#include "common.h"
#include "hal/sensor.h"
#include "sensor_drv_api.h"
#include "sensor_hal.h"


#define BMA253_I2C_ADDR1 (0x18)
#define BMA253_I2C_ADDR2 (0x19)
#define BMA253_I2C_ADDR3 (0x10)
#define BMA253_I2C_ADDR4 (0x11)
#define BMA253_I2C_ADDR_TRANS(n) ((n) << 1)
#define BMA253_I2C_ADDR BMA253_I2C_ADDR_TRANS(BMA253_I2C_ADDR1)


#define BMA253_INIT_VALUE (0)
#define BMA253_GEN_READ_WRITE_LENGTH (1)
#define BMA253_INTERFACE_IDLE_TIME_DELAY (1)
#define BMA253_LSB_MSB_READ_LENGTH (2)
#define BMA253_SHIFT_TWO_BITS (2)
#define BMA253_SHIFT_FOUR_BITS (4)
#define BMA253_SHIFT_FIVE_BITS (5)
#define BMA253_SHIFT_SIX_BITS (6)
#define BMA253_SHIFT_EIGHT_BITS (8)
#define BMA253_12_BIT_SHIFT (0xF0)

#define BMA253_FIFO_MODE_STATUS_RANGE (2)
#define BMA253_FIFO_DATA_SELECT_RANGE (4)
#define BMA253_FIFO_MODE_RANGE (4)
#define BMA253_FIFO_WML_RANGE (32)
#define BMA253_FIFO_XYZ_DATA_ENABLED (0x00)
#define BMA253_FIFO_X_DATA_ENABLED (0x01)
#define BMA253_FIFO_Y_DATA_ENABLED (0x02)
#define BMA253_FIFO_Z_DATA_ENABLED (0x03)
#define BMA253_FIFO_DATA_ENABLED_MASK (0x03)
#define BMA253_FIFO_XYZ_AXES_FRAME_SIZE (6)
#define BMA253_FIFO_SINGLE_AXIS_FRAME_SIZE (2)
#define BMA253_ACCEL_BW_MIN_RANGE (7)
#define BMA253_ACCEL_BW_1000HZ_RANGE (15)
#define BMA253_ACCEL_BW_MAX_RANGE (16)
#define BMA253_SLEEP_DURN_MIN_RANGE (4)
#define BMA253_SLEEP_TIMER_MODE_RANGE (2)
#define BMA253_SLEEP_DURN_MAX_RANGE (16)
#define BMA253_POWER_MODE_RANGE (6)
#define BMA253_SELF_TEST_AXIS_RANGE (4)
#define BMA253_SELF_TEST_SIGN_RANGE (2)

#define BMA253_EEP_OFFSET (0x16)
#define BMA253_IMAGE_BASE (0x38)
#define BMA253_IMAGE_LEN (22)
#define BMA253_CHIP_ID_ADDR (0x00)
#define BMA253_CHIP_ID_VALUE (0xFA)
#define BMA253_X_AXIS_LSB_ADDR (0x02)
#define BMA253_X_AXIS_MSB_ADDR (0x03)
#define BMA253_Y_AXIS_LSB_ADDR (0x04)
#define BMA253_Y_AXIS_MSB_ADDR (0x05)
#define BMA253_Z_AXIS_LSB_ADDR (0x06)
#define BMA253_Z_AXIS_MSB_ADDR (0x07)
#define BMA253_TEMP_ADDR (0x08)
#define BMA253_STAT1_ADDR (0x09)
#define BMA253_STAT2_ADDR (0x0A)
#define BMA253_STAT_TAP_SLOPE_ADDR (0x0B)
#define BMA253_STAT_ORIENT_HIGH_ADDR (0x0C)
#define BMA253_STAT_FIFO_ADDR (0x0E)
#define BMA253_RANGE_SELECT_ADDR (0x0F)
#define BMA253_BW_SELECT_ADDR (0x10)
#define BMA253_MODE_CTRL_ADDR (0x11)
#define BMA253_LOW_NOISE_CTRL_ADDR (0x12)
#define BMA253_DATA_CTRL_ADDR (0x13)
#define BMA253_RST_ADDR (0x14)
#define BMA253_INTR_ENABLE1_ADDR (0x16)
#define BMA253_INTR_ENABLE2_ADDR (0x17)
#define BMA253_INTR_SLOW_NO_MOTION_ADDR (0x18)
#define BMA253_INTR1_PAD_SELECT_ADDR (0x19)
#define BMA253_INTR_DATA_SELECT_ADDR (0x1A)
#define BMA253_INTR2_PAD_SELECT_ADDR (0x1B)
#define BMA253_INTR_SOURCE_ADDR (0x1E)
#define BMA253_INTR_SET_ADDR (0x20)
#define BMA253_INTR_CTRL_ADDR (0x21)
#define BMA253_LOW_DURN_ADDR (0x22)
#define BMA253_LOW_THRES_ADDR (0x23)
#define BMA253_LOW_HIGH_HYST_ADDR (0x24)
#define BMA253_HIGH_DURN_ADDR (0x25)
#define BMA253_HIGH_THRES_ADDR (0x26)
#define BMA253_SLOPE_DURN_ADDR (0x27)
#define BMA253_SLOPE_THRES_ADDR (0x28)
#define BMA253_SLOW_NO_MOTION_THRES_ADDR (0x29)
#define BMA253_TAP_PARAM_ADDR (0x2A)
#define BMA253_TAP_THRES_ADDR (0x2B)
#define BMA253_ORIENT_PARAM_ADDR (0x2C)
#define BMA253_THETA_BLOCK_ADDR (0x2D)
#define BMA253_THETA_FLAT_ADDR (0x2E)
#define BMA253_FLAT_HOLD_TIME_ADDR (0x2F)
#define BMA253_SELFTEST_ADDR (0x32)
#define BMA253_EEPROM_CTRL_ADDR (0x33)
#define BMA253_SERIAL_CTRL_ADDR (0x34)
#define BMA253_OFFSET_CTRL_ADDR (0x36)
#define BMA253_OFFSET_PARAMS_ADDR (0x37)
#define BMA253_OFFSET_X_AXIS_ADDR (0x38)
#define BMA253_OFFSET_Y_AXIS_ADDR (0x39)
#define BMA253_OFFSET_Z_AXIS_ADDR (0x3A)
#define BMA253_GP0_ADDR (0x3B)
#define BMA253_GP1_ADDR (0x3C)
#define BMA253_FIFO_MODE_ADDR (0x3E)
#define BMA253_FIFO_DATA_OUTPUT_ADDR (0x3F)
#define BMA253_FIFO_WML_TRIG (0x30)

#define BMA253_12_RESOLUTION (0)
#define BMA253_10_RESOLUTION (1)
#define BMA253_14_RESOLUTION (2)

#define BMA253_ENABLE_SOFT_RESET_VALUE (0xB6)
#define BMA253_RANGE_SELECT_POS (0)
#define BMA253_RANGE_SELECT_LEN (4)
#define BMA253_RANGE_SELECT_MSK (0x0F)
#define BMA253_RANGE_SELECT_REG BMA253_RANGE_SELECT_ADDR

#define BMA253_RANGE_2G (3)
#define BMA253_RANGE_4G (5)
#define BMA253_RANGE_8G (8)
#define BMA253_RANGE_16G (12)

#define BMA253_BW_15_63 (15)
#define BMA253_BW_31_25 (31)
#define BMA253_BW_62_5 (62)
#define BMA253_BW_125 (125)
#define BMA253_BW_250 (250)
#define BMA253_BW_500 (500)
#define BMA253_BW_1000 (1000)

#define BMA253_BW_7_81HZ (0x08)
#define BMA253_BW_15_63HZ (0x09)
#define BMA253_BW_31_25HZ (0x0A)
#define BMA253_BW_62_50HZ (0x0B)
#define BMA253_BW_125HZ (0x0C)
#define BMA253_BW_250HZ (0x0D)
#define BMA253_BW_500HZ (0x0E)
#define BMA253_BW_1000HZ (0x0F)
#define BMA253_BW_BIT_MASK (~0x0F)

#define BMA253_SLEEP_DURN_0_5MS (0x05)
#define BMA253_SLEEP_DURN_1MS (0x06)
#define BMA253_SLEEP_DURN_2MS (0x07)
#define BMA253_SLEEP_DURN_4MS (0x08)
#define BMA253_SLEEP_DURN_6MS (0x09)
#define BMA253_SLEEP_DURN_10MS (0x0A)
#define BMA253_SLEEP_DURN_25MS (0x0B)
#define BMA253_SLEEP_DURN_50MS (0x0C)
#define BMA253_SLEEP_DURN_100MS (0x0D)
#define BMA253_SLEEP_DURN_500MS (0x0E)
#define BMA253_SLEEP_DURN_1S (0x0F)
#define BMA253_SLEEP_DURN_POS (1)
#define BMA253_SLEEP_DURN_LEN (4)
#define BMA253_SLEEP_DURN_MSK (0x1E)
#define BMA253_SLEEP_DURN_REG BMA253_MODE_CTRL_ADDR
#define BMA253_SLEEP_MODE (0x40)
#define BMA253_DEEP_SUSPEND_MODE (0x20)
#define BMA253_SUSPEND_MODE (0x80)
#define BMA253_NORMAL_MODE (0x40)

#define BMA253_LOWPOWER_MODE (0x40)

#define BMA253_MODE_CTRL_POS (5)
#define BMA253_MODE_CTRL_LEN (3)
#define BMA253_MODE_CTRL_MSK (0xE0)
#define BMA253_MODE_CTRL_REG BMA253_MODE_CTRL_ADDR
#define BMA253_LOW_POWER_MODE_POS (6)
#define BMA253_LOW_POWER_MODE_LEN (1)
#define BMA253_LOW_POWER_MODE_MSK (0x40)
#define BMA253_LOW_POWER_MODE_REG BMA253_LOW_NOISE_CTRL_ADDR

#define BMA253_DEFAULT_ODR_100HZ (100)

#define BMA253_FIFO_DEPTH_MAX (32)
#define BMA253_FIFO_DATA_NUM (6)
#define BMA253_FIFO_BUFF_LEN (BMA253_FIFO_DEPTH_MAX * BMA253_FIFO_DATA_NUM)
// bma253 sensitivity factor table, the unit is LSB/g
#define BMA253_IRQ_CLEAR_VAL (0x80)
#define BMA253_IRQ_LATCHED (0x0f)
#define BMA253_IRQ_NON_LATCHED (0)

#define BMA253_IRQ_FIFO_STAT (0x40)
#define BMA253_IRQ_DATA_READY_STAT (0x80)

#define BMA253_IRQ_DATA_READY_ENABLE (0x10)
#define BMA253_IRQ_FIFO_FULL_ENABLE (0x20)
#define BMA253_IRQ_FIFO_WML_ENABLE (0x40)

#define BMA253_IRQ_MAP_DATA_INT2 (0x80)
#define BMA253_IRQ_MAP_FIFO_WML_INT2 (0x40)
#define BMA253_IRQ_CONFIG_PUSH_PULL (0x04)
#define BMA253_IRQ_FIFO_WML_MODE (0x40)
#define BMA253_IRQ_PIN (62)
#define BMA253_FIFO_DEPTH_USD (20)
#define BMA253_GET_BITSLICE(regvar, bitname) \
    ((regvar & bitname##_MSK) >> bitname##_POS)

#define BMA253_SET_BITSLICE(regvar, bitname, val) \
    ((regvar & ~bitname##_MSK) | ((val << bitname##_POS) & bitname##_MSK))

// bma253 sensitivity factor table, the unit is LSB/g
static uint32_t           bma253_factor[4]       = { 1024, 512, 256, 128 };
static uint32_t           current_factor         = 0;
static uint32_t           bma253_fifo_wml        = 0;
static work_mode_e        bma253_work_mode       = 0;
static gpio_irq_trigger_t bma253_irq_mode_rising = IRQ_TRIGGER_RISING_EDGE;

uint8_t   bma253_fifo_data[BMA253_FIFO_BUFF_LEN] = { 0 };
i2c_dev_t bma253_ctx                             = {
    .port                 = 3,
    .config.address_width = 8,
    .config.freq          = 400000,
    .config.dev_addr      = BMA253_I2C_ADDR,
};
static int drv_acc_bosch_bma253_data_ready_init(void);
static int drv_acc_bosch_bma253_fifo_init(void);

static int drv_acc_bosch_bma253_fifo_wml_set(uint32_t num)
{
    int ret = 0;

    if (unlikely(num > BMA253_FIFO_DEPTH_MAX)) {
        return -1;
    }

    bma253_fifo_wml = num;

    return 0;
}


static int drv_acc_bosch_bma253_soft_reset(i2c_dev_t *drv)
{
    int     ret   = 0;
    uint8_t value = BMA253_ENABLE_SOFT_RESET_VALUE;
    ret           = sensor_i2c_write(drv, BMA253_RST_ADDR, &value, I2C_DATA_LEN,
                           I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return -1;
    }
    return 0;
}

static int drv_acc_bosch_bma253_validate_id(i2c_dev_t *drv, uint8_t id_value)
{
    uint8_t value = 0x00;
    int     ret   = 0;

    if (unlikely(drv == NULL)) {
        return -1;
    }

    ret = sensor_i2c_read(drv, BMA253_CHIP_ID_ADDR, &value, I2C_DATA_LEN,
                          I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    if (unlikely(id_value != value)) {
        return -1;
    }

    return 0;
}

static int drv_acc_bosch_bma253_set_power_mode(i2c_dev_t *      drv,
                                               dev_power_mode_e mode)
{
    uint8_t value, value1 = 0x00;
    int     ret = 0;
    uint8_t buf[4];

    ret = sensor_i2c_read(drv, BMA253_MODE_CTRL_REG, &value, I2C_DATA_LEN,
                          I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    switch (mode) {
        case DEV_POWER_ON: {
            if ((value & BMA253_MODE_CTRL_MSK) == BMA253_NORMAL_MODE) {
                return 0;
            }
            value |= BMA253_NORMAL_MODE;
            ret = sensor_i2c_write(drv, BMA253_MODE_CTRL_REG, &value,
                                   I2C_DATA_LEN, I2C_OP_RETRIES);
            if (unlikely(ret)) {
                return ret;
            }
        } break;

        case DEV_POWER_OFF: {
            if ((value & BMA253_MODE_CTRL_MSK) == BMA253_DEEP_SUSPEND_MODE) {
                return 0;
            }

            value |= BMA253_DEEP_SUSPEND_MODE;
            ret = sensor_i2c_write(drv, BMA253_MODE_CTRL_REG, &value,
                                   I2C_DATA_LEN, I2C_OP_RETRIES);
            if (unlikely(ret)) {
                return ret;
            }
        } break;

        case DEV_SLEEP: {
            if ((value & BMA253_MODE_CTRL_MSK) == BMA253_SLEEP_MODE) {
                return 0;
            }

            value |= BMA253_SLEEP_MODE;
            ret = sensor_i2c_write(drv, BMA253_MODE_CTRL_REG, &value,
                                   I2C_DATA_LEN, I2C_OP_RETRIES);
            if (unlikely(ret)) {
                return ret;
            }

        } break;

        case DEV_SUSPEND: {
            if ((value & BMA253_MODE_CTRL_MSK) == BMA253_SUSPEND_MODE) {
                return 0;
            }

            value |= BMA253_SUSPEND_MODE;
            ret = sensor_i2c_write(drv, BMA253_MODE_CTRL_REG, &value,
                                   I2C_DATA_LEN, I2C_OP_RETRIES);
            if (unlikely(ret)) {
                return ret;
            }

        } break;

        default:
            break;
    }
    return 0;
}

static int drv_acc_bosch_bma253_set_odr(i2c_dev_t *drv, uint32_t odr)
{
    int      ret   = 0;
    uint8_t  value = 0x00;
    uint32_t bw    = odr / 2;

    ret = sensor_i2c_read(drv, BMA253_BW_SELECT_ADDR, &value, I2C_DATA_LEN,
                          I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    if (bw >= BMA253_BW_1000) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_1000HZ;
    } else if (bw >= BMA253_BW_500) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_500HZ;
    } else if (bw >= BMA253_BW_250) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_250HZ;
    } else if (bw >= BMA253_BW_125) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_125HZ;
    } else if (bw >= BMA253_BW_62_5) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_62_50HZ;
    } else if (bw >= BMA253_BW_31_25) {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_31_25HZ;
    } else {
        value &= BMA253_BW_BIT_MASK;
        value |= BMA253_BW_15_63HZ;
    }

    ret = sensor_i2c_write(drv, BMA253_BW_SELECT_ADDR, &value, I2C_DATA_LEN,
                           I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }
    return 0;
}

static int drv_acc_bosch_bma253_set_range(i2c_dev_t *drv, uint32_t range)
{
    int     ret   = 0;
    uint8_t value = 0x00;

    ret = sensor_i2c_read(drv, BMA253_RANGE_SELECT_REG, &value, I2C_DATA_LEN,
                          I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    switch (range) {
        case ACC_RANGE_2G: {
            value =
              BMA253_SET_BITSLICE(value, BMA253_RANGE_SELECT, BMA253_RANGE_2G);
        } break;

        case ACC_RANGE_4G: {
            value =
              BMA253_SET_BITSLICE(value, BMA253_RANGE_SELECT, BMA253_RANGE_4G);
        } break;

        case ACC_RANGE_8G: {
            value =
              BMA253_SET_BITSLICE(value, BMA253_RANGE_SELECT, BMA253_RANGE_8G);
        } break;

        case ACC_RANGE_16G: {
            value =
              BMA253_SET_BITSLICE(value, BMA253_RANGE_SELECT, BMA253_RANGE_16G);
        } break;

        default:
            break;
    }

    /* Write the range register 0x0F*/
    ret = sensor_i2c_write(drv, BMA253_RANGE_SELECT_REG, &value, I2C_DATA_LEN,
                           I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    if ((range >= ACC_RANGE_2G) && (range <= ACC_RANGE_16G)) {
        current_factor = bma253_factor[range];
    }

    return 0;
}


static int drv_acc_bosch_bma253_open(void)
{
    int ret = 0;
    if (bma253_work_mode == DEV_DATA_READY) {
        ret = drv_acc_bosch_bma253_data_ready_init();
        if (unlikely(ret)) {
            return -1;
        }
    } else if (bma253_work_mode == DEV_FIFO) {
        ret = drv_acc_bosch_bma253_fifo_init();
        if (unlikely(ret)) {
            return -1;
        }
    }

    ret = drv_acc_bosch_bma253_set_power_mode(&bma253_ctx, DEV_POWER_ON);
    if (unlikely(ret)) {
        return -1;
    }
    return 0;
}

static int drv_acc_bosch_bma253_close(void)
{
    int ret = 0;
    ret     = drv_acc_bosch_bma253_set_power_mode(&bma253_ctx, DEV_POWER_OFF);
    if (unlikely(ret)) {
        return -1;
    }
    return 0;
}

static int drv_acc_bosch_bma253_read_origion(void *buf, size_t len)
{
    int           ret = 0;
    size_t        size;
    uint8_t       reg[6];
    accel_data_t *accel = (accel_data_t *)buf;
    if (unlikely(buf == NULL)) {
        return -1;
    }

    size = sizeof(accel_data_t);
    if (unlikely(len < size)) {
        return -1;
    }

    ret = sensor_i2c_read(&bma253_ctx, BMA253_X_AXIS_LSB_ADDR, &reg[0],
                          I2C_REG_LEN, I2C_OP_RETRIES);
    ret |= sensor_i2c_read(&bma253_ctx, BMA253_X_AXIS_MSB_ADDR, &reg[1],
                           I2C_REG_LEN, I2C_OP_RETRIES);
    ret |= sensor_i2c_read(&bma253_ctx, BMA253_Y_AXIS_LSB_ADDR, &reg[2],
                           I2C_REG_LEN, I2C_OP_RETRIES);
    ret |= sensor_i2c_read(&bma253_ctx, BMA253_Y_AXIS_MSB_ADDR, &reg[3],
                           I2C_REG_LEN, I2C_OP_RETRIES);
    ret |= sensor_i2c_read(&bma253_ctx, BMA253_Z_AXIS_LSB_ADDR, &reg[4],
                           I2C_REG_LEN, I2C_OP_RETRIES);
    ret |= sensor_i2c_read(&bma253_ctx, BMA253_Z_AXIS_MSB_ADDR, &reg[5],
                           I2C_REG_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return -1;
    }

    accel->data[DATA_AXIS_X] =
      (int32_t)((((int32_t)((int8_t)reg[1])) << BMA253_SHIFT_EIGHT_BITS) |
                (reg[0] & BMA253_12_BIT_SHIFT));
    accel->data[DATA_AXIS_X] =
      accel->data[DATA_AXIS_X] >> BMA253_SHIFT_FOUR_BITS;

    accel->data[DATA_AXIS_Y] =
      (int32_t)((((int32_t)((int8_t)reg[3])) << BMA253_SHIFT_EIGHT_BITS) |
                (reg[2] & BMA253_12_BIT_SHIFT));
    accel->data[DATA_AXIS_Y] =
      accel->data[DATA_AXIS_Y] >> BMA253_SHIFT_FOUR_BITS;

    accel->data[DATA_AXIS_Z] =
      (int32_t)((((int32_t)((int8_t)reg[5])) << BMA253_SHIFT_EIGHT_BITS) |
                (reg[4] & BMA253_12_BIT_SHIFT));
    accel->data[DATA_AXIS_Z] =
      accel->data[DATA_AXIS_Z] >> BMA253_SHIFT_FOUR_BITS;

    if (current_factor != 0) {
        // the unit of acc is mg, 1000 mg = 1 g.
        accel->data[DATA_AXIS_X] = accel->data[DATA_AXIS_X] *
                                   ACCELEROMETER_UNIT_FACTOR /
                                   (int32_t)current_factor;
        accel->data[DATA_AXIS_Y] = accel->data[DATA_AXIS_Y] *
                                   ACCELEROMETER_UNIT_FACTOR /
                                   (int32_t)current_factor;
        accel->data[DATA_AXIS_Z] = accel->data[DATA_AXIS_Z] *
                                   ACCELEROMETER_UNIT_FACTOR /
                                   (int32_t)current_factor;
    }
    accel->timestamp = aos_now_ms();

    return (int)size;
}
static int drv_acc_bosch_bma253_read_fifo(accel_data_t *accel, size_t len)
{
    int i;
    int ret;
    int num = len / sizeof(accel_data_t);

    if (unlikely(num != bma253_fifo_wml)) {
        return -1;
    }

    ret = sensor_i2c_read(
      &bma253_ctx, BMA253_FIFO_DATA_OUTPUT_ADDR, &bma253_fifo_data[0],
      BMA253_FIFO_DATA_NUM * bma253_fifo_wml, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return -1;
    }

    for (i = 0; i < num; i++) {
        accel[i].data[DATA_AXIS_X] =
          (int32_t)((((int32_t)((int8_t)bma253_fifo_data[6 * i + 1]))
                     << BMA253_SHIFT_EIGHT_BITS) |
                    (bma253_fifo_data[6 * i + 0] & BMA253_12_BIT_SHIFT));
        accel[i].data[DATA_AXIS_X] =
          accel[i].data[DATA_AXIS_X] >> BMA253_SHIFT_FOUR_BITS;
        accel[i].data[DATA_AXIS_Y] =
          (int32_t)((((int32_t)((int8_t)bma253_fifo_data[6 * i + 3]))
                     << BMA253_SHIFT_EIGHT_BITS) |
                    (bma253_fifo_data[6 * i + 2] & BMA253_12_BIT_SHIFT));
        accel[i].data[DATA_AXIS_Y] =
          accel[i].data[DATA_AXIS_Y] >> BMA253_SHIFT_FOUR_BITS;
        accel[i].data[DATA_AXIS_Z] =
          (int32_t)((((int32_t)((int8_t)bma253_fifo_data[6 * i + 5]))
                     << BMA253_SHIFT_EIGHT_BITS) |
                    (bma253_fifo_data[6 * i + 4] & BMA253_12_BIT_SHIFT));
        accel[i].data[DATA_AXIS_Z] =
          accel[i].data[DATA_AXIS_Z] >> BMA253_SHIFT_FOUR_BITS;
        if (current_factor != 0) {
            accel[i].data[DATA_AXIS_X] = accel[i].data[DATA_AXIS_X] *
                                         ACCELEROMETER_UNIT_FACTOR /
                                         (int32_t)current_factor;
            accel[i].data[DATA_AXIS_Y] = accel[i].data[DATA_AXIS_Y] *
                                         ACCELEROMETER_UNIT_FACTOR /
                                         (int32_t)current_factor;
            accel[i].data[DATA_AXIS_Z] = accel[i].data[DATA_AXIS_Z] *
                                         ACCELEROMETER_UNIT_FACTOR /
                                         (int32_t)current_factor;
        }
        accel[i].timestamp = aos_now_ms();
    }

    return len;
}


static int drv_acc_bosch_bma253_workmode_set(work_mode_e mode)
{
    int ret = 0;

    if (unlikely(mode >= DEV_MODE_INV)) {
        return -1;
    }

    bma253_work_mode = mode;

    return 0;
}

static work_mode_e drv_acc_bosch_bma253_workmode_get(void)
{
    return bma253_work_mode;
}

static int drv_acc_bosch_bma253_data_ready_init(void)
{
    int     ret;
    uint8_t value;
    value = BMA253_IRQ_DATA_READY_ENABLE;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_ENABLE2_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_MAP_DATA_INT2;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_DATA_SELECT_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_CONFIG_PUSH_PULL;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_SET_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }


    value = BMA253_IRQ_CLEAR_VAL | BMA253_IRQ_NON_LATCHED;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_CTRL_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }
    return 0;
}

static int drv_acc_bosch_bma253_fifo_init(void)
{
    int     ret;
    uint8_t value;

    value = BMA253_IRQ_FIFO_WML_MODE;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_FIFO_MODE_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_FIFO_WML_ENABLE;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_ENABLE2_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = bma253_fifo_wml;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_FIFO_WML_TRIG, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_MAP_FIFO_WML_INT2;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_DATA_SELECT_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_CONFIG_PUSH_PULL;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_SET_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    value = BMA253_IRQ_CLEAR_VAL | BMA253_IRQ_NON_LATCHED;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_CTRL_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return ret;
    }

    return 0;
}
static int drv_acc_bosch_bma253_read(void *buf, size_t len)
{
    int ret = 0;

    if (0 == bma253_fifo_wml) {
        return drv_acc_bosch_bma253_read_origion(buf, len);
    } else {

        return drv_acc_bosch_bma253_read_fifo(buf, len);
    }
}


static void drv_acc_bosch_bma253_irq_clear(void)
{
    uint8_t value;
    int     ret;

    value = BMA253_IRQ_CLEAR_VAL | BMA253_IRQ_NON_LATCHED;
    ret   = sensor_i2c_write(&bma253_ctx, BMA253_INTR_CTRL_ADDR, &value,
                           I2C_DATA_LEN, I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return;
    }
}

static work_mode_e drv_acc_bosch_bma253_irq_mode_active(void)
{
    uint8_t value;
    int     ret;

    ret = sensor_i2c_read(&bma253_ctx, BMA253_STAT2_ADDR, &value, I2C_DATA_LEN,
                          I2C_OP_RETRIES);
    if (unlikely(ret)) {
        return DEV_MODE_INV;
    }

    if ((value & BMA253_IRQ_FIFO_STAT) == BMA253_IRQ_FIFO_STAT) {
        return DEV_FIFO;
    }

    else if ((value & BMA253_IRQ_DATA_READY_STAT) ==
             BMA253_IRQ_DATA_READY_STAT) {
        return DEV_DATA_READY;
    } else {
        return DEV_INT;
    }
}


static void drv_acc_bosch_bma253_irq_handle(void)
{
    drv_acc_bosch_bma253_irq_clear();
}

static int drv_acc_bosch_bma253_ioctl(int cmd, unsigned long arg)
{
    int ret = 0;

    switch (cmd) {
        case SENSOR_IOCTL_ODR_SET: {
            ret = drv_acc_bosch_bma253_set_odr(&bma253_ctx, arg);
            if (unlikely(ret)) {
                return -1;
            }
        } break;
        case SENSOR_IOCTL_RANGE_SET: {
            ret = drv_acc_bosch_bma253_set_range(&bma253_ctx, arg);
            if (unlikely(ret)) {
                return -1;
            }
        } break;
        case SENSOR_IOCTL_SET_POWER: {
            ret = drv_acc_bosch_bma253_set_power_mode(&bma253_ctx, arg);
            if (unlikely(ret)) {
                return -1;
            }
        } break;
        case SENSOR_IOCTL_GET_INFO: {
            /* fill the dev info here */
            dev_sensor_info_t *info = (dev_sensor_info_t *)arg;
            info->model             = "BMA253";
            info->range_max         = 16;
            info->range_min         = 2;
            info->unit              = mg;
        } break;

        default:
            break;
    }

    return 0;
}

int drv_acc_bosch_bma253_init(void)
{
    int          ret = 0;
    int          num;
    sensor_obj_t sensor;

    memset(&sensor, 0, sizeof(sensor));

    /* fill the sensor obj parameters here */
    sensor.io_port     = I2C_PORT;
    sensor.tag         = TAG_DEV_ACC;
    sensor.path        = dev_acc_path;
    sensor.open        = drv_acc_bosch_bma253_open;
    sensor.close       = drv_acc_bosch_bma253_close;
    sensor.read        = drv_acc_bosch_bma253_read;
    sensor.write       = NULL;
    sensor.ioctl       = drv_acc_bosch_bma253_ioctl;
    sensor.irq_handle  = drv_acc_bosch_bma253_irq_handle;
    sensor.mode        = DEV_FIFO;
    sensor.gpio.port   = BMA253_IRQ_PIN;
    sensor.gpio.config = IRQ_MODE;
    sensor.gpio.priv   = &bma253_irq_mode_rising;
    if (DEV_FIFO == sensor.mode) {
        ret = drv_acc_bosch_bma253_fifo_wml_set(BMA253_FIFO_DEPTH_USD);
        if (unlikely(ret)) {
            return -1;
        }

        sensor.data_len = bma253_fifo_wml * sizeof(accel_data_t);
    } else {
        sensor.data_len = sizeof(accel_data_t);
    }

    ret = drv_acc_bosch_bma253_workmode_set(sensor.mode);
    if (unlikely(ret)) {
        return -1;
    }


    ret = sensor_create_obj(&sensor);
    if (unlikely(ret)) {
        return -1;
    }

    ret = drv_acc_bosch_bma253_validate_id(&bma253_ctx, BMA253_CHIP_ID_VALUE);
    if (unlikely(ret)) {
        return -1;
    }

    ret = drv_acc_bosch_bma253_soft_reset(&bma253_ctx);
    if (unlikely(ret)) {
        return -1;
    }

    aos_msleep(5);
    ret = drv_acc_bosch_bma253_set_range(&bma253_ctx, ACC_RANGE_8G);
    if (unlikely(ret)) {
        return -1;
    }

    // set odr is 100hz, and will update
    ret = drv_acc_bosch_bma253_set_odr(&bma253_ctx, BMA253_DEFAULT_ODR_100HZ);
    if (unlikely(ret)) {
        return -1;
    }
    ret = drv_acc_bosch_bma253_set_power_mode(&bma253_ctx, DEV_SLEEP);
    if (unlikely(ret)) {
        return -1;
    }

    /* update the phy sensor info to sensor hal */
    LOG("%s %s successfully \n", SENSOR_STR, __func__);
    return 0;
}
