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
/*  Module:         dpSpiProg.c                                             */
/*                                                                          */
/*  Description:    Contains SPIPROG related functions                      */
/*                                                                          */
/* ************************************************************************ */

#include "dpuser.h"
#ifdef ENABLE_SPI_FLASH_SUPPORT
#include "dputil.h"
#include "dpcom.h"
#include "dpalg.h"
#include "dpSpiProg.h"
#include "dpjtag.h"

/* See bit definition in dpSPI.h.
The hardware can automatically generate the SPI clock to reduce the vector cound by two.
This takes place when JTAG state machine goes through the UPDATE state.  
Therefore, goto_jtag(IDLE_STATE) is required. */

DPUCHAR spiprog_reg = 0x0u;
DPUCHAR spiprog_reg_out = 0x0u;

void init_spiprog_port(void)
{
    opcode = G5M_SPIPROG;
    // Set the initial values prior to enabling the instruction
    spiprog_reg = DISABLE_SPIPROG_INSTRUCTION | SLVSEL_HIGH | SPI_CLOCK_INACTIVE_STATE_LOW | SPI_SAMPLE_ON_POSTIVE_EDGE;
    IRSCAN_in(); // Should only be required once.
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
    
    // Enable the instruction
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_HIGH | SPI_CLOCK_INACTIVE_STATE_LOW | SPI_SAMPLE_ON_POSTIVE_EDGE;
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
    
    return;
}

void disable_spiprog_port(void)
{
    // Set the initial values prior to enabling the instruction
    spiprog_reg = DISABLE_SPIPROG_INSTRUCTION | SLVSEL_HIGH | SPI_CLOCK_INACTIVE_STATE_LOW | SPI_SAMPLE_ON_POSTIVE_EDGE;
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
    
    return;
}

void enable_cs(void)
{
    opcode = G5M_SPIPROG;
    // Set the initial values prior to enabling the instruction
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_LOW | SPI_CLOCK_INACTIVE_STATE_LOW | SPI_SAMPLE_ON_POSTIVE_EDGE;
    IRSCAN_in();
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
}

void disable_cs(void)
{
    opcode = G5M_SPIPROG;
    // Set the initial values prior to enabling the instruction
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_HIGH | SPI_CLOCK_INACTIVE_STATE_LOW | SPI_SAMPLE_ON_POSTIVE_EDGE;
    IRSCAN_in();
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
}

void spi_shift_dummy_bit(void)
{
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_LOW | SPI_CLOCK_TOGGLE | SPI_SAMPLE_ON_POSTIVE_EDGE;
    DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,0);
    return;
}

void spi_scan(DPUCHAR command, DPULONG total_bytes, DPUCHAR *data_in, DPUCHAR *data_out)
{
    DPULONG index;
    enable_cs();
    spi_shift_byte_in(command);
    if (data_in != DPNULL)
    {
        
        for (index = 0; index < total_bytes; index++)
        {
            spi_shift_byte_in(data_in[index]);
        }
    }
    if (data_out != DPNULL)
    {
        spi_shift_dummy_bit();
        for (index = 0; index < total_bytes; index++)
        {
            spi_shift_byte_out(&data_out[index]);
        }
    }
    disable_cs();
}

void spi_shift_byte_in(DPUCHAR byte_in)
{
    DPUCHAR index;
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_LOW | SPI_CLOCK_TOGGLE | SPI_SAMPLE_ON_POSTIVE_EDGE;
    
    for (index = 0x80; index > 0; index >>= 1)
    {
        spiprog_reg &= 0xfd;
        if (index & byte_in)
        {
            spiprog_reg |= 0x2;
        }
        DRSCAN_in(0u, G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg);
        goto_jtag_state(JTAG_RUN_TEST_IDLE,0);
    }
}

void spi_shift_byte_out(DPUCHAR* byte_out)
{
    DPUCHAR index;
    
    spiprog_reg = ENABLE_SPIPROG_INSTRUCTION | SLVSEL_LOW | SPI_CLOCK_TOGGLE | SPI_SAMPLE_ON_POSTIVE_EDGE;
    
    *byte_out = 0;
    for (index = 0x80; index > 0; index >>= 1)
    {
        DRSCAN_out(G5M_SPIPROG_REGISTER_BIT_LENGTH, &spiprog_reg, &spiprog_reg_out);
        goto_jtag_state(JTAG_RUN_TEST_IDLE,0);
        
        if (spiprog_reg_out & 0x1u)
        {
            *byte_out |= index;
        }
    }
}
#endif /* ENABLE_SPI_FLASH_SUPPORT */

