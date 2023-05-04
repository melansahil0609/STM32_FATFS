/*
 * fw_update.c
 *
 *  Created on: May 3, 2023
 *      Author: LENOVO
 */
#include <stdio.h>
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "fatfs.h"
#include "fw_update.h"
#include "ff.h"

#define APP_START_ADDR    0x08040000
#define APP_MAX_SIZE      0x20000   // 128KB
#define FLASH_PAGE_SIZE   100
void Flash_Read_Data (uint32_t StartPageAddress, __IO uint32_t * DATA_32);
FUNC_RET check_fw_update_SD(void){

	   FATFS FatFs;    //Fatfs handle
	   FIL   fil;      //File handle
	   FRESULT  res_mount, res_open, res_read, res_seek; //File result
	   UINT br;
	   uint32_t flash_addr = APP_START_ADDR;
	   bool FALSE  = FALSE;
	   HAL_StatusTypeDef status;

	   do{
		   res_mount = f_mount(&FatFs,"",1);
		   if(res_mount!=FR_OK){
			   printf("No SD card found\r\n");
			   return NOT_OK;
			   break;
		   }
		   res_open = f_open(&fil, FW_PATH, FA_OPEN_EXISTING | FA_READ);
		   if(res_open!=FR_OK){
			   printf("Error opening firmware image file\r\n");
			   return NOT_OK;
			   break;
		   }
		   else{
			   res_seek = f_lseek(&fil, f_size(&fil)-f_size(&fil));
			   if(res_seek!=FR_OK){
				   printf("Unable to seek");
			   }
		       HAL_FLASH_Unlock();
		       FLASH_Erase_Sector(FLASH_SECTOR_6, VOLTAGE_RANGE_3);
		       HAL_FLASH_Lock();

			   // Program firmware image chunks into flash memory
			   while (flash_addr < (APP_START_ADDR + APP_MAX_SIZE)) {
				   uint32_t sd_buffer[FLASH_PAGE_SIZE];
				   res_read = f_read(&fil, sd_buffer, FLASH_PAGE_SIZE, &br);
				       if (res_read != FR_OK) {
				         printf("Error reading firmware image from SD card\r\n");
				         break;
				       }

				       // Erase the page in the flash memory

				       // Program the page in the flash memory
				       HAL_FLASH_Unlock();
				       printf("Writing firmware to flash\n");
				       for (uint32_t i = 0; i < br; i += 4) {
				         uint32_t data = *(uint32_t*)(sd_buffer + i);
				         status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flash_addr + i, data);
				         if(status!=HAL_OK){
				        	 printf("unable to write the program to flash");
				         }
				         break;
				       }
				       HAL_FLASH_Lock();

				       // Check if programming was successful
				       /*for (uint32_t i = 0; i < br; i += 4) {
				         uint32_t data = *(uint32_t*)(flash_addr + i);
				         if (data != *(uint32_t*)(sd_buffer + i)) {
				           printf("Error programming firmware image into flash memory\r\n");
				           return NOT_OK;
				           break;
				         }
				       }*/

				       // Move to the next page in the flash memory
				       flash_addr += FLASH_PAGE_SIZE;
				     }
			   f_close(&fil);
		   }
		   return OK;
		   break;
	   }while(FALSE);
}

void Flash_Read_Data (uint32_t StartPageAddress, __IO uint32_t * DATA_32)
{
		*DATA_32 = *(__IO uint32_t *)StartPageAddress;
		if (*DATA_32 == APP_START_ADDR+APP_MAX_SIZE)
		{
			*DATA_32 = '\0';
		}
		StartPageAddress += 4;
		DATA_32++;
}
