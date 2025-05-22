/* ************ MICROSEMI SOC CORP. DIRECTC LICENSE AGREEMENT ************* */
/* ------------------------------------------------------------------------ 
PLEASE READ: BEFORE INSTALLING THIS SOFTWARE, CAREFULLY READ THE FOLLOWING 
MICROSEMI SOC CORP LICENSE AGREEMENT REGARDING THE USE OF THIS SOFTWARE. 
INSTALLING THIS SOFTWARE INDICATES THAT YOU ACCEPT AND UNDERSTAND THIS AGREEMENT 
AND WILL ABIDE BY IT. 

Note: This license agreement (“License”) only includes the following software: 
DirectC. DirectC is licensed under the following terms and conditions.

Hereinafter, Microsemi SoC Corp. shall be referred to as “Licensor” or “Author,” 
whereas the other party to this License shall be referred to as “Licensee.” Each 
party to this License shall be referred to, singularly, as a “Party,” or, 
collectively, as the “Parties.”

Permission to use, copy, modify, and/or distribute DirectC for any purpose, with
or without fee, is hereby granted by Licensor to Licensee, provided that the 
above Copyright notice and this permission notice appear in all copies, 
modifications and/or distributions of DirectC.

DIRECTC IS PROVIDED "AS IS" AND THE AUTHOR/LICENSOR DISCLAIMS ALL WARRANTIES 
WITH REGARD TO DIRECTC INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS. IN NO EVENT SHALL AUTHOR/LICENSOR BE LIABLE TO LICENSEE FOR ANY DAMAGES, 
INCLUDING SPECIAL, DIRECT,INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF 
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION 
WITH THE USE OR PERFORMANCE OF DIRECTC.

Export Control: Information furnished to Licensee may include United States 
origin technical data. Accordingly, Licensee is responsible for complying with, 
and warrants to Licensor that it will comply with, all U.S. export control laws 
and regulations, including the provisions of the Export Administration Act of 
1979 and the Export Administration Regulations promulgated thereunder, the Arms 
Export Control Act, and the sanctions laws administered by the Office of Foreign 
Assets Control including any other U.S. Government regulation applicable to the 
export, re-export, or disclosure of such controlled technical data (or the 
products thereof) to Foreign Nationals, whether within or without the U.S., 
including those employed by, or otherwise associated with, Licensee. Licensee 
shall obtain Licensor’s written consent prior to submitting any request for 
authority to export any such technical data.

ADR: Any dispute between the Parties arising from or related to this License or 
the subject matter hereof, including its validity, construction or performance 
thereunder, shall be exclusively resolved through arbitration by a mutually 
acceptable impartial and neutral arbitrator appointed by the Judicial 
Arbitration and Mediation Services (JAMS) in accordance with its rules and 
procedures. If the Parties are not able to agree on an arbitrator within 10 days 
of the date of request for mediation is served, then JAMS shall appoint an 
arbitrator. Notice of arbitration shall be served and filed with the JAMS main 
offices in Irvine, California. Each Party shall be responsible for all costs 
associated with the preparation and representation by attorneys, or any other 
persons retained thereby, to assist in connection with any such Arbitration. 
However, all costs charged by the mutually agreed upon Arbitration entity shall 
be equally shared by the Parties. The Party seeking Mediation and/or Arbitration 
as provided herein agrees that the venue for any such Mediation and Arbitration 
shall be selected by the other Party and that such venue must be Los Angeles, 
California; New York, New York; or Chicago, Illinois; whereby the applicable law 
and provisions of the Evidence Code of the State selected thereby shall be 
applicable and shall govern the validity, construction and performance of this 
License.

Governing Law: This license will be governed by the laws of the State of 
California, without regard to its conflict of law provisions.

Entire Agreement: This document constitutes the entire agreement between the 
Parties with respect to the subject matter herein and supersedes all other 
communications whether written or oral.                                     */

/* ************************************************************************ */
/*                                                                          */
/*  JTAG_DirectC    Copyright (C) Microsemi Corporation                     */
/*  Version 2022.1  Release date July 2022                                  */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpS25F.c                                                */
/*                                                                          */
/*  Description:    Contains S25F specific SPI-Flash functions.             */
/*                                                                          */
/* Device Manufacturer: Cypress / Spansion                                  */
/* Specs: https://www.cypress.com/file/177971/download                      */
/* Devices tested: S25FL512S                                                */
/* ************************************************************************ */

#include "dpuser.h"
#ifdef ENABLE_SPI_FLASH_SUPPORT
#include "dpcom.h"
#include "dpjtag.h"
#include "dpSpiProg.h"
#include "dpSPIalg.h"
#include "dpalg.h"
#include "dpS25F.h"


