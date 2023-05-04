/*
 * fw_update.h
 *
 *  Created on: May 3, 2023
 *      Author: LENOVO
 */

#ifndef INC_FW_UPDATE_H_
#define INC_FW_UPDATE_H_

#define FW_PATH "application_code.bin"
typedef enum{
  FW_OK       = 0,    // Updated firmware successfully
  NO_SD    = 1,    // No SD card Found
  FU_ERR   = 2,    // Failure during Firmware update
  O_ERR      = 3,    // Other Failure
}SD_FW;

typedef enum{
	OK = 0,  // SUCCESS
	NOT_OK = -1, //FAILURE
}FUNC_RET;

#endif /* INC_FW_UPDATE_H_ */
