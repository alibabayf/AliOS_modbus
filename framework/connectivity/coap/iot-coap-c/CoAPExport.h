/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include "aos/aos.h"
#include "CoAPNetwork.h"
#include "lite-utils.h"

#ifndef __COAP_EXPORT_H__
#define __COAP_EXPORT_H__

//#define COAP_DTLS_SUPPORT


#define COAP_MSG_MAX_TOKEN_LEN    12
#define COAP_MSG_MAX_OPTION_NUM   12
#define COAP_MSG_MAX_PATH_LEN     32
#define COAP_MSG_MAX_PDU_LEN      1280

/*CoAP Content Type*/
#define COAP_CT_TEXT_PLAIN                 0   /* text/plain (UTF-8) */
#define COAP_CT_APP_LINK_FORMAT           40   /* application/link-format */
#define COAP_CT_APP_XML                   41   /* application/xml */
#define COAP_CT_APP_OCTET_STREAM          42   /* application/octet-stream */
#define COAP_CT_APP_RDF_XML               43   /* application/rdf+xml */
#define COAP_CT_APP_EXI                   47   /* application/exi  */
#define COAP_CT_APP_JSON                  50   /* application/json  */
#define COAP_CT_APP_CBOR                  60   /* application/cbor  */

/*CoAP option types. */
#define COAP_OPTION_IF_MATCH        1   /* C, opaque,  0-8 B, (none) */
#define COAP_OPTION_URI_HOST        3   /* C, String,  1-255 B, destination address */
#define COAP_OPTION_ETAG            4   /* E, opaque,  1-8 B, (none) */
#define COAP_OPTION_IF_NONE_MATCH   5   /* empty,      0 B, (none) */
#define COAP_OPTION_URI_PORT        7   /* C, uint,    0-2 B, destination port */
#define COAP_OPTION_LOCATION_PATH   8   /* E, String,  0-255 B, - */
#define COAP_OPTION_URI_PATH       11   /* C, String,  0-255 B, (none) */
#define COAP_OPTION_CONTENT_FORMAT 12   /* E, uint,      0-2 B, (none) */
#define COAP_OPTION_MAXAGE         14   /* E, uint,    0--4 B, 60 Seconds */
#define COAP_OPTION_URI_QUERY      15   /* C, String,  1-255 B, (none) */
#define COAP_OPTION_ACCEPT         17   /* C, uint,    0-2 B, (none) */
#define COAP_OPTION_LOCATION_QUERY 20   /* E, String,      0-255 B, (none) */
#define COAP_OPTION_BLOCK2         23   /* C, uint,    0--3 B, (none) */
#define COAP_OPTION_BLOCK1         27   /* C, uint,    0--3 B, (none) */
#define COAP_OPTION_PROXY_URI      35   /* C, String,  1-1024 B, (none) */
#define COAP_OPTION_PROXY_SCHEME   39   /* C, String,  1-255 B, (none) */
#define COAP_OPTION_SIZE1          60   /* E, uint,    0-4 B, (none) */
#define COAP_OPTION_AUTH_TOKEN     61   /* C, String,  1-255B, (none)*/


/*CoAP Message types*/
#define COAP_MESSAGE_TYPE_CON   0
#define COAP_MESSAGE_TYPE_NON   1
#define COAP_MESSAGE_TYPE_ACK   2
#define COAP_MESSAGE_TYPE_RST   3

/* CoAP module error code base */
#define COAP_ERROR_BASE            (1<<8)
#define COAP_ERROR_MSG_BASE        (1<<16)
#define COAP_ERROR_DTLS_BASE       (1<<24)