DPUCHAR dp_top_S25F(void) 
{
    if (error_code == DPE_SUCCESS)
    {
        dp_perform_S25F_action();
    }
    return error_code;
}

void dp_perform_S25F_action(void)
{
    if (Action_code != DP_SPI_FLASH_READ_ID_ACTION_CODE)
    {
        S25F_reset();
        S25F_clear_status_register();
        S25F_write_status_register(0x0);
        if ((address_mode == ADDRESS_4BYTE_MODE))
        {
            dp_display_text("\r\nSetting address mode to 4 bytes in register");
            if (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE1_ID)
                S25F_write_bank_address_register(0x80);
            else
            S25F_enable_4byte_address_mode();
        }
        if (error_code == DPE_SUCCESS)
        {
            switch(Action_code)
            {
                case DP_SPI_FLASH_READ_ACTION_CODE:
                dp_SPI_read_action();
                break;         
                case DP_SPI_FLASH_ERASE_ACTION_CODE:
                dp_S25F_erase_action();
                break;
                case DP_SPI_FLASH_PROGRAM_ACTION_CODE:
                dp_S25F_program_action();
                break;
                case DP_SPI_FLASH_VERIFY_ACTION_CODE:
                dp_SPI_verify_action();
                break;
                case DP_SPI_FLASH_BLANK_CHECK_ACTION_CODE:
                dp_SPI_blank_check_action();
                break;
                case DP_SPI_FLASH_ERASE_IMAGE_ACTION_CODE:
                dp_S25F_image_erase_action();
                break;
            }
        }
        if (error_code == DPE_SUCCESS)
        {
            dp_display_text("\r\nOperation Status: Passed");
        }
        else
        {
            dp_display_text("\r\nError: Operation Status: Failed");
        }
        
    }
    return;
}

void dp_S25F_erase_action(void)
{
    dp_display_text("\r\nPerforming SPI Flash Die Erase Action:\r\n");
    if (error_code == DPE_SUCCESS)
    {
        S25F_die_erase();
    }
}

void dp_S25F_image_erase_action(void)
{
    dp_display_text("\r\nPerforming SPI Flash Image Erase Action: ");
    bytes_processed = 0u;
    
    if (error_code == DPE_SUCCESS)
    {
        dp_check_image_address_and_size();
        
        if (error_code == DPE_SUCCESS)
        {
            dp_S25F_erase();
        }
    }
    return;
}

void dp_S25F_program_action(void)
{
    dp_display_text("\r\nPerforming SPI Flash Program Action: ");
    
    if (error_code == DPE_SUCCESS)
    {
        dp_check_image_address_and_size();
        
        if (error_code == DPE_SUCCESS)
        {
            dp_S25F_erase();
            bytes_processed = 0u;
            
            if (error_code == DPE_SUCCESS)
            {
                #ifdef ENABLE_DISPLAY
                old_progress = 0;
                dp_display_text("\r\nProgramming... ");
                #endif
                DataIndex = 0;
                do {
                    page_buffer_ptr = dp_get_data(Header_ID, DataIndex * 8u);
                    if (return_bytes > image_size )
                        return_bytes = image_size;
                    
                    // Max buffer size should be multiple of 512 bytes which is the minimum page size.
                    S25F_program_memory(spi_target_address + DataIndex, return_bytes, page_buffer_ptr);
                    if (error_code != DPE_SUCCESS)
                        break;
                    DataIndex += return_bytes;
                    
                } while (DataIndex < image_size);
            }
        }
    }
    return;
}

void dp_S25F_erase(void)
{
    DPULONG number_of_sectors_to_erase;  
    DPULONG address_to_process;
    
    dp_display_text("\r\nSPI Flash memory region to erase: 0x");
    dp_display_value(spi_target_address, HEX);
    dp_display_text(" - 0x");
    dp_display_value(spi_target_address + image_size - 1u , HEX);
    dp_display_text(". Please wait...\r\n");
    
    // Only do this if the starting address is not sector aligned
    if ((spi_target_address % sector_byte_size) > 0u)
    {
        dp_display_text("\r\nWarning: SPI target address is not sector aligned.  Data in the entire sector will be erased. "); 
    }
    
    address_to_process = spi_target_address;
    number_of_sectors_to_erase = (DPULONG)((spi_target_address % sector_byte_size + image_size + sector_byte_size - 1) / sector_byte_size);
    if (error_code == DPE_SUCCESS)
    {
        while (number_of_sectors_to_erase)
        {
            S25F_sector_erase(address_to_process);
            address_to_process += sector_byte_size;
            number_of_sectors_to_erase--;          
            if (error_code != DPE_SUCCESS)
                break;
        }
    }
    
    return;
}

