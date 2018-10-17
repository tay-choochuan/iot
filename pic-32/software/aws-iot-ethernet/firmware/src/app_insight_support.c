
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "app_insight_support.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

int APP_ParseCommand(char * pReadBuffer)
{
    if(pReadBuffer == NULL)
        return COMMAND_BAD_POINTER;
    JSON_Value *rootValue = json_parse_string(pReadBuffer);
    if (json_value_get_type(rootValue) != JSONObject)
            return COMMAND_BAD_JSON;
    JSON_Object * tObject = json_value_get_object(rootValue);

    if(json_object_dotget_string(tObject, "message.command") != NULL)
    {
        if(strcmp(json_object_dotget_string(tObject, "message.command"), "hello") == 0)
        {
            return COMMAND_HELLO;
        }
        else if (strcmp(json_object_dotget_string(tObject, "message.command"), "configuration") == 0)
        {
            return COMMAND_CONFIGURE;
        }
        else if (strcmp(json_object_dotget_string(tObject, "message.command"), "debug_set") == 0)
        {
            return COMMAND_DEBUG_SET;
        }
        else
        {
            return COMMAND_INVALID;
        }
    }
    else
    {
        return COMMAND_INVALID;
    }
    return COMMAND_INVALID;
}

void BuildAckNackMessage(JSON_Object *rootObject, char * command, char * msg)
{
    json_object_dotset_string(rootObject, "message.command", command);
    json_object_dotset_string(rootObject, "message.ack_nack_message", msg);
}

int APP_CheckForMessage(char * pMessage)
{
    if(pMessage == NULL)
        return MESSAGE_BAD_POINTER;
    if(strchr(pMessage, DELIMITER))
    {
        return MESSAGE_DELIMITER;
    }
    else
    {
        return MESSAGE_NO_DELIMITER;
    }

}

void APP_BuildDebugString(char * messageBuffer, int sizeMessageBuffer, int command)
{
    if(messageBuffer == NULL)
        return;


}

const char* APP_ReturnDebugCodeToString(int debugCommand)
{
    switch(debugCommand)
    {
        case DEBUG_UPDATE:
            return "Debug update";
        case DEBUG_NVM_WRITE_SUCCESS:
            return "Configuration successfully written to NVM";
        case DEBUG_NVM_WRITE_FAILED:
            return "Configuration failed to write to NVM, reset and try again";
        case DEBUG_NO_IP_ADDRESS:
            return "Not getting an IP address, check connection";
        case DEBUG_SOCKET_CHANGE:
            return "Change socket state";
        case DEBUG_MQTT_CHANGE:
            return "Change mqtt state";
        case DEBUG_GOT_IP_ADDRESS:
            return "Got IP address";
        case DEBUG_ERROR_CLOSING_SOCKET:
            return "Error, closing socket and retrying";
        case DEBUG_FATAL_ERROR:
            return "Fatal Error, requires board reset";

    }
    return "Unknown debug command";
}

/* *****************************************************************************
 End of File
 */
