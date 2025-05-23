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
/*  Module:         dpuser.h                                                */
/*                                                                          */
/*  Description:    DP user specific file                                   */
/*                  users should define their own functions                 */
/*                                                                          */
/* ************************************************************************ */
#ifndef INC_DPUSER_H
#define INC_DPUSER_H

#include <ftdi.h>

/* Compiler switches */
#define ENABLE_EMBEDDED_SUPPORT

#define ENABLE_DISPLAY
#define ENABLE_GPIO_SUPPORT
#define PERFORM_CRC_CHECK
#define ENABLE_SPI_FLASH_SUPPORT
#define ENABLE_G3_SUPPORT
#define ENABLE_G4_SUPPORT
#define ENABLE_G5_SUPPORT
/* 
Warning 1: RTG4 FPGA programming circuitry is not radiation hardened. 
Please refer to the latest data on reprogramming in radiation environments.
Warning 2:  Before programming, please make sure to exit the System Controller 
Suspend Mode, which is used to protect the device from unintended behavior 
due to single event upsets (SEUs) in radiation environment.
*/
#define ENABLE_RTG4_SUPPORT

// #define USE_PAGING
/* #define CHAIN_SUPPORT */
/* Enable BSR_SAMPLE switch maintains the last known state of the IOs regardless 
*  of the data file setting. */
/* #define BSR_SAMPLE */

/*************** End of compiler switches ***********************************/


/*************** Hardware related constants *****************************/
/*
* User Attention: 
* Bit assignments in the hardware JTAG port register
* TCK - D0
* TDI - D1
* TDO - D2
* TMS - D3
* TRST - D7
*/
#define TCK     0x1u
#define TDI     0x2u
#define TDO     0x4u
#define TMS     0x8u
#define TRST    0x80u
/*************** End of hardware related constants ************************/



/***********************************************/
/* DPCHAR    -- 8-bit Windows (ANSI) character */
/*              i.e. 8-bit signed integer      */
/* DPINT     -- 16-bit signed integer          */
/* DPLONG    -- 32-bit signed integer          */
/* DPBOOL    -- boolean variable (0 or 1)      */
/* DPUCHAR   -- 8-bit unsigned integer         */
/* DPUSHORT  -- 16-bit unsigned integer        */
/* DPUINT    -- 16-bit unsigned integer        */
/* DPULONG   -- 32-bit unsigned integer        */
/***********************************************/
typedef unsigned char  DPUCHAR;
typedef unsigned short DPUSHORT;
typedef unsigned int   DPUINT;
typedef unsigned long  DPULONG;
typedef unsigned long long DPULONGLONG;
typedef unsigned char  DPBOOL;
typedef   signed char  DPCHAR;
typedef   signed int   DPINT;
typedef   signed long  DPLONG;

#define DPNULL ((void*)0)
#define TRUE 1U
#define FALSE 0U

#define GPIO_SEL 1u
#define IAP_SEL 2u

extern DPUCHAR *image_buffer;
extern DPUCHAR hardware_interface;
extern DPUCHAR enable_mss_support;
extern struct ftdi_context ftdi;


void dp_exit_avionics_mode(void);
void dp_delay(DPULONG microseconds);

#ifdef ENABLE_EMBEDDED_SUPPORT
DPUCHAR jtag_inp(void);
void jtag_outp(DPUCHAR outdata);
void dp_jtag_init(void);
void dp_jtag_tms(DPUCHAR tms);
void dp_jtag_tms_tdi(DPUCHAR tms, DPUCHAR tdi);
DPUCHAR dp_jtag_tms_tdi_tdo(DPUCHAR tms, DPUCHAR tdi);
/* User defined */
void configure_MSSIO(void);
/* End of User defined */
#endif


#ifdef ENABLE_DISPLAY
#define HEX 0u
#define DEC 1u
#define CHR 2u

/******************************************************************************/
/* users should define their own functions to replace the following functions */
/******************************************************************************/
void dp_display_text(char * text);
void dp_display_value(DPULONG value,DPUINT descriptive);
void dp_display_array(DPUCHAR *value,DPUINT bytes, DPUINT descriptive);
void dp_display_array_reverse(DPUCHAR *outbuf,DPUINT bytes, DPUINT descriptive);
void dp_report_progress(DPUCHAR value);

#endif

#endif /* INC_DPUSER_H */

/* *************** End of File *************** */
