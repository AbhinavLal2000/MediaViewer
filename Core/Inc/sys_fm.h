/*
 * sys_fm.h
 *
 *  Created on: 12-Jul-2025
 *      Author: Abhinav
 */

#ifndef INC_SYS_FM_H_
#define INC_SYS_FM_H_

#include "fatfs.h"
#include "tft.h"
#include "bmp.h"
#include "tjpgd.h"

#define MAX_FILE_ENTRIES     256
#define MAX_TFT_LIST_ENTRIES 10

extern tft_t tftObject;

struct button_t
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
};
typedef struct button_t button;

struct FileEntry_t
{
    const TCHAR* path;
    FILINFO flinfo[MAX_FILE_ENTRIES];
    uint8_t index;
};
typedef struct FileEntry_t FileEntry;

void sys_flog(tft_t *tft, FRESULT res);
void sys_title(tft_t *tft, const char *title);
int sys_writeFileEntries(const TCHAR* path, FileEntry *fileEntries);
int sys_flist(FileEntry *fileTable, tft_t *tft, uint8_t tft_x_pos, uint8_t tft_y_pos);
int sys_fread(tft_t *tft, const TCHAR* path);
int sys_readBMP(tft_t *tft, const TCHAR *path);
int sys_readJPG(tft_t *tft, const TCHAR *path);

#endif /* INC_SYS_FM_H_ */