// SPI Flash memory specific functions.
void S25F_parse_idcode(void)
{
    spi_flash_memory_byte_size = 0;
    switch (spi_flash_memory_size_id)
    {
        case S25F_128MB_BYTE_SIZE_ID:
        dp_display_text("\r\n3 byte address mode is selected");
        address_mode = ADDRESS_3BYTE_MODE;
        page_byte_size = S25F_PAGE_256_BYTE_SIZE;
        sector_byte_size = S25F_SECTOR_64K_BYTE_SIZE;
        spi_flash_memory_byte_size = N128MBIT_BYTE_SIZE;
        break;      
        case S25F_256MB_BYTE_SIZE_ID:
        address_mode = ADDRESS_4BYTE_MODE;
        page_byte_size = S25F_PAGE_256_BYTE_SIZE;
        sector_byte_size = S25F_SECTOR_64K_BYTE_SIZE;
        spi_flash_memory_byte_size = N256MBIT_BYTE_SIZE;
        break;      
        case S25F_512MB_BYTE_SIZE_ID:
        address_mode = ADDRESS_4BYTE_MODE;
        page_byte_size = S25F_PAGE_512_BYTE_SIZE;
        sector_byte_size = S25F_SECTOR_256K_BYTE_SIZE;
        spi_flash_memory_byte_size = N512MBIT_BYTE_SIZE;
        break;
    }
}

void S25F_reset(void)
{
    spi_scan(S25F_RESET_ENABLE, 0, DPNULL, DPNULL);
}


DPUCHAR S25F_busy_wait(void)
{
    DPUCHAR status_register;
    DPULONG timeout = 0;
    
    do {
        status_register = S25F_read_status_register();
        if (timeout++ > TIMEOUT_MAX_VALUE)
        {
            dp_display_text("\r\nError: Time out polling error detected.");
            error_code = DPE_SPI_FLASH_TIMEOUT_ERROR;
            break;
        }
    } while ((status_register & 0x1) == 0x1);
    
    return status_register;
}

void S25F_clear_status_register(void)
{
    spi_scan(S25F_CLEAR_STATUS_REGISTER_CMD, 0, DPNULL, DPNULL);
}

DPUCHAR S25F_read_status_register(void)
{
    DPUCHAR data_out = 0;
    spi_scan(S25F_READ_STATUS_REGISTER_CMD, 1, DPNULL, &data_out);
    
    return data_out;
}

DPUCHAR S25F_read_status_register2(void)
{
    DPUCHAR data_out = 0;
    spi_scan(S25F_READ_STATUS_REGISTER2_CMD, 1, DPNULL, &data_out);
    
    return data_out;
}


DPUCHAR S25F_read_configuration_register(void)
{
    DPUCHAR data_out = 0;
    spi_scan(S25F_READ_CONFIGURATION_REGISTER_CMD, 1, DPNULL, &data_out);
    
    return data_out;
}


void S25F_write_status_register(DPUCHAR data_in)
{
    S25F_write_enable();
    spi_scan(S25F_WRITE_STATUS_REGISTER_CMD, 1, &data_in, DPNULL);
    S25F_busy_wait();
}

DPUCHAR S25F_read_bank_address_register(void)
{
    DPUCHAR data_out = 0;
    spi_scan(S25F_READ_BANK_ADDRESS_REGISTER_CMD, 1, DPNULL, &data_out);
    
    return data_out;
}

void S25F_write_bank_address_register(DPUCHAR data_in)
{
    S25F_write_enable();
    spi_scan(S25F_WRITE_BANK_ADDREES_REGISTER_CMD, 1, &data_in, DPNULL);
    S25F_busy_wait();
}

void S25F_write_enable(void)
{
    spi_scan(S25F_WRITE_ENABLE_CMD, 0, DPNULL, DPNULL);
}

void S25F_write_disable(void)
{
    spi_scan(S25F_WRITE_DISABLE_CMD, 0, DPNULL, DPNULL);
}

void S25F_die_erase(void)
{
    DPUCHAR status_register;
    dp_display_text("\r\nPerforming die erase. Please wait...");
    S25F_write_enable();
    spi_scan(S25F_DIE_ERASE_CMD, 0, DPNULL, DPNULL);
    status_register = S25F_busy_wait();
    if (error_code == DPE_SUCCESS)
    {
        if (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE3_ID)
        {
            status_register = S25F_read_status_register2();
            if ((status_register & SR2_ERASE_ERROR_BIT) == SR2_ERASE_ERROR_BIT)
            {
                dp_display_text("\nError: Failed to erase SPI Flash.");
                error_code = DPE_SPI_FLASH_ERASE_ERROR;
            }          
        }        
        else
        {
            if ((status_register & SR1_ERASE_ERROR_BIT) == SR1_ERASE_ERROR_BIT)
            {
                dp_display_text("\nError: Failed to erase SPI Flash.");
                error_code = DPE_SPI_FLASH_ERASE_ERROR;
            }          
        }
    }
}

