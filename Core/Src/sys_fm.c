/*
 * sys_fm.c
 *
 *  Created on: 12-Jul-2025
 *      Author: Abhinav
 */
#include "sys_fm.h"

static const int tft_y_offset = 12;
static char *result[20] = {
		"ok",
		"disk error",
		"not ready",
		"no file",
		"no path",
		"invalid name",
		"denied",
		"no access",
		"invalid object",
		"write protect",
		"invalid drive",
		"no area",
		"no fs",
		"mkfs abort",
		"timeout",
		"locked",
		"no mem",
		"too many files",
		"invalid param"
};

char buffer[4096];

void sys_flog(tft_t *tft, char *str, FRESULT res)
{
	char buff[32];
	memset(buff, 0, sizeof(buff));
	tft_fill_rect(tft, 3, 147, ST_WIDTH-4, 8, BLACK);
	sprintf(buff, "%s %s", str, result[res]);
	tft_write_string(tft, 3, 147, buff, ORANGE, BLACK);
}

void sys_title(tft_t *tft, const char *title)
{
	tft_fill_rect(tft, 3, 3, ST_WIDTH-4, 8, BLACK);
	(strcmp(title, "") == 0) ? 
		tft_write_string(tft, 3, 3, "/", ORANGE, BLACK) :
		tft_write_string(tft, 3, 3, (char *)title, ORANGE, BLACK);
}

int sys_writeFileEntries(const TCHAR *path, FileEntry *fileEntries)
{
	DIR fdir = {0};
	FRESULT fresult = FR_OK;
	FILINFO flinfo = {0};
	fileEntries->index = 0;
	//fileEntries->path = path;
	fresult = f_opendir(&fdir, path);
	if (fresult != FR_OK)
	{
		return -1;
	}

	sys_flog(&tftObject, "dir open", fresult);

	while (1)
	{
		fresult = f_readdir(&fdir, &flinfo);
		if ((fresult != FR_OK) || (flinfo.fname[0] == 0))
		{
			break;
		}	
		if (!(flinfo.fattrib & AM_HID))
		{
			if (fileEntries->index < MAX_FILE_ENTRIES)
			{
				fileEntries->flinfo[fileEntries->index] = flinfo;
				fileEntries->index++;
			}
		}
	}

	fresult = f_closedir(&fdir);
	if (fresult != FR_OK)
	{
		return -2;
	}

	sys_flog(&tftObject, "dir close", fresult);

	return fileEntries->index;
}

static void sys_processFile(FILINFO fileInfo)
{
	char *fileNameToken;
	char *tempFileName = buffer;

	memset(buffer, 0, sizeof(buffer));
	strcpy(tempFileName, fileInfo.fname);
	strtok(tempFileName, ".");
	fileNameToken = strtok(NULL, ".");
	if (fileNameToken != NULL)
	{
		if (strcmp((const char*) fileNameToken, "TXT") == 0)
		{
			sys_fread(&tftObject, fileInfo.fname);
		}
		else if (strcmp((const char*) fileNameToken, "C") == 0)
		{
			sys_fread(&tftObject, fileInfo.fname);
		}
		else if (strcmp((const char*) fileNameToken, "BMP") == 0)
		{
			sys_readBMP(&tftObject, fileInfo.fname);
		}
		else if (strcmp((const char*) fileNameToken, "JPG") == 0)
		{
			sys_readJPG(&tftObject, fileInfo.fname);
		}
		else if (strcmp((const char*) fileNameToken, "JPEG") == 0)
		{
			sys_readJPG(&tftObject, fileInfo.fname);
		}
		else if (strcmp((const char*) fileNameToken, "BIN") == 0)
		{
			sys_readBIN(&tftObject, fileInfo.fname);
		}
	}
}

