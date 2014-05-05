#include <asf.h>
#include <stdlib.h>
#include <string.h>
#include "ff.h"
#include "sd_mmc_mem.h"
#define MAX_SAMPLES 15
#define MAX_FREQ_1FILES 90000
#define MAX_FREQ_2FILES 50000

//! Analog control value for DAC
#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02) | DACC_ACR_IBCTLDACCORE(0x01))

// Structure for definition of sample files
typedef struct
{
	//filename size max is 30 chars
	char filenames[MAX_SAMPLES][30];
	uint32_t file_frequencies[MAX_SAMPLES];
	
}sample_files;

// FS struct
static FATFS fs;
// sample files struct
sample_files samples;
// file struct
static FIL sample0,sample1;
// DAC signal for ch.0
uint16_t ch0_value=0;
// DAC signal for ch.1
uint16_t ch1_value=0;
// merged data of DAC ch.0 and ch.1 
uint32_t dac_value;
// digital output defining bit array
uint8_t digitals=0;
// current frequency
uint32_t current_freq;
// number of bytes read from file
UINT br0,br1;
// ch.0 status
bool ch0=false;
// ch.1 status
bool ch1=false;
// common file for both channel
bool common_file=false;
//	PIOA struct pointing to peripheral
Pio *p_pio = (Pio *)((uint32_t)PIOA );
//	input data selection ch.0
uint8_t ch0_ss=0;
//	input data selection ch.1
uint8_t ch1_ss=0;
//	FS operation status
volatile FRESULT r;
//	file read buffer
volatile uint16_t buffer;

/*
	compare given filename with the expecting one
	returns true if match
*/
bool validate_file(char *fn);

/*
	scan all files in given as a parameter directory name
	returns status FR_OK if success
*/
FRESULT scan_files (char* path);

/*
	check the selected sample for exact channel given as a parameter
	returns selected input data index
*/
uint8_t getSelectedSample(uint8_t channel);

/*
	check frequency of file with given number on some digit 
*/
int checkSelectedSample(uint8_t number);

/*
	set temporary data (for one sample) for given DAC-channel and update digitals value
*/
void setData(uint16_t *ch_value1, uint8_t *digitals,uint8_t ch);

/*
	initialize DAC
*/
void configure_dacc(void);

/*
	update temporary data for the next sample
*/
int updateSamples(void);

/*
	try to start generate signals with given inputs
*/
int startGenerating(void);

/*
	read from file for given channel
*/
int readFile(uint8_t channel);

/*
	stop SysTick timer
*/
void stopSysTick(void);

int updateSamples(){
	digitals=0;
	if (common_file){
		readFile(0);
		setData(&ch0_value,&digitals,0);
		setData(&ch1_value,&digitals,1);
	}
	else{
		if(ch0){
			readFile(0);
			setData(&ch0_value,&digitals,0);
		}
		if(ch1){
			readFile(1);
			setData(&ch1_value,&digitals,1);
		}		
	}	
	dac_value=((0x2000|ch0_value)<<16)|(0x1000|ch1_value);
	return 1;
	}
	
int readFile(uint8_t channel){
	if (channel==0){
		r = f_read(&sample0, &buffer, sizeof(buffer), &br0);
		if (r!=FR_OK){
			return 4;
		}
		if (sample0.fptr>sample0.fsize-2){
			sample0.fptr=0;
		}
	}
	if (channel==1){
		r = f_read(&sample1, &buffer, sizeof(buffer), &br1);
		if (r!=FR_OK){
			return 4;
		}
		if (sample1.fptr>sample1.fsize-2){
			sample1.fptr=0;
		}
	}
	return 1;
}

void SysTick_Handler(void)
{
	/* If ready for new data */
	if ((dacc_get_interrupt_status(DACC) & DACC_ISR_TXRDY) == DACC_ISR_TXRDY) {
		dacc_write_conversion_data(DACC, dac_value);
		p_pio->PIO_CODR |= (~(digitals)<<D3_GPIO);
		p_pio->PIO_SODR |= (digitals<<D3_GPIO);
		updateSamples();
	}
}
int startGenerating(){
	if (ch0_ss>0 && checkSelectedSample(ch0_ss-1)) ch0=true;
	if (ch1_ss>0 && checkSelectedSample(ch1_ss-1)) ch1=true;
	
	if (!(ch0 || ch1)) return 0;
	if (ch0 && ch1){
		if (samples.file_frequencies[ch0_ss-1]!=samples.file_frequencies[ch1_ss-1]){
			return 2;
		}
	}	
	if (ch0 && ch1 && ch0_ss==ch1_ss){
		r=f_open(&sample0,samples.filenames[ch0_ss-1],FA_OPEN_EXISTING | FA_READ);
		if (r!=FR_OK) return 3;
		current_freq=samples.file_frequencies[ch0_ss-1];
		common_file=true;
	}
	else{
		if (ch0){
			r=f_open(&sample0,samples.filenames[ch0_ss-1],FA_OPEN_EXISTING | FA_READ);
			if (r!=FR_OK) return 3;
			current_freq=samples.file_frequencies[ch0_ss-1];
		}
		
		if (ch1){
			r=f_open(&sample1,samples.filenames[ch1_ss-1],FA_OPEN_EXISTING | FA_READ);
			if (r!=FR_OK) return 3;
			current_freq=samples.file_frequencies[ch1_ss-1];
		}		
	}		
	if (!updateSamples()) return 4;
	return 1;
}

