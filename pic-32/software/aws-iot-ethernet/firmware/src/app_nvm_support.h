

#ifndef APP_NVM_SUPPORT_H
#define	APP_NVM_SUPPORT_H

#include "app.h"

bool APP_NVM_Erase(uint32_t nvm_dest_address);
bool APP_NVM_Write(uint32_t nvm_dest_address, uint8_t * data);
bool APP_NVM_Read(uint32_t nvm_dest_address, uint8_t * buffer, uint32_t bufferLength);

#endif	/* APP_NVM_SUPPORT_H */
