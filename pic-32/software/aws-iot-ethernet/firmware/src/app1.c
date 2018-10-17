
#include "app1.h"
#include "peripheral/cmp/plib_cmp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
struct switchMessage mySwitchMessage;
#define APP_LOW_VOLTAGE 630
#define APP_GOOD_VOLTAGE 650

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP1_DATA app1Data;
extern APP_DATA appData;
extern BSP_DATA bspData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app1Data.state = APP1_STATE_INIT;
    BSP_Initialize();
    BSP_LED_LightShowSet(BSP_LED_EASY_CONFIGURATION);
    app1Data.newPotSamp = 0;
    app1Data.potTimer = 0;
    app1Data.newVoltageSamp = 0;
    app1Data.currIsLVD = true;

    // Queue for switch data
    app1Data.switchQueue = xQueueCreate( 20, sizeof(mySwitchMessage) );
    if(app1Data.switchQueue == NULL) {
        ; // Handle this
    }
    // Queue for LED light show data
    // This will hold the requested Light Show state only
    app1Data.lightShowQueue = xQueueCreate( 1, 4 );
    if(app1Data.lightShowQueue == NULL) {
        ; // Handle this
    }
    // Queue for potentiometer data
    // This will hold the latest potentiometer data
    app1Data.potentiometerQueue = xQueueCreate( 20, sizeof(app1Data.potValue) );
    if(app1Data.potentiometerQueue == NULL) {
        ; // Handle this
    }
    xQueueReset(app1Data.switchQueue);
    xQueueReset(app1Data.lightShowQueue);
    xQueueReset(app1Data.potentiometerQueue);
}


/******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( app1Data.state )
    {
        /* Application's initial state. */
        case APP1_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {
                // Open the ADC drivers
                DRV_ADC0_Open();
               // DRV_ADC1_Open();
                DRV_ADC_DigitalFilter0_Open();
               // DRV_ADC_DigitalFilter1_Open();
                DRV_ADC_Start();
                app1Data.state = APP1_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP1_STATE_SERVICE_TASKS:
        {
            // BSP tasks that control switch and led functions
            BSP_SYS_Tasks();

            // Check if switches are pressed and send a message to the queue
            if(BSP_SWITCH_SwitchGetState(BSP_SWITCH_1_PORT) != bspData.previousStateS1){
                BSP_SWITCH_SwitchSetPreviousState(BSP_SWITCH_1_PORT, BSP_SWITCH_SwitchGetState(BSP_SWITCH_1_PORT));
                mySwitchMessage.switchNum = BSP_SWITCH_1;
                mySwitchMessage.switchVal = bspData.previousStateS1;
                xQueueSendToBack( app1Data.switchQueue, &mySwitchMessage, 1 );
            }
            if(BSP_SWITCH_SwitchGetState(BSP_SWITCH_2_PORT) != bspData.previousStateS2){
                BSP_SWITCH_SwitchSetPreviousState(BSP_SWITCH_2_PORT, BSP_SWITCH_SwitchGetState(BSP_SWITCH_2_PORT));
                mySwitchMessage.switchNum = BSP_SWITCH_2;
                mySwitchMessage.switchVal = bspData.previousStateS2;
                xQueueSendToBack( app1Data.switchQueue, &mySwitchMessage, 1 );
            }
            if(BSP_SWITCH_SwitchGetState(BSP_SWITCH_3_PORT) != bspData.previousStateS3){
                BSP_SWITCH_SwitchSetPreviousState(BSP_SWITCH_3_PORT, BSP_SWITCH_SwitchGetState(BSP_SWITCH_3_PORT));
                mySwitchMessage.switchNum = BSP_SWITCH_3;
                mySwitchMessage.switchVal = bspData.previousStateS3;
                xQueueSendToBack( app1Data.switchQueue, &mySwitchMessage, 1 );
            }
            if(BSP_SWITCH_SwitchGetState(BSP_SWITCH_4_PORT) != bspData.previousStateS4){
                BSP_SWITCH_SwitchSetPreviousState(BSP_SWITCH_4_PORT, BSP_SWITCH_SwitchGetState(BSP_SWITCH_4_PORT));
                mySwitchMessage.switchNum = BSP_SWITCH_4;
                mySwitchMessage.switchVal = bspData.previousStateS4;
                xQueueSendToBack( app1Data.switchQueue, &mySwitchMessage, 1 );
            }

            // Trigger an ADC reading every one second for the pot
            if((SYS_TMR_TickCountGet() - app1Data.potTimer) > (1000)){
                app1Data.potTimer = SYS_TMR_TickCountGet();
                DRV_ADC_Start();
            }

            // If the ADC reading is ready, see if value changed and send a message to queue
            if(DRV_ADC_DigitalFilter0_DataIsReady()) {
                app1Data.newPotSamp = (uint16_t)DRV_ADC_DigitalFilter0_DataRead();
                uint32_t adcVal;
                adcVal = app1Data.newPotSamp >> 6;
                if(adcVal != app1Data.potValue) {
                    app1Data.potValue = adcVal;
                    app1Data.potChanged = true;
                }

                if(app1Data.potChanged){
                    xQueueSendToBack( app1Data.potentiometerQueue, &app1Data.potValue, 1 );
                    app1Data.potChanged = false;
                }
            }

            // Check light show queue for a state, if exists, set state
            if( uxQueueMessagesWaiting( app1Data.lightShowQueue ) > 0 ){
                uint32_t lightShowVar;
                xQueueReceive( app1Data.lightShowQueue, &lightShowVar, 1 );
                BSP_LED_LightShowSet(lightShowVar);
            }

            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}



/*******************************************************************************
 End of File
 */