/* CoAP base error code */
#define COAP_SUCCESS                           (0)                    /*  Successful */
#define COAP_ERROR_INTERNAL                    (COAP_ERROR_BASE | 1)  /*  Internal Error */
#define COAP_ERROR_INVALID_PARAM               (COAP_ERROR_BASE | 2)  /*  Invalid Parameter */
#define COAP_ERROR_INVALID_LENGTH              (COAP_ERROR_BASE | 3)  /*  Invalid Length */
#define COAP_ERROR_INVALID_DATA                (COAP_ERROR_BASE | 4)  /*  Invalid Data */
#define COAP_ERROR_DATA_SIZE                   (COAP_ERROR_BASE | 5)  /*  Data size exceeds limit */
#define COAP_ERROR_NULL                        (COAP_ERROR_BASE | 6)  /*  Null Pointer */
#define COAP_ERROR_INVALID_URI                 (COAP_ERROR_BASE | 7)
#define COAP_ERROR_DNS_FAILED                  (COAP_ERROR_BASE | 8)
#define COAP_ERROR_NOT_FOUND                   (COAP_ERROR_BASE | 9)

/* CoAP Message and Transmission error code*/
#define COAP_MESSAGE_ERROR_NULL            (COAP_ERROR_MSG_BASE | 0)
#define COAP_MESSAGE_ERROR_INVALID_CONF    (COAP_ERROR_MSG_BASE | 1)
#define COAP_MESSAGE_INVALID_CONTENT       (COAP_ERROR_MSG_BASE | 2)
#define COAP_TRANSMISSION_RESET_BY_PEER    (COAP_ERROR_MSG_BASE | 3)
#define COAP_TRANSMISSION_TIMEOUT          (COAP_ERROR_MSG_BASE | 4)
#define COAP_TRANSPORT_SECURITY_MISSING    (COAP_ERROR_MSG_BASE | 5)


/* CoAP DTLS error code */
#define COAP_DTLS_NEW_SESSION_REQ           (COAP_ERROR_DTLS_BASE | 0)
#define COAP_DTLS_CONFIGURATION_FAILED      (COAP_ERROR_DTLS_BASE | 1)
#define COAP_DTLS_CONTEXT_SETUP_FAILED      (COAP_ERROR_DTLS_BASE | 2)
#define COAP_DTLS_HANDSHAKE_IN_PROGRESS     (COAP_ERROR_DTLS_BASE | 3)
#define COAP_DTLS_READ_DATA_FAILED          (COAP_ERROR_DTLS_BASE | 4)
#define COAP_DTLS_INVALID_CA_CERTIFICATE    (COAP_ERROR_DTLS_BASE | 5)
#define COAP_DTLS_OWN_CERT_SETUP_FAILED     (COAP_ERROR_DTLS_BASE | 6)
#define COAP_DTLS_FATAL_ALERT_MESSAGE       (COAP_ERROR_DTLS_BASE | 7)
#define COAP_DTLS_PEER_CLOSE_NOTIFY         (COAP_ERROR_DTLS_BASE | 8)
#define COAP_DTLS_HANDSHAKE_FAILED          (COAP_ERROR_DTLS_BASE | 9)


#define COAP_MSG_CODE_DEF(N) (((N)/100 << 5) | (N)%100)

