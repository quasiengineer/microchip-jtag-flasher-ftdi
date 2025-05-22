/* ************ MICROSEMI SOC CORP. DIRECTC LICENSE AGREEMENT ************* */
/* ------------------------------------------------------------------------ 
PLEASE READ: BEFORE INSTALLING THIS SOFTWARE, CAREFULLY READ THE FOLLOWING 
MICROSEMI SOC CORP LICENSE AGREEMENT REGARDING THE USE OF THIS SOFTWARE. 
INSTALLING THIS SOFTWARE INDICATES THAT YOU ACCEPT AND UNDERSTAND THIS AGREEMENT 
AND WILL ABIDE BY IT. 

Note: This license agreement (�License�) only includes the following software: 
DirectC. DirectC is licensed under the following terms and conditions.

Hereinafter, Microsemi SoC Corp. shall be referred to as �Licensor� or �Author,� 
whereas the other party to this License shall be referred to as �Licensee.� Each 
party to this License shall be referred to, singularly, as a �Party,� or, 
collectively, as the �Parties.�

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
shall obtain Licensor�s written consent prior to submitting any request for 
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
/*  Module:         dpSPIalg.c                                              */
/*                                                                          */
/*  Description:    Contains common SPI-Flash functions.                    */
/*                                                                          */
/* ************************************************************************ */
#include "dpuser.h"
#ifdef ENABLE_SPI_FLASH_SUPPORT

#include <stddef.h>

#include "dputil.h"
#include "dpcom.h"
#include "dpjtag.h"
#include "dpSpiProg.h"
#include "dpSPIalg.h"
#include "dpalg.h"
#include "dpS25F.h"

DPULONG spi_target_address = 0;
unsigned long long spi_flash_memory_byte_size;
unsigned long long bytes_processed;
DPUCHAR id_buf[3] = {0, 0, 0};
DPUCHAR spi_flash_vendor_id = 0;
DPUCHAR spi_flash_memory_type_id = 0;
DPUCHAR spi_flash_memory_size_id = 0;
DPUCHAR address_mode = ADDRESS_3BYTE_MODE;
DPUINT page_byte_size = 0;
DPULONG sector_byte_size = 0;


DPUCHAR dp_top_spi_flash (void)
{
    error_code = DPE_SUCCESS;
    dp_init_com_vars();
    Action_done = FALSE;
    
    init_spiprog_port();
    dp_SPI_read_idcode();
    
    if ((spi_flash_vendor_id == CYPRESS_VENDOR_ID) && 
    ((spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE1_ID) || (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE2_ID) || (spi_flash_memory_type_id == CYPRESS_MEMORY_TYPE3_ID)))             
    {
        dp_display_text("\r\nCypress S25F device is found.");
        dp_display_text("\r\nSPI-Flash IDCode (HEX) = ");
        dp_display_array(id_buf, 3, HEX);
        S25F_parse_idcode();
        if (spi_flash_memory_byte_size > 1048576)
        {
            dp_display_text("\r\nDevice size (MBytes) = ");
            dp_display_value(spi_flash_memory_byte_size / 1048576, DEC);
        }
        else
        {
            dp_display_text("\r\nDevice size (KBytes) = ");
            dp_display_value(spi_flash_memory_byte_size / 1024, DEC);
        }
        if (spi_flash_memory_byte_size == 0)
        {
            dp_display_text("\r\nError: Failed to recognize device density.");
            error_code = DPE_IDCODE_ERROR;
        }
        else
        {
            dp_top_S25F();
            Action_done = TRUE;
        }
    }
    if (Action_done == FALSE)
    {
        dp_display_text("\r\nError: SPI-Flash is not connected or not supported.");
        error_code = DPE_IDCODE_ERROR;
    }
    
    return error_code;
}

void dp_check_image_address_and_size(void)
{
    if ((spi_target_address + image_size) > spi_flash_memory_byte_size)
    {
        dp_display_text("\r\nError: Image byte size is greater than available memory space.");
        dp_display_text("\r\nSPI Target Address + Image size: ");
        dp_display_value((spi_target_address + image_size), DEC);
        dp_display_text("\r\nAvailable memory space: ");
        dp_display_value(spi_flash_memory_byte_size, DEC);
        error_code = DPE_IMAGE_SIZE_ERROR;
    }
}

void dp_SPI_read_idcode(void)
{
    spi_scan(SPI_READ_ID_CMD, 3u, NULL, id_buf);
    
    spi_flash_vendor_id = id_buf[0];
    spi_flash_memory_type_id = id_buf[1];
    spi_flash_memory_size_id = id_buf[2];
    return;
}

void dp_SPI_blank_check_action(void)
{
    dp_display_text("\r\nPerforming SPI Flash Blank Check Action:\r\n");
    bytes_processed = 0u;
    SPI_blank_check_memory(0u, spi_flash_memory_byte_size);
    
    return;
}