uint8_t sys_flist(FileEntry fileTable, tft_t *tft, uint8_t tft_x_pos, uint8_t tft_y_pos)
{
	uint8_t selected = 0;
	uint16_t item_color = WHITE;
	uint8_t buttonOkStatus = 0;
	uint8_t original_y_pos = tft_y_pos;
	uint8_t run = 1;

	tft_fill_rect(tft, 0, 0, ST_WIDTH, ST_HEIGHT, BLACK);
	tft_new_bar(tft, BLUE, 144);
	tft_new_bar(tft, BLUE, 0);
	//sys_title(tft, fileTable.path);

	for (uint8_t i = 0; ((i < MAX_TFT_LIST_ENTRIES) && (i < fileTable.index)); i++)
	{
		item_color = (fileTable.flinfo[i].fattrib & AM_DIR) ? YELLOW : WHITE;
		tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[i].fname, item_color, BLACK);
		tft_y_pos++;
	}

	tft_y_pos = original_y_pos;
	tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[selected].fname, GREEN, BLACK);

	while (run)
	{
		if ((HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == 1) && (buttonOkStatus == 0))
		{
			HAL_Delay(200);
			item_color = (fileTable.flinfo[selected].fattrib & AM_DIR) ? YELLOW : WHITE;
			tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[selected].fname, item_color, BLACK);
			selected++;
			tft_y_pos++;
			if ((selected >= MAX_TFT_LIST_ENTRIES) || (tft_y_pos >= (MAX_TFT_LIST_ENTRIES+2)) || (selected >= fileTable.index))
			{
				selected = 0;
				tft_y_pos = original_y_pos;
			}
			tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[selected].fname, GREEN, BLACK);
		}
		if ((HAL_GPIO_ReadPin(BUTTON_OK_GPIO_Port, BUTTON_OK_Pin) == 1) && (buttonOkStatus == 0))
		{
			HAL_Delay(200);
			if (fileTable.flinfo[selected].fattrib & AM_DIR)
			{
				//fresult = f_getcwd(cwd, 32);
				//memset(cwd, 0, 32);
				/*
				fresult = f_chdir(fileTable.flinfo[selected].fname);
				fresult = f_getcwd(cwd, 32);
				sys_writeFileEntries(cwd, &newFileTable);
				if (fresult == FR_OK)
				{
					sys_flist(newFileTable, tft, 0, 2);
				}
				//f_chdir(path);
				buttonOkStatus = 1;
				*/
				return selected;
			}
			else
			{
				sys_processFile(fileTable.flinfo[selected]);
				buttonOkStatus = 1;
			}
		}
		if (HAL_GPIO_ReadPin(BUTTON_BACK_GPIO_Port, BUTTON_BACK_Pin) == 1)
		{
			HAL_Delay(200);
			if (buttonOkStatus == 0)
			{
				run = 0;
			}
			else if (buttonOkStatus == 1)
			{
				buttonOkStatus = 0;
				tft_fill_rect(tft, 0, 0, ST_WIDTH, ST_HEIGHT, BLACK);
				tft_new_bar(tft, BLUE, 144);
				tft_new_bar(tft, BLUE, 0);
				//sys_title(tft, fileTable.path);
				tft_y_pos = original_y_pos;
				for (uint8_t i = 0; ((i < MAX_TFT_LIST_ENTRIES) && (i < fileTable.index)); i++)
				{
					item_color = (fileTable.flinfo[i].fattrib & AM_DIR) ? YELLOW : WHITE;
					tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[i].fname, item_color, BLACK);
					tft_y_pos++;
				}
				tft_y_pos = selected + 2;
				tft_write_string(tft, tft_x_pos, tft_y_pos * tft_y_offset, fileTable.flinfo[selected].fname, GREEN, BLACK);
			}
		}
	}

	return 0;
}

int sys_fread(tft_t *tft, const TCHAR* path)
{
	UINT count = 0;
	FIL fil;
	FRESULT fresult;

	memset(buffer, 0, sizeof(buffer));
	sys_title(tft, path);
	fresult = f_open(&fil, path, FA_OPEN_ALWAYS | FA_READ);
	sys_flog(tft, (char*)path, fresult);
	if (fresult != FR_OK)
	{
		return -1;
	}
	
	fresult = f_read(&fil, buffer, fil.fsize, &count);
	sys_flog(tft, "file read", fresult);
	if (fresult != FR_OK)
	{
		goto close;
	}
	
	buffer[count] = 0;
	tft_fill_rect(tft, 0, 2 * tft_y_offset, ST_WIDTH, ST_HEIGHT-44, BLACK);
	tft_write_string(tft, 0, 2 * tft_y_offset, buffer, WHITE, BLACK);

close:
	fresult = f_close(&fil);
	sys_flog(tft, "file close", fresult);
	if (fresult != FR_OK)
	{
		return -2;
	}
	return 0;
}

static inline uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) |
           ((g & 0xFC) << 3) |
           (b >> 3);
}