void S25F_sector_erase(DPULONG start_address)
{
    DPUCHAR index;
    DPUCHAR address[4] ={0x0, 0x0, 0x0, 0x0};
    DPUCHAR status_register;
    for (index = 0; index < address_mode; index++)
    {
        address[index] = (start_address >> ((address_mode - 1 - index) * 8)) & 0xff;
    }
    enable_cs();
    spi_shift_byte_in(S25F_WRITE_ENABLE_CMD);
    disable_cs();
    spi_scan(S25F_SECTOR_ERASE_CMD, address_mode, address, DPNULL);
    status_register = S25F_busy_wait();
    if (error_code == DPE_SUCCESS)
    {
        if (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE3_ID)
        {
            status_register = S25F_read_status_register2();
            if ((status_register & SR2_ERASE_ERROR_BIT) == SR2_ERASE_ERROR_BIT)
            {
                dp_display_text("\nError: Failed to erase SPI Flash.");
                error_code = DPE_SPI_FLASH_ERASE_ERROR;
            }          
        }    
        else
        {
            if ((status_register & SR1_ERASE_ERROR_BIT) == SR1_ERASE_ERROR_BIT)
            {
                dp_display_text("\nError: Failed to erase SPI Flash.");
                error_code = DPE_SPI_FLASH_ERASE_ERROR;
            }          
        }
    }
}

// Start programming address must be subsector aligned which is page aligned.
void S25F_program_memory(DPULONG start_address, DPULONG number_of_bytes, DPUCHAR *data)
{
    DPULONG index;
    DPUCHAR address[4] ={0x0, 0x0, 0x0, 0x0};
    DPULONG page_bytes;
    DPULONG processed_page_bytes = 0;
    DPUCHAR status_register;
    
    if (error_code == DPE_SUCCESS)
    {
        // Align the data with the first page by only programming valid data in page
        page_bytes = page_byte_size - start_address % page_byte_size;
        if (page_bytes > number_of_bytes)
            page_bytes = number_of_bytes;
        
        do {
            S25F_write_enable();
            enable_cs();
            spi_shift_byte_in(S25F_PAGE_PROGRAM_CMD);
            
            for (index = 0; index < address_mode; index++)
            {
                address[index] = (start_address >> ((address_mode - 1 - index) * 8)) & 0xff;
                spi_shift_byte_in(address[index]);
            }
            
            for (index = processed_page_bytes; index < (processed_page_bytes + page_bytes); index++)
            {
                spi_shift_byte_in(data[index]);
            }
            disable_cs();
            status_register = S25F_busy_wait();
            if(error_code == DPE_SUCCESS)
            {
                if (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE3_ID)
                {
                    status_register = S25F_read_status_register2();
                    if ((status_register & SR2_PROGRAM_ERROR_BIT) == SR2_PROGRAM_ERROR_BIT)
                    {
                        dp_display_text("\nError: Failed to program SPI Flash.");
                        error_code = DPE_SPI_FLASH_PROGRAM_ERROR;
                        break;                                    
                    }          
                }    
                else
                {
                    if ((status_register & SR1_PROGRAM_ERROR_BIT) == SR1_PROGRAM_ERROR_BIT)
                    {
                        dp_display_text("\nError: Failed to program SPI Flash.");
                        error_code = DPE_SPI_FLASH_PROGRAM_ERROR;
                        break;                                    
                    }          
                }              
            }
            else
            {
                break;
            }
            number_of_bytes -= page_bytes;
            start_address += page_bytes;
            bytes_processed += page_bytes;
            processed_page_bytes += page_bytes;
            
            #ifdef ENABLE_DISPLAY
            new_progress = (bytes_processed * 100u / image_size );
            if (new_progress != old_progress)
            {
                dp_report_progress(new_progress);
                old_progress = new_progress;
            }
            #endif
            page_bytes = number_of_bytes;
            if (page_bytes > page_byte_size)
                page_bytes = page_byte_size;
            
        } while (number_of_bytes > 0);
    }
}

void S25F_enable_4byte_address_mode(void)
{
    spi_scan(S25F_ENABLE_4BYTE_ADDRESS_CMD, 0, DPNULL, DPNULL);
}

#endif /* ENABLE_SPI_FLASH_SUPPORT */