/*CoAP Message codes*/
typedef enum {
    /* CoAP Empty Message */
    COAP_MSG_CODE_EMPTY_MESSAGE                  = COAP_MSG_CODE_DEF(0),  /* Mapping to CoAP code 0.00 */

    /* CoAP Method Codes */
    COAP_MSG_CODE_GET                            = COAP_MSG_CODE_DEF(1),  /* CoAP Get method */
    COAP_MSG_CODE_POST                           = COAP_MSG_CODE_DEF(2),  /* CoAP Post method */
    COAP_MSG_CODE_PUT                            = COAP_MSG_CODE_DEF(3),  /* CoAP Put method */
    COAP_MSG_CODE_DELETE                         = COAP_MSG_CODE_DEF(4),  /* CoAP Delete method */

    /* CoAP Success Response Codes */
    COAP_MSG_CODE_201_CREATED                    = COAP_MSG_CODE_DEF(201),  /* Mapping to CoAP code 2.01, Hex:0x41, Created */
    COAP_MSG_CODE_202_DELETED                    = COAP_MSG_CODE_DEF(202),  /* Mapping to CoAP code 2.02, Hex:0x42, Deleted*/
    COAP_MSG_CODE_203_VALID                      = COAP_MSG_CODE_DEF(203),  /* Mapping to CoAP code 2.03, Hex:0x43, Valid*/
    COAP_MSG_CODE_204_CHANGED                    = COAP_MSG_CODE_DEF(204),  /* Mapping to CoAP code 2.04, Hex:0x44, Changed*/
    COAP_MSG_CODE_205_CONTENT                    = COAP_MSG_CODE_DEF(205),  /* Mapping to CoAP code 2.05, Hex:0x45, Content*/
    COAP_MSG_CODE_231_CONTINUE                   = COAP_MSG_CODE_DEF(231),  /* Mapping to CoAP code 2.31, Hex:0x5F, Continue*/

    /* CoAP Client Error Response Codes */
    COAP_MSG_CODE_400_BAD_REQUEST                = COAP_MSG_CODE_DEF(400),  /* Mapping to CoAP code 4.00, Hex:0x80, Bad Request */
    COAP_MSG_CODE_401_UNAUTHORIZED               = COAP_MSG_CODE_DEF(401),  /* Mapping to CoAP code 4.01, Hex:0x81, Unauthorized */
    COAP_MSG_CODE_402_BAD_OPTION                 = COAP_MSG_CODE_DEF(402),  /* Mapping to CoAP code 4.02, Hex:0x82, Bad Option */
    COAP_MSG_CODE_403_FORBIDDEN                  = COAP_MSG_CODE_DEF(403),  /* Mapping to CoAP code 4.03, Hex:0x83, Forbidden */
    COAP_MSG_CODE_404_NOT_FOUND                  = COAP_MSG_CODE_DEF(404),  /* Mapping to CoAP code 4.04, Hex:0x84, Not Found */
    COAP_MSG_CODE_405_METHOD_NOT_ALLOWED         = COAP_MSG_CODE_DEF(405),  /* Mapping to CoAP code 4.05, Hex:0x85, Method Not Allowed */
    COAP_MSG_CODE_406_NOT_ACCEPTABLE             = COAP_MSG_CODE_DEF(406),  /* Mapping to CoAP code 4.06, Hex:0x86, Not Acceptable */
    COAP_MSG_CODE_408_REQUEST_ENTITY_INCOMPLETE  = COAP_MSG_CODE_DEF(408),  /* Mapping to CoAP code 4.08, Hex:0x88, Request Entity Incomplete */
    COAP_MSG_CODE_412_PRECONDITION_FAILED        = COAP_MSG_CODE_DEF(412),  /* Mapping to CoAP code 4.12, Hex:0x8C, Precondition Failed */
    COAP_MSG_CODE_413_REQUEST_ENTITY_TOO_LARGE   = COAP_MSG_CODE_DEF(413),  /* Mapping to CoAP code 4.13, Hex:0x8D, Request Entity Too Large */
    COAP_MSG_CODE_415_UNSUPPORTED_CONTENT_FORMAT = COAP_MSG_CODE_DEF(415),  /* Mapping to CoAP code 4.15, Hex:0x8F, Unsupported Content-Format */

    /* CoAP Server Error Response Codes */
    COAP_MSG_CODE_500_INTERNAL_SERVER_ERROR      = COAP_MSG_CODE_DEF(500),  /* Mapping to CoAP code 5.00, Hex:0xA0, Internal Server Error */
    COAP_MSG_CODE_501_NOT_IMPLEMENTED            = COAP_MSG_CODE_DEF(501),  /* Mapping to CoAP code 5.01, Hex:0xA1, Not Implemented */
    COAP_MSG_CODE_502_BAD_GATEWAY                = COAP_MSG_CODE_DEF(502),  /* Mapping to CoAP code 5.02, Hex:0xA2, Bad Gateway */
    COAP_MSG_CODE_503_SERVICE_UNAVAILABLE        = COAP_MSG_CODE_DEF(503),  /* Mapping to CoAP code 5.03, Hex:0xA3, Service Unavailable */
    COAP_MSG_CODE_504_GATEWAY_TIMEOUT            = COAP_MSG_CODE_DEF(504),  /* Mapping to CoAP code 5.04, Hex:0xA4, Gateway Timeout */
    COAP_MSG_CODE_505_PROXYING_NOT_SUPPORTED     = COAP_MSG_CODE_DEF(505)   /* Mapping to CoAP code 5.05, Hex:0xA5, Proxying Not Supported */

} CoAPMessageCode;


