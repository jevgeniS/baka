/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	WDT->WDT_MR = WDT_MR_WDDIS;
	

	/* Configure pins */
	gpio_configure_pin(LED0_GPIO, LED0_FLAGS);
	gpio_configure_pin(LED1_GPIO, LED1_FLAGS);
	gpio_configure_pin(LED2_GPIO, LED2_FLAGS);
	gpio_configure_pin(SS0_GPIO, SS0_FLAGS);
	gpio_configure_pin(SS1_GPIO, SS1_FLAGS);
	gpio_configure_pin(SS2_GPIO, SS2_FLAGS);
	gpio_configure_pin(SS3_GPIO, SS3_FLAGS);
	gpio_configure_pin(SS4_GPIO, SS4_FLAGS);
	gpio_configure_pin(SS5_GPIO, SS5_FLAGS);
	gpio_configure_pin(SS6_GPIO, SS6_FLAGS);
	gpio_configure_pin(SS7_GPIO, SS7_FLAGS);
	gpio_configure_pin(BUT0_GPIO, BUT0_FLAGS);
	gpio_configure_pin(D0_GPIO, D0_FLAGS);
	gpio_configure_pin(D1_GPIO, D1_FLAGS);
	gpio_configure_pin(D2_GPIO, D2_FLAGS);
	gpio_configure_pin(D3_GPIO, D3_FLAGS);
	/* Configure HSMCI pins */
	gpio_configure_pin(PIN_HSMCI_MCCDA_GPIO, PIN_HSMCI_MCCDA_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCCK_GPIO, PIN_HSMCI_MCCK_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA0_GPIO, PIN_HSMCI_MCDA0_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA1_GPIO, PIN_HSMCI_MCDA1_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA2_GPIO, PIN_HSMCI_MCDA2_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA3_GPIO, PIN_HSMCI_MCDA3_FLAGS);
	
	//gpio_configure_pin(SD_MMC_0_CD_GPIO, SD_MMC_0_CD_FLAGS);
	
}
