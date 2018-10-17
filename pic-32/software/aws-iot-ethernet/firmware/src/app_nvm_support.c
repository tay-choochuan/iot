
#include "app_nvm_support.h"


extern APP_DATA appData;

bool APP_NVM_Erase(uint32_t nvm_dest_address)
{
    int tmp;
    tmp = nvm_dest_address * (appData.gAppNVMMediaGeometry->geometryTable[2].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);

    DRV_NVM_Erase(appData.nvmHandle, &appData.nvmCommandHandle[0],  tmp, 1);
    if(appData.nvmCommandHandle == DRV_NVM_COMMAND_HANDLE_INVALID)
    {
        return false;
    }

    while(DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle[0]))
    {
        ;
    }

    return true;
}

bool APP_NVM_Write(uint32_t nvm_dest_address, uint8_t * data)
{
    int tmp;
    tmp = nvm_dest_address * (appData.gAppNVMMediaGeometry->geometryTable[1].numBlocks) / (DRV_NVM_MEDIA_SIZE * 1024);
    DRV_NVM_EraseWrite(appData.nvmHandle, &appData.nvmCommandHandle[0], (uint8_t *)data, tmp, 1);
    if(DRV_NVM_COMMAND_HANDLE_INVALID == appData.nvmCommandHandle[0])
    {
        return false;
    }

    return true;
}

bool APP_NVM_Read(uint32_t nvm_dest_address, uint8_t * buffer, uint32_t bufferLength)
{
    DRV_NVM_Read(appData.nvmHandle, &appData.nvmCommandHandle[0], buffer, nvm_dest_address, bufferLength);
    if(DRV_NVM_COMMAND_HANDLE_INVALID == appData.nvmCommandHandle[0])
    {
        return false;
    }

    while(DRV_NVM_COMMAND_COMPLETED != DRV_NVM_CommandStatus(appData.nvmHandle, appData.nvmCommandHandle[0]))
    {
        ;
    }

    return true;
}