int sys_readBMP(tft_t *tft, const TCHAR *path)
{
	UINT count = 0;
	FIL fil = {0};
	FRESULT fresult = FR_OK;
	b_header_t bmpHeader = {0};
	b_dib_t bmpInfoHeader = {0};
	uint8_t bmpHeight = 0;
	uint8_t bmpWidth = 0;
	uint8_t rgb[3] = {0};
	uint16_t colorData = 0;

	fresult = f_open(&fil, path, FA_OPEN_ALWAYS | FA_READ);
	if (fresult != FR_OK)
	{
		return -1;
	}

	fresult = f_read(&fil, &bmpHeader, sizeof(bmpHeader), &count);
	fresult = f_read(&fil, &bmpInfoHeader, sizeof(bmpInfoHeader), &count);
	if (fresult != FR_OK)
	{
		goto close;
	}

	bmpHeight = bmpInfoHeader.height;
	bmpWidth  = bmpInfoHeader.width;

	f_lseek(&fil, bmpHeader.offset);
	if (fresult != FR_OK)
	{
		goto close;
	}

	tft_fill_rect(tft, 0, 0, ST_WIDTH, ST_HEIGHT, BLACK);
	tft_set_addr_window(tft, 0, 0, bmpWidth-1, bmpHeight-1);

	for (uint8_t y = 0; y < bmpHeight; y++)   // BMP bottom-up
	{
		for (uint8_t x = 0; x < bmpWidth; x++)
		{
			f_read(&fil, rgb, 3, &count);
			colorData = RGB565(rgb[2], rgb[1], rgb[0]);
	        tft_cs_low(tft);
	        tft_send_data(tft, colorData >> 8);
	        tft_send_data(tft, colorData & 0xFF);
	        tft_cs_high(tft);
	    }
	}

close:
	fresult = f_close(&fil);
	if (fresult != FR_OK)
	{
		return -2;
	}
	return 0;
}

UINT in_func (JDEC *jd, BYTE *buff, UINT nbyte)
{
    UINT br = nbyte;
    FIL *fp = (FIL*)jd->device;

    if(buff)
        f_read(fp, buff, nbyte, &br);
    else
        f_lseek(fp, f_tell(fp) + nbyte);

    return br;
}

int out_func (JDEC *jd, void *bitmap, JRECT *rect)
{
    uint16_t *pixels = (uint16_t *)bitmap;
    uint16_t width  = rect->right - rect->left + 1;
    uint16_t height = rect->bottom - rect->top + 1;

    tft_set_addr_window(&tftObject, rect->left, rect->top, rect->right, rect->bottom);

    tft_cs_low(&tftObject);
    for (uint32_t i = 0; i < width * height; i++) {
    	tft_send_data(&tftObject, pixels[i] >> 8);
    	tft_send_data(&tftObject, pixels[i] & 0xFF);
    }
    tft_cs_high(&tftObject);

    return 1;
}

int sys_readJPG(tft_t *tft, const TCHAR *path)
{
	FIL file;
	JDEC jd;
	JRESULT res;

	memset(buffer, 0, sizeof(buffer));
	f_open(&file, path, FA_OPEN_ALWAYS | FA_READ);
	res = jd_prepare(&jd, in_func, buffer, sizeof(buffer), &file);

	if (res == JDR_OK)
	{
		tft_fill_rect(tft, 0, 0, ST_WIDTH, ST_HEIGHT, BLACK);
		tft_set_addr_window(tft, 0, 0, ST_WIDTH-1, ST_HEIGHT-1);
		jd_decomp(&jd, out_func, 0);
	}

	f_close(&file);
	return 0;
}

int sys_readBIN(tft_t *tft, const TCHAR* path)
{
	UINT count = 0;
	FIL fil;
	FRESULT fresult;
	uint16_t data = 0;

	fresult = f_open(&fil, path, FA_OPEN_ALWAYS | FA_READ);
	sys_flog(tft, (char*)path, fresult);
	if (fresult != FR_OK)
	{
		return -1;
	}

	if (fresult != FR_OK)
	{
		goto close;
	}

	tft_fill_rect(tft, 0, 0, ST_WIDTH, ST_HEIGHT, BLACK);
	tft_set_addr_window(tft, 0, 0, ST_WIDTH-1, ST_HEIGHT-1);

	tft_cs_low(tft);
	for (uint16_t x = 0; x < 901; x++)
	{
		for (uint16_t i = 0; i < (ST_WIDTH * ST_HEIGHT); i++)
		{
			fresult = f_read(&fil, &data, sizeof(data), &count);
			if (fresult != FR_OK)
			{
				break;
			}
			tft_send_data(tft, data & 0xFF);
			tft_send_data(tft, data >> 8);
		}
	}
	tft_cs_high(tft);

close:
	fresult = f_close(&fil);
	sys_flog(tft, "file close", fresult);
	if (fresult != FR_OK)
	{
		return -2;
	}
	return 0;
}


