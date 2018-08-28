#ifndef _SENSOR_MODBUS_H_
#define _SENSOR_MODBUS_H_

/* public interface */
int modbus_init(void);
int modbus_read(unsigned char *buff, unsigned char slave,
	unsigned short addr, unsigned short count, unsigned short response_time);


#endif /* _SENSOR_MODBUS_H_ */
