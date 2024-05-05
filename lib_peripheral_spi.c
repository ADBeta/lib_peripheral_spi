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
* ADBeta (c) 2024    Ver 0.1.4    05 May 2024
******************************************************************************/
#include "lib_peripheral_spi.h"

#include <esp_err.h>
#include <driver/spi_master.h>

// Takes a function, if the return is NOT ESP_OK, it will return the same 
// esp_err_t value. NOTE: This is similar to ESP_RETURN_ON_ERROR, but quiet
#define QUIET_RETURN_ON_ERROR(funct) do {                                     \
	esp_err_t f_ret_ = (funct);                                               \
	if(unlikely(f_ret_ != ESP_OK)) return f_ret_;                             \
} while(0)

/*** Interface Control Functions *********************************************/
esp_err_t SPI_Init(spi_handler_t *spi)
{
	// TODO: Catch init on already existing deivce
	// Make a device added to the same BUS work properly

	/*** Configure the SPI structs ***/
	spi->config = (spi_bus_config_t){
		.miso_io_num = spi->miso,
		.mosi_io_num = spi->mosi,
		.sclk_io_num = spi->sclk,
		.max_transfer_sz = spi->max_bytes,	

		// Unused pins and options. -1 to disable
		.quadhd_io_num = -1,
		.quadwp_io_num = -1,
	};

	spi->iface = (spi_device_interface_config_t){
		.spics_io_num = spi->cs,
		.mode = spi->mode,
		.clock_speed_hz = spi->hz,
		.input_delay_ns = spi->delay,
		.queue_size = 7,
		
		.command_bits = spi->cmd_addr_bits,
		.address_bits = spi->cmd_addr_bits,

		// Unused features
		.dummy_bits = 0,
		.duty_cycle_pos = 128,
		// NOTE: Not using clalbacks in this version. May add them later
		.pre_cb = NULL,
		.post_cb = NULL,
	};
	
	// Set Flags if specified by init
	if(spi->half_duplex) spi->iface.flags |= SPI_DEVICE_HALFDUPLEX; 
	if(spi->positive_cs) spi->iface.flags |= SPI_DEVICE_POSITIVE_CS;
	
	/*** Set up the SPI Driver ***/
	esp_err_t status;

	status = spi_bus_initialize(spi->host, &spi->config, SPI_DMA_CH_AUTO);
	if(status != ESP_OK) return status;

	status = spi_bus_add_device(spi->host, &spi->iface, &spi->handle); 
	return status;
}

esp_err_t SPI_Transmit(const spi_handler_t *spi, const uint64_t addr, 
					                      const uint8_t *arr, const size_t len)
{
	esp_err_t status;

	// TODO: detect small enough size to not use array
	size_t bits = len * 8;

	spi_transaction_t t = {
		.addr = addr,    
		.length = bits,
		.tx_buffer = arr,

		.rxlength = 0,
		.rx_buffer = NULL,
		
		//.flags = SPI_TRANS_USE_RXDATA, 
	};

	// TODO: poll or interrupt depending on options
	status = spi_device_polling_transmit(spi->handle, &t);

	return status;
}



esp_err_t SPI_Receive(const spi_handler_t *spi, const uint64_t addr, 
					                            uint8_t *out, const size_t len)
{
	esp_err_t status;

	// TODO: detect small enough size to not use array
	size_t bits = len * 8;

	spi_transaction_t t = {
		.cmd = addr,    
		.length = bits,
		.rxlength = bits,
		.rx_buffer = out,
		
		.tx_buffer = NULL,
		//.flags = SPI_TRANS_USE_RXDATA, 
	};

	// TODO: poll or interrupt depending on options
	status = spi_device_polling_transmit(spi->handle, &t);

	return status;
}