int main(void){
	Ctrl_status status;
	board_init();
	
	sysclk_init();
	
    sysclk_enable_peripheral_clock(ID_PIOA);
	configure_dacc();
	sd_mmc_init();
	card_init:
	gpio_set_pin_low(LED2_GPIO);
	while (CTRL_GOOD != sd_mmc_test_unit_ready(0));
	r=sd_mmc_check(0);
	r = f_mount(0, &fs);
	if (FR_OK != r) {
			
			goto card_init;
		}
		
	r=scan_files("/");	
	gpio_set_pin_high(LED1_GPIO);
		
	while (gpio_pin_is_low(BUT0_GPIO));
	
	ch0_ss = getSelectedSample(0);
	ch1_ss = getSelectedSample(1);
	int state;
	
	start:
	state=startGenerating();
	if (state==1){
		
		gpio_set_pin_high(LED2_GPIO);
	
		if (ch0 && ch1 && !common_file){
			if (current_freq>MAX_FREQ_2FILES)
				current_freq=MAX_FREQ_2FILES;
		}
		else{
			if (current_freq>MAX_FREQ_1FILES)
				current_freq=MAX_FREQ_1FILES;
		}						
			
		SysTick_Config(sysclk_get_cpu_hz()/current_freq);
	}		
	else if(state>=3){
		goto close_files; 
	}
	else {
		goto card_init;
	}
	
	while (gpio_pin_is_high(BUT0_GPIO));
	
	close_files:
	f_close(&sample0);
	f_close(&sample1);
	
	f_mount(0, NULL);
	goto card_init;
	
}

void stopSysTick(){
	SysTick->CTRL  = 0<<SysTick_CTRL_ENABLE_Pos;
}

FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
 
    char *fn; 
	
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif
    res = f_opendir(&dir, path); //Open dir
    if (res == FR_OK)
    {
	
        for (;;)
        {
            res = f_readdir(&dir, &fno);  // Read directory
            if (res != FR_OK || fno.fname[0] == 0) 
               break; // if file end or error
            if (fno.fname[0] == '.')
               continue; // dot path
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
			
#else
            fn = fno.fname;
#endif
            if (!(fno.fattrib & AM_DIR)){
				validate_file(fn);
            }
        }
    }
    return res;
}
/* Store filename into array if it is valid*/
bool validate_file(char *fn)
{
	uint8_t sample_num=0;
	uint16_t sample_freq=0;
	char * pch;
	
	if (fn[0]=='s'){
		char temp[30];
		strcpy(temp,fn);
		pch = strtok (temp,"_");
		int i=0;
		while (pch != NULL)
		{
			i++;
			pch = strtok (NULL, "_");
			if (i==1) sample_num=(uint8_t)atoi(pch);
			if (i==2) sample_freq=(uint16_t)atoi(pch);
		}
	}
	else {
		return false;
	}
	
	if (sample_num > 0 && sample_freq >0 && sample_num<=MAX_SAMPLES){
		strcpy(samples.filenames[sample_num-1],fn);
		samples.file_frequencies[sample_num-1]=sample_freq;
	}
	else {
		return false;
	}

	return true;
}

int checkSelectedSample(uint8_t number){
	if (samples.file_frequencies[number]>0 && samples.filenames[number]!=NULL)
		return 1;
	return 0;
}

uint8_t getSelectedSample(uint8_t channel){
	uint8_t value=0;
	if (channel==0){
		value=(gpio_pin_is_high(SS0_GPIO) <<0) |(gpio_pin_is_high(SS1_GPIO) <<1) 
		|(gpio_pin_is_high(SS2_GPIO) <<2) |(gpio_pin_is_high(SS3_GPIO) <<3);
	}
	else{
		value=(gpio_pin_is_high(SS4_GPIO) <<0) |(gpio_pin_is_high(SS5_GPIO) <<1)
		|(gpio_pin_is_high(SS6_GPIO) <<2) |(gpio_pin_is_high(SS7_GPIO) <<3);
	}
	return value;
	
}

void configure_dacc(void){
	/* Enable clock for DACC */
	pmc_enable_periph_clk(ID_DACC);
	
	/* Reset DACC registers */
	dacc_reset(DACC);

	/* Half word transfer mode */
	dacc_set_transfer_mode(DACC, 1);
	dacc_set_power_save(DACC, 0, 0);
	
	/* Timing:
	 * refresh        - 0x08 (1024*8 dacc clocks)
	 * max speed mode -    0 (disabled)
	 * startup time   - 0x10 (1024 dacc clocks)
	 */
	dacc_set_timing(DACC, 0x08, 0, 0x10);

	/* Disable TAG and select output channel DACC_CHANNEL */
	//dacc_set_channel_selection(DACC, 0);
	dacc_enable_flexible_selection(DACC);
	
	/* Enable output channel DACC_CHANNEL */
	dacc_enable_channel(DACC, 0);
	dacc_enable_channel(DACC, 1);
	/* Set up analog current */
	dacc_set_analog_control(DACC, DACC_ANALOG_CONTROL);
	
}
void setData(uint16_t *ch_value, uint8_t *ch_digitals,uint8_t ch){
	*ch_value=(((buffer>>8)|(buffer<<8)));
	*ch_digitals|=(*ch_value & 0xF000)>>12-2*ch;
	*ch_value=(*ch_value & 0x0FFF);
}

