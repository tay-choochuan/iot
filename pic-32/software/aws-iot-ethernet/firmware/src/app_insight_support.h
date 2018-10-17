
#include "parser.h"

#ifndef _APP_INSIGHT_SUPPORT_H    /* Guard against multiple inclusion */
#define _APP_INSIGHT_SUPPORT_H

#define DELIMITER '\r'

enum ErrorCodes
{
    MESSAGE_DELIMITER,
    MESSAGE_NO_DELIMITER,
    MESSAGE_BAD_POINTER
};

enum CommandCodes
{
    COMMAND_BAD_POINTER,
    COMMAND_BAD_JSON,
    COMMAND_INVALID,
    COMMAND_HELLO,
    COMMAND_CONFIGURE,
    COMMAND_WIFI_CONFIGURE,
    COMMAND_DEBUG_MSG,
    COMMAND_DISCOVERY,
    COMMAND_ACK,
    COMMAND_NACK,
    COMMAND_DEBUG_SET
};

enum DebugCodes
{
    DEBUG_UPDATE,
    DEBUG_NVM_WRITE_FAILED,
    DEBUG_NVM_WRITE_SUCCESS,
    DEBUG_NO_IP_ADDRESS,
    DEBUG_SOCKET_CHANGE,
    DEBUG_MQTT_CHANGE,
    DEBUG_GOT_IP_ADDRESS,
    DEBUG_ERROR_CLOSING_SOCKET,
    DEBUG_FATAL_ERROR
};

int APP_ParseCommand(char * pReadBuffer);
int APP_CheckForMessage(char * pMessage);
void BuildAckNackMessage(JSON_Object *rootObject, char * command, char * msg);
const char* APP_ReturnDebugCodeToString(int return_code);

#endif /* _APP_INSIGHT_SUPPORT_H */

/* *****************************************************************************
 End of File
 */
