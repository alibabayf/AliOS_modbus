/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdint.h>
#include "iot_import_dtls.h"

#ifndef COAP_TRANSPORT_H__
#define COAP_TRANSPORT_H__

typedef enum {
    COAP_ENDPOINT_NOSEC = 0,
    COAP_ENDPOINT_DTLS,
} coap_endpoint_type;


typedef struct {
    DTLSContext         *context;
    int                  socket_id;
} coap_remote_session_t;


typedef struct {
    int                      socket_id;
    coap_endpoint_type       ep_type;
    coap_address_t           remote_endpoint;
    coap_remote_session_t    remote_session;
} coap_network_t;


typedef struct {
    coap_endpoint_type       ep_type;
    coap_address_t           remote;
    unsigned char           *p_ca_cert_pem;
    char                    *p_host;
} coap_network_init_t;


unsigned int CoAPNetwork_init (const coap_network_init_t    *p_param, coap_network_t *p_network);


unsigned int CoAPNetwork_write(coap_network_t *p_network,
                               const unsigned char   *p_data,
                               unsigned int           datalen);

int CoAPNetwork_read(coap_network_t *network, unsigned char  *data,
                     unsigned int datalen, unsigned int timeout);

unsigned int CoAPNetwork_deinit(coap_network_t *p_network);


#endif

