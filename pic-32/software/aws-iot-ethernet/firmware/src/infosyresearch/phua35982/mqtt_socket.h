
#ifndef WOLFMQTT_SOCKET_H
#define WOLFMQTT_SOCKET_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Options */
/* Use without WolfSSL (on by default) */
//#define ENABLE_MQTT_TLS

#include "wolfmqtt/mqtt_types.h"
#ifdef ENABLE_MQTT_TLS
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#endif

/* Default Port Numbers */
#define MQTT_DEFAULT_PORT   1883
#define MQTT_SECURE_PORT    8883


struct _MqttClient;

/* Function callbacks */
typedef int (*MqttTlsCb)(struct _MqttClient* client);

typedef int (*MqttNetConnectCb)(void *context,
    const char* host, word16 port, int timeout_ms);
typedef int (*MqttNetWriteCb)(void *context,
    const byte* buf, int buf_len, int timeout_ms);
typedef int (*MqttNetReadCb)(void *context,
    byte* buf, int buf_len, int timeout_ms);
typedef int (*MqttNetDisconnectCb)(void *context);

/* Strucutre for Network Security */
#ifdef ENABLE_MQTT_TLS
typedef struct _MqttTls {
    WOLFSSL_CTX         *ctx;
    WOLFSSL             *ssl;
} MqttTls;
#endif

/* Structure for Network callbacks */
typedef struct _MqttNet {
    void                *context;
    MqttNetConnectCb    connect;
    MqttNetReadCb       read;
    MqttNetWriteCb      write;
    MqttNetDisconnectCb disconnect;
} MqttNet;


/* MQTT SOCKET APPLICATION INTERFACE */
int MqttSocket_Init(struct _MqttClient *client, MqttNet* net);
int MqttSocket_Write(struct _MqttClient *client, const byte* buf, int buf_len,
    int timeout_ms);
int MqttSocket_Read(struct _MqttClient *client, byte* buf, int buf_len,
    int timeout_ms);

int MqttSocket_Connect(struct _MqttClient *client, const char* host,
    word16 port, int timeout_ms, int use_tls, MqttTlsCb cb);
int MqttSocket_Disconnect(struct _MqttClient *client);


#ifdef __cplusplus
    } /* extern "C" */
#endif

#endif /* WOLFMQTT_SOCKET_H */
