/******************************************************************************
* ESP32 Peripheral Library Collection: SPI Master Handler - 3 & 4 Wire ONLY
* 
* This program is free software; you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by the 
* Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General 
* Public License for more details.
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
* ADBeta (c) 2024
******************************************************************************/
#ifndef LIB_PERIPH_SPI
#define LIB_PERIPH_SPI

#include <esp_err.h>
#include <driver/spi_master.h>

/*** Interface Control Functions *********************************************/
typedef struct {
	// Configuration Variables
	spi_host_device_t host;        // SPI Host Device,e.g SPI2_HOST
	uint8_t miso, mosi, sclk, cs;  // SPI GPIO Pins
	uint32_t hz;                   // SPI Clock Speed in hz
	int max_bytes;                 // Max transfer size. Leave at 0 for auto
	uint8_t mode;                  // CPOL CPHA mode, 0 is "normal"
	int delay;                     // ns delay from SCLK to valid Data. 0 Default
	uint8_t cmd_addr_bits;         // CMD and ADDR Bits (Single variable)
	
	bool intr_transfer;            // Use Interrupt type transfer?
	bool half_duplex;              // Use Half Duplex Mode?
	bool positive_cs;              // Is CS Active Positive?

	// WARN: Do NOT change these variables manually
	spi_device_handle_t           handle;
	spi_device_interface_config_t iface;
	spi_bus_config_t              config;
} spi_handler_t;


// Take an SPI Handler, and set up and initialise the bus with the given
// parameters.
// Returns ESP_OK on success
esp_err_t SPI_Init(spi_handler_t *spi);

// Take an SPI Handler, Address, Array and Length. 
// Transmit to the Slave device at a specified address using polling or
// interrupt transaction methods, depending on user configuration.
// NOTE: If cmd_addr_bits are 0, this will avoid sending any address data
esp_err_t SPI_Transmit(const spi_handler_t *spi, const uint64_t addr, 
					                     const uint8_t *arr, const size_t len);

// Take an SPI Handler, Address, Array and Length.
// Receive from the Slave device specific address using polling or interrupt
// transaction methods, depending on user configuration.
// NOTE: If cmd_addr_bits are 0, this will avoid sending any address data
esp_err_t SPI_Receive(const spi_handler_t *spi, const uint64_t addr, 
					                           uint8_t *out, const size_t len);

#endif
