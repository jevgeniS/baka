/**
 * \file
 *
 * \brief User board definition (definiton of used pins)
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define SD_MMC_ENABLE 1

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL		(32768U)
#define BOARD_FREQ_SLCK_BYPASS		(32768U)
#define BOARD_FREQ_MAINCK_XTAL		(12000000U)
#define BOARD_FREQ_MAINCK_BYPASS	(12000000U)

/** Master clock frequency */
#define BOARD_MCK					CHIP_FREQ_CPU_MAX

/** board main clock xtal statup time */
#define BOARD_OSC_STARTUP_US   15625

/** LED #0 (onboard) pin definition. */
#define LED0_GPIO    (PIO_PA0_IDX)
#define LED0_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)


/** LED #1 (external) pin definition. */
#define LED1_GPIO    (PIO_PA6_IDX)
#define LED1_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)


/** LED #2 (external) pin definition. */
#define LED2_GPIO    (PIO_PA5_IDX)
#define LED2_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)


/** BUT #0 (external) pin definition. */
#define BUT0_GPIO	(PIO_PA16_IDX)
#define BUT0_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE )


/** Sample Select #0 (external) pin definition. */
#define SS0_GPIO    (PIO_PA19_IDX)
#define SS0_FLAGS (PIO_INPUT | PIO_DEFAULT | PIO_DEGLITCH| PIO_DEBOUNCE )
/** Sample Select #1 (external) pin definition. */
#define SS1_GPIO    (PIO_PA22_IDX)
#define SS1_FLAGS (PIO_INPUT | PIO_DEFAULT | PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #2 (external) pin definition. */
#define SS2_GPIO    (PIO_PA23_IDX)
#define SS2_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #3 (external) pin definition. */
#define SS3_GPIO    (PIO_PA20_IDX)
#define SS3_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #4 (external) pin definition. */
#define SS4_GPIO    (PIO_PA15_IDX)
#define SS4_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #5 (external) pin definition. */
#define SS5_GPIO    (PIO_PA14_IDX)
#define SS5_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #6 (external) pin definition. */
#define SS6_GPIO    (PIO_PA13_IDX)
#define SS6_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)
/** Sample Select #7 (external) pin definition. */
#define SS7_GPIO    (PIO_PA24_IDX)
#define SS7_FLAGS (PIO_INPUT | PIO_DEFAULT| PIO_DEGLITCH| PIO_DEBOUNCE)


/** Digital output #0 (external) pin definition. */
#define D0_GPIO    (PIO_PA12_IDX)
#define D0_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)
/** Digital output #1 (external) pin definition. */
#define D1_GPIO    (PIO_PA11_IDX)
#define D1_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)
/** Digital output #2 (external) pin definition. */
#define D2_GPIO    (PIO_PA10_IDX)
#define D2_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)
/** Digital output #3 (external) pin definition. */
#define D3_GPIO    (PIO_PA9_IDX)
#define D3_FLAGS (PIO_OUTPUT_0 | PIO_DEFAULT)

/** Define pins for HSMCI driver*/
#define PINS_HSMCI   {0x3fUL << 26, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_PULLUP}
#    define SD_MMC_HSMCI_MEM_CNT          1

#    define SD_MMC_HSMCI_SLOT_0_SIZE      4 // 4-bits connector pin
#    define PIN_HSMCI_MCCDA_GPIO          (PIO_PA28_IDX)
#    define PIN_HSMCI_MCCDA_FLAGS         (PIO_PERIPH_C | PIO_DEFAULT)
#    define PIN_HSMCI_MCCK_GPIO           (PIO_PA29_IDX)
#    define PIN_HSMCI_MCCK_FLAGS          (PIO_PERIPH_C | PIO_DEFAULT)
#    define PIN_HSMCI_MCDA0_GPIO          (PIO_PA30_IDX)
#    define PIN_HSMCI_MCDA0_FLAGS         (PIO_PERIPH_C | PIO_DEFAULT)
#    define PIN_HSMCI_MCDA1_GPIO          (PIO_PA31_IDX)
#    define PIN_HSMCI_MCDA1_FLAGS         (PIO_PERIPH_C | PIO_DEFAULT)
#    define PIN_HSMCI_MCDA2_GPIO          (PIO_PA26_IDX)
#    define PIN_HSMCI_MCDA2_FLAGS         (PIO_PERIPH_C | PIO_DEFAULT)
#    define PIN_HSMCI_MCDA3_GPIO          (PIO_PA27_IDX)
#    define PIN_HSMCI_MCDA3_FLAGS         (PIO_PERIPH_C | PIO_DEFAULT)
#endif // USER_BOARD_H