typedef struct {
    unsigned char                  version   : 2;
    unsigned char                  type      : 2;
    unsigned char                  tokenlen  : 4;
    unsigned char                  code;
    unsigned short                 msgid;
} CoAPMsgHeader;


typedef struct {
    unsigned short num;
    unsigned short len;
    unsigned char *val;
} CoAPMsgOption;

typedef void (*CoAPRespMsgHandler)(void *data, void *message);

typedef void (*CoAPEventNotifier)(unsigned int event, void *p_message);

typedef struct {
    void                    *user;
    unsigned short           msgid;
    char                     acked;
    unsigned char            tokenlen;
    unsigned char            token[8];
    unsigned char            retrans_count;
    unsigned short           timeout;
    unsigned short           timeout_val;
    unsigned char           *message;
    unsigned int             msglen;
    CoAPRespMsgHandler       handler;
    struct list_head         sendlist;
} CoAPSendNode;

typedef struct {
    unsigned char            count;
    unsigned char            maxcount;
    struct list_head         sendlist;
} CoAPSendList;


typedef struct {
    CoAPMsgHeader   header;
    unsigned char   token[COAP_MSG_MAX_TOKEN_LEN];
    CoAPMsgOption   options[COAP_MSG_MAX_OPTION_NUM];
    unsigned char   optnum;
    unsigned char   optdelta;
    unsigned char  *payload;
    unsigned short  payloadlen;
    CoAPRespMsgHandler handler;
    void           *user;
} CoAPMessage;

typedef struct {
    char *url;
    unsigned char  maxcount;  /*list maximal count*/
} CoAPInitParam;

typedef struct {
    unsigned short           message_id;
    coap_network_t           network;
    CoAPEventNotifier        notifier;
    unsigned char            *sendbuf;
    unsigned char            *recvbuf;
    CoAPSendList             list;
} CoAPContext;
/*
#define coap_log_print(level, ...) \
    {\
    fprintf(stderr, "%s [%s #%d]   ",level, __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__);\
    }
*/

// #define COAP_TRC(fmt,  args...)  coap_log_print("<TRACE>", fmt, ##args)
// #define COAP_DUMP(fmt, args...)  coap_log_print("<DUMP> ", fmt, ##args)
// #define COAP_DEBUG(fmt,args...)  coap_log_print("<DEBUG>", fmt, ##args)
// #define COAP_INFO(fmt, args...)  coap_log_print("<INFO> ", fmt, ##args)
// #define COAP_ERR(fmt,  args...)  coap_log_print("<ERROR>", fmt, ##args)

#define COAP_TRC(fmt,  args...)  LOGD("CoAP", fmt,##args)
#define COAP_DEBUG(fmt,args...)  LOGD("CoAP", fmt,##args)
#define COAP_INFO(fmt, args...)  LOGI("CoAP", fmt,##args)
#define COAP_ERR(fmt, args...)   LOGE("CoAP", fmt,##args)

CoAPContext *CoAPContext_create(CoAPInitParam *param);
void CoAPContext_free(CoAPContext *p_ctx);


#endif