void dp_SPI_read_action(void)
{
    DPULONG bytes_read = 0u; // This is used to keep track of how many bytes read and also used as the starting address to read
    DPULONG bytes_to_read = 0u;
    
    dp_display_text("\r\nPerforming SPI Flash Read Action:\r\n");
    
    // Image_size contains the data in ddr including header.
    while (bytes_read < (image_size))
    {
        bytes_to_read = image_size - bytes_read;
        if (bytes_to_read > PAGE_BUFFER_SIZE)
            bytes_to_read = PAGE_BUFFER_SIZE;
        
        SPI_read_memory(bytes_read, bytes_to_read);
        bytes_read += bytes_to_read;
        dp_display_array_reverse(page_global_buffer, bytes_to_read, HEX);
    }
    
    return;
}

void dp_SPI_verify_action(void)
{
    dp_display_text("\r\nPerforming SPI Flash Verify Action: ");
    
    bytes_processed = 0u;
    
    if (error_code == DPE_SUCCESS)
    {
        DataIndex = 0;
        dp_display_text("\r\nVerifying image from address = 0x");
        dp_display_value(spi_target_address + DataIndex, HEX);
        dp_display_text(" - 0x");
        dp_display_value(spi_target_address + DataIndex + image_size - 1u , HEX);
        
        do {
            page_buffer_ptr = dp_get_data(Header_ID, DataIndex * 8u);
            if (return_bytes > image_size )
                return_bytes = image_size;
            
            SPI_verify_memory(spi_target_address + DataIndex, return_bytes, page_buffer_ptr);
            if (error_code != DPE_SUCCESS)
                break;
            DataIndex += return_bytes;
            
        } while (DataIndex < image_size);
    }
    return;
}

void SPI_blank_check_memory(DPULONG start_address, DPULONG number_of_bytes)
{
    DPULONG index;
    DPUCHAR data;
    DPUCHAR address[4];
    
    enable_cs();
    spi_shift_byte_in(SPI_READ);
    
    for (index = 0; index < address_mode; index++)
    {
        address[index] = (start_address >> ((address_mode - 1 - index) * 8)) & 0xff;
        spi_shift_byte_in(address[index]);
    }
    spi_shift_dummy_bit();
    
    for (index = 0; index < number_of_bytes; index++)
    {
        spi_shift_byte_out(&data);
        if (data != 0xffu)
        {
            error_code = DPE_SPI_FLASH_BLANK_CHECK_ERROR;
            dp_display_text("\r\nError: SPI-Flash Is not blank: Address = 0x");
            dp_display_value((start_address + index), HEX);
            dp_display_text(" - Actual value = 0x");
            dp_display_value(data, HEX);
            break;
        }
        bytes_processed ++;
        #ifdef ENABLE_DISPLAY
        new_progress = (DPUCHAR)(bytes_processed * 100 / spi_flash_memory_byte_size);
        if (new_progress != old_progress)
        {
            dp_report_progress(new_progress);
            old_progress = new_progress;
        }
        #endif
        
    }
    disable_cs();
}

void SPI_read_memory(DPULONG start_address, DPULONG number_of_bytes)
{
    DPULONG index;
    DPUCHAR data;
    DPUCHAR address[4];
    
    dp_display_text("\r\nReading data at address: 0x");
    dp_display_value(start_address, HEX);
    dp_display_text(" - 0x");
    dp_display_value(start_address + number_of_bytes - 1u , HEX);        
    dp_display_text("\r\n");
    
    
    enable_cs();
    spi_shift_byte_in(SPI_READ);
    
    for (index = 0; index < address_mode; index++)
    {
        address[index] = (start_address >> ((address_mode - 1 - index) * 8)) & 0xff;
        spi_shift_byte_in(address[index]);
    }
    spi_shift_dummy_bit();
    
    for (index = 0; index < number_of_bytes; index++)
    {
        spi_shift_byte_out(&data);
        page_global_buffer[index] = data;
    }
    disable_cs();
}

void SPI_verify_memory(DPULONG start_address, DPULONG number_of_bytes, DPUCHAR *data)
{
    DPULONG index;
    DPUCHAR actual_data;
    DPUCHAR address[4];
    
    enable_cs();
    spi_shift_byte_in(SPI_READ);
    
    for (index = 0; index < address_mode; index++)
    {
        address[index] = (start_address >> ((address_mode - 1 - index) * 8)) & 0xff;
        spi_shift_byte_in(address[index]);
    }
    spi_shift_dummy_bit();
    for (index = 0; index < number_of_bytes; index++)
    {
        spi_shift_byte_out(&actual_data);
        if (data[index] != actual_data)
        {
            error_code = DPE_SPI_FLASH_VERIFY_ERROR;
            dp_display_text("\r\nError: SPI-Flash verify operation failed at address: 0x");
            dp_display_value((start_address + index), HEX);
            dp_display_text(" - Expected value = 0x");
            dp_display_value(data[index], HEX);
            dp_display_text(" - Actual value = 0x");
            dp_display_value(actual_data, HEX);
            break;
        }
        
        bytes_processed ++;
        #ifdef ENABLE_DISPLAY
        new_progress = (bytes_processed * 100u / image_size);
        if (new_progress != old_progress)
        {
            dp_report_progress(new_progress);
            old_progress = new_progress;
        }
        #endif
    }
    disable_cs();
}
#endif
/* *************** End of File *************** */

