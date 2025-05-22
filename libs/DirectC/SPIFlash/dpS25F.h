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
/*  Module:         dpS25F.h                                                */
/*                                                                          */
/*  Description:    Contains S25F specific SPI-Flash function prototypes.   */
/*                                                                          */
/* Device Manufacturer: Cypress / Spansion                                  */
/* ************************************************************************ */

#ifndef INC_DPS25L_H
#define INC_DPS25L_H

#define S25F_RESET_ENABLE									    0xF0u

#define S25F_WRITE_ENABLE_CMD									0x06u
#define S25F_WRITE_DISABLE_CMD									0x04u

#define S25F_READ_STATUS_REGISTER_CMD						    0x05u
#define S25F_WRITE_STATUS_REGISTER_CMD  					    0x01u
#define S25F_CLEAR_STATUS_REGISTER_CMD  					    0x30u

#define S25F_READ_STATUS_REGISTER2_CMD						    0x07u
#define S25F_READ_CONFIGURATION_REGISTER_CMD				    0x35u

#define S25F_READ_BANK_ADDRESS_REGISTER_CMD                     0x16u   
#define S25F_WRITE_BANK_ADDREES_REGISTER_CMD                    0x17u

#define S25F_PAGE_PROGRAM_CMD									0x02u
#define S25F_SECTOR_ERASE_CMD								    0xD8u
#define S25F_DIE_ERASE_CMD										0xC7u
#define S25F_ENABLE_4BYTE_ADDRESS_CMD                           0xB7u


#define CYPRESS_VENDOR_ID             0x01u 
#define CYPRESS_MEMORY_TYPE1_ID       0x02u // 512 Mb Sample device
#define CYPRESS_MEMORY_TYPE2_ID       0x20u // 128 Mb Sample device
#define CYPRESS_MEMORY_TYPE3_ID       0x60u // 256 Mb Sample device

#define S25F_128MB_BYTE_SIZE_ID       0x18u
#define S25F_256MB_BYTE_SIZE_ID       0x19u
#define S25F_512MB_BYTE_SIZE_ID       0x20u

#define S25F_PAGE_256_BYTE_SIZE       256u
#define S25F_PAGE_512_BYTE_SIZE       512u

#define S25F_SECTOR_64K_BYTE_SIZE     65536u
#define S25F_SECTOR_256K_BYTE_SIZE    262144u

#define SR1_ERASE_ERROR_BIT               0x20u
#define SR1_PROGRAM_ERROR_BIT             0x40u

#define SR2_ERASE_ERROR_BIT               0x40u
#define SR2_PROGRAM_ERROR_BIT             0x20u


DPUCHAR dp_top_S25F(void);
void dp_perform_S25F_action(void);
void dp_S25F_erase_action(void);
void dp_S25F_image_erase_action(void);
void dp_S25F_program_action(void);
void dp_S25F_erase(void);

void S25F_parse_idcode(void);
void S25F_reset(void);
DPUCHAR S25F_busy_wait(void);
void S25F_clear_status_register(void);
DPUCHAR S25F_read_status_register(void);
DPUCHAR S25F_read_status_register2(void);
DPUCHAR S25F_read_configuration_register(void);
void S25F_write_status_register(DPUCHAR data_in);
DPUCHAR S25F_read_bank_address_register(void);
void S25F_write_bank_address_register(DPUCHAR data_in);
void S25F_write_enable(void);
void S25F_write_disable(void);
void S25F_enable_4byte_address_mode(void);

void S25F_die_erase(void);
void S25F_sector_erase(DPULONG start_address);
void S25F_program_memory(DPULONG start_address, DPULONG number_of_bytes, DPUCHAR *data);
#endif
