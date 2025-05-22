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
/*  Module:         dpSPIalg.h                                              */
/*                                                                          */
/*  Description:    Contains common SPI-Flash function prototypes.          */
/*                                                                          */
/* ************************************************************************ */
#ifndef INC_DPSPIALG_H
#define INC_DPSPIALG_H

/* Programming method.  Common to all families */
extern DPULONG spi_target_address;
extern DPULONGLONG spi_flash_memory_byte_size;
extern DPULONGLONG bytes_processed;
extern DPUCHAR id_buf[3];
extern DPUCHAR spi_flash_vendor_id;
extern DPUCHAR spi_flash_memory_type_id;
extern DPUCHAR spi_flash_memory_size_id;
extern DPUCHAR address_mode;
extern DPUINT page_byte_size;
extern DPULONG sector_byte_size;


// SPI Flash memory specific actions/
#define DP_SPI_FLASH_READ_ID_ACTION_CODE	     40u
#define DP_SPI_FLASH_READ_ACTION_CODE            41u
#define DP_SPI_FLASH_PARTIAL_READ_ACTION_CODE    42u
#define DP_SPI_FLASH_ERASE_ACTION_CODE           43u
#define DP_SPI_FLASH_PARTIAL_ERASE_ACTION_CODE   44u
#define DP_SPI_FLASH_PROGRAM_ACTION_CODE         45u
#define DP_SPI_FLASH_VERIFY_ACTION_CODE          46u
#define DP_SPI_FLASH_BLANK_CHECK_ACTION_CODE     47u
#define DP_SPI_FLASH_ERASE_IMAGE_ACTION_CODE     48u

#define N4MBIT_BYTE_SIZE                0x80000u
#define N8MBIT_BYTE_SIZE                0x100000u
#define N16MBIT_BYTE_SIZE               0x200000u
#define N32MBIT_BYTE_SIZE               0x400000u
#define N64MBIT_BYTE_SIZE               0x800000u
#define N128MBIT_BYTE_SIZE              0x1000000u
#define N256MBIT_BYTE_SIZE              0x2000000u
#define N512MBIT_BYTE_SIZE              0x4000000u
#define N1GBIT_BYTE_SIZE                0x8000000u
#define N2GBIT_BYTE_SIZE                0x10000000u

#define ADDRESS_3BYTE_MODE              3u
#define ADDRESS_4BYTE_MODE              4u
#define TOP_SECTOR                      0u
#define BOTTOM_SECTOR                   1u

#define DPE_SPI_FLASH_DISABLE_SECURITY_ERROR                    200u
#define DPE_SPI_FLASH_NVCR_ERROR                                201u
#define DPE_SPI_FLASH_ERASE_ERROR                               202u
#define DPE_SPI_FLASH_PROGRAM_ERROR                             203u
#define DPE_SPI_FLASH_VERIFY_ERROR                              204u
#define DPE_SPI_FLASH_BUFFER_SIZE_ERROR                         205u
#define DPE_SPI_FLASH_TIMEOUT_ERROR                             206u
#define DPE_SPI_FLASH_INVALID_IMAGE_ERROR                       207u
#define DPE_ADDRESS_BOUNDARY_ERROR                              208u
#define DPE_IMAGE_SIZE_ERROR                                    209u
#define DPE_SPI_FLASH_BLANK_CHECK_ERROR                         210u

#define TIMEOUT_MAX_VALUE                                       0x1000000u

// Common SPI-Flash opcodes
#define SPI_READ											    0x03u
#define SPI_READ_ID_CMD                                         0x9fu

void dp_check_image_address_and_size(void);

DPUCHAR dp_top_spi_flash(void);
void dp_SPI_read_idcode(void);
void dp_SPI_blank_check_action(void);
void dp_SPI_read_action(void);
void dp_SPI_verify_action(void);
void SPI_blank_check_memory(DPULONG start_address, DPULONG number_of_bytes);
void SPI_read_memory(DPULONG start_address, DPULONG number_of_bytes);
void SPI_verify_memory(DPULONG start_address, DPULONG number_of_bytes, DPUCHAR *data);
#endif /* INC_DPSPIALG_H */

/* *************** End of File *************** */
