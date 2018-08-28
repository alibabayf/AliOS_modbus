/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _SPP_GSM_H_
#define _SPP_GSM_H_


#include "cs_types.h"
#include "gsm.h"
#if (SPC_IF_VER == 0)
#include "spp_gsm_v5.h"
#elif (SPC_IF_VER == 1)
#include "spp_gsm_v5.h"
#elif (SPC_IF_VER == 2)
#include "spp_gsm_v5.h"
#elif (SPC_IF_VER == 3)
#include "spp_gsm_v5.h"
#elif (SPC_IF_VER == 4)
#include "spp_gsm_v5.h"
#elif (SPC_IF_VER == 5)
#include "spp_gsm_v5.h"
#else
#include "spp_gsm_std.h"
#endif

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================




//-------------------
// SPP IR structures
//-------------------

typedef union
{
    struct
    {
        UINT32 bsn : 11;  // BSN number
        UINT32 mod : 1;   // Modulation (1 GMSK, 0 8PSK)
        UINT32 mcs : 4;   // MCS (+ SPB for GMSK only)
    };

    UINT16 ID;

} SPP_HEADER_CTX_T;



typedef union
{
    struct
    {
        UINT32 bsnresi  : 5;   // residuel BSN number
        UINT32 spb      : 2;   // SPB for GMSK only
        UINT32 next     : 9;   // index of next bock
    };

    UINT16 ID_H;

} SPP_EGPRS_IR_HASH;






#ifdef CHIP_DIE_8955
typedef struct
{
    UINT32 *samples; // ptr to received symbols
    UINT32 *ref;     // ptr to reconstructed symbols
    UINT32 taps[5]; // channel taps
    UINT8  *sv;   // pointer to equalized softvalues
    UINT32 power; // normalized power (/symbol)
    UINT32 noise; // normalized noise power (/symbol)
    SPP_COMPLEX_T dco; // DC Offset level
    INT16 fof;
    UINT8  tof;
    //UINT8  type;
    UINT8  tsc;
    UINT8  samples_offset;
} SPP_EQU_BURST_T;
#endif

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

// SPP PUBLIC STRUCTURES/ ENUMS
///@defgroup gsm_struct GSM Structures and enums
///@{

//---------------
// SPP structures
//---------------

//volatile UINT8 decoded_out[5];

// Overlap Vitac Buffer Mapping (sequential use only)
typedef union
{
    SPP_NB_VITAC_T               NbVitac;
    SPP_SCH_VITAC_T              SchVitac;
    SPP_DECODE_VITAC_T           DecodeVitac;
    SPP_EGPRS_PREFILTER_BUFS_T   EgprsPrefilterBufs;
    SPP_EGPRS_DDFSE_BUFS_T       EgprsDdfseBufs;
    SPP_EGPRS_DECODE_VITAC_T     EgprsDecodeVitac;
} SPP_UNCACHED_BUFFERS_T;

//----------
// SPP Enums
//----------

///@}

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// SPP PUBLIC FUNCTIONS
///@defgroup gsm_rx_func Rx processing functions
///@{

//======================================================================
// FCCH Detection function
//----------------------------------------------------------------------
/// This function implements FCCH detection over large reception windows
/// (usually 9 Time Slots in @b sliced mode or 12 TDMA frames in @b Full
/// mode). For both modes, the reception window must be sliced in sub
/// parts. The function must be called for each sub part of the window
/// and will return an FCCH detection status each time. See FCCH
/// detection memory requirements.
/// @param RxBuffer UINT32*. Start address of the reception buffer.
/// @param Init BOOL. Takes following values:
/// - TRUE : first call for this FCCH window,
/// - FALSE: subsequent calls for a same FCCH window.
/// .
/// @param Mode SPP_FCCH_MODE_T. Takes following values:
/// - #FCCH_SLICED for detection during dedicated mode,
/// - #FCCH_FULL for initial cell syncronization process.
/// .
/// @param Res SPP_FCCH_RESULT_T*. Pointer to the FCCH Result structure.
///
/// @return Takes following values:
/// - #FCCH_FOUND if fcch has been detected (results valid),
/// - #FCCH_NOT_FOUND if no fcch has been detected,
/// - #FCCH_INVALID if fcch has been detected but conditions
/// of detection (window boundaries, suspect frequency error
/// ...) are imposing to discard the detection results.
/// .
//======================================================================
PUBLIC SPP_FCCH_RETURN_T spp_SearchFcch(UINT32* RxBuffer,
                                        BOOL Init,
                                        SPP_FCCH_MODE_T Mode,
                                        SPP_FCCH_DELAY_TYPE_T DelayType,
                                        SPP_FCCH_RESULT_T* Res);

PUBLIC SPP_FCCH_RETURN_T  spp_SearchFcchFilter(UINT32* RxBuffer,
        BOOL Init,
        SPP_FCCH_MODE_T Mode,
        SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetect(UINT32* Ptr_old,
                           UINT32* Ptr_new,
                           SPP_FCCH_PARAM_T* Params,
                           SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetectAsm(UINT32* Ptr_old,
                              UINT32* Ptr_new,
                              SPP_FCCH_PARAM_T* Params,
                              SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetectFilterAsm(UINT32* Ptr_old,
                                    UINT32* Ptr_new,
                                    SPP_FCCH_PARAM_T* Params,
                                    SPP_FCCH_RESULT_T* Res);

//======================================================================
// SCH Equalize and Decode function
//----------------------------------------------------------------------
/// Performs SCH burst equalization including:
/// - DC offset estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
/// @param RxBuffer : Pointer to the beginning of the RF samples buffer
/// @param UncachedBuffers : uncached working memory space
/// @param Res : Pointer to the SCH Result structure
//======================================================================
PUBLIC BOOL spp_EqualizeDecodeSch(UINT32* RxBuffer,
                                  SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                  SPP_SCH_RESULT_T* Res);

//======================================================================
// spp_EqualizeNBurst
//----------------------------------------------------------------------
/// Performs normal burst equalization including:
/// - DC offset estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
///
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param Tsc (UINT8): Training sequence number
/// @param EqBufferOut (UINT32*): Pointer to the equalized output
/// @param UncachedBuffers : uncached working memory space
/// @param Res (SPP_N_BURST_RESULT_T*): Pointer to the Normal Burst Result structure
//======================================================================
PUBLIC VOID spp_EqualizeNBurst(UINT32* RxBuffer,
                               UINT8 Tsc,
                               UINT32* EqBufferOut,
                               SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                               SPP_N_BURST_RESULT_T* Res);


#ifdef CHIP_DIE_8955
//=============================================================================
// spp_Equalize_saic
//-----------------------------------------------------------------------------
/// This function performs Viterbi equalization of a burst.
///
/// @param WinType UINT8. INPUT. Toggles channel estimation function to either
///                       normal burst or SCH.
/// @param burst SPP_EQU_BURST_T*. INPUT/OUTPUT Pointer to the burst structure.
/// @param RxTscAddr.UINT32*.INPUT Pointer the start address of Training sequence.
/// @param temsv UINT8. tempbuffer,is used to save the softvalues of whole burst.
/// @param UncachedBuffers SPP_UNCACHED_BUFFERS_T*. INPUT/OUTPUT. Pointer to
/// the work buffers designated by SPC layer.
//=============================================================================

PUBLIC VOID  spp_Equalize_saic(UINT8 WinType,
                               SPP_EQU_BURST_T* burst,
                               UINT8 temsv[],
                               SPP_UNCACHED_BUFFERS_T* UncachedBuffers);

//======================================================================
// spp_EqualizeNBurstSaicTest
//----------------------------------------------------------------------
/// Performs normal burst equalization including:
/// - DC offset estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
///
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param Tsc (UINT8): Training sequence number
/// @param EqBufferOut (UINT32*): Pointer to the equalized output
/// @param UncachedBuffers : uncached working memory space
/// @param Res (SPP_N_BURST_RESULT_T*): Pointer to the Normal Burst Result structure
//======================================================================
PUBLIC VOID spp_EqualizeNBurstSaic(UINT32* RxBuffer,
                                   UINT8 Tsc,
                                   UINT32* EqBufferOut,
                                   UINT32* EqBufferOutSaic,
                                   SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                   SPP_N_BURST_RESULT_T* Res);


//=============================================================================
// spp_DcocProcess
//-----------------------------------------------------------------------------
/// Dc canceliation and derotation.
///
/// @param RxBuffer UINT32*, INPUT.The start address of 1x data ,down sampling from 2x data.
/// @param size UINT8, INPUT.length of one burst data.
/// @param derotate BOOL. INPUT.derotation setting.
/// @param bypassDcoc BOOL.INPUT.remove dc value setting.
/// @param Dc_I INT16*,OUTPUT. real part of DC
/// @param  Dc_Q INT16*.OUTPUT.imag part of DC
/// @param Is2xMode (BOOL):INPUT.To indicate 1x rate mode or 2x rate mode,1: 2x mode. 0: default 1x mode.
/// @return UINT32. Return the normalized power of one burst data.
//=============================================================================

PUBLIC  UINT32 spp_DcocProcess(UINT32* RxBuffer,
                               UINT8 size,
                               BOOL derotate,
                               BOOL bypassDcoc,
                               INT16* Dc_I,
                               INT16* Dc_Q,
                               BOOL Is2xMode);

//=============================================================================
// spp_SaicPro
//-----------------------------------------------------------------------------
// Performs saic process step1
// @param RxBuffer UINT16*. Address of the 2x received samples.
// @param R_imag_vec UINT32*. Address of the 2x received samples,imaginary parts.
// @param M_vecstep2 INT16*. M matrix(S(H)*S),it is used for saic process2.
// @param Tsc UINT8,Training Sequence index.
// @param Y_SHIFT UINT8, Shift parameter.
// @param Out UINT32*. Samples filterd out of saic process1.
// @param Tap UINT32*.equavalent taps.
//=============================================================================

PUBLIC VOID  spp_SaicPro(INT16* RxBuffer, UINT32* R_imag_vec, INT16* M_vecstep2, UINT8 Tsc, UINT8 Y_SHIFT , UINT32 * Out, UINT32* Tap);
//=============================================================================
// spp_SaicProstep2
//-----------------------------------------------------------------------------
// Performs saic process step2
// @param RxBuffer UINT16*. Address of the 2x received samples.
// @param R_imag_vec UINT32*. Address of the 2x received samples,imag parts.
// @param M_vec INT16*. M matrix(S(H)*S),it is used for saic process2.
// @param Tsc UINT8,Training Sequence index.
// @param Y_SHIFT UINT8, Shift parameter.
// @param sv UINT8*. soft value of vitac output.
// @param sv_out UINT8*.soft value of saic process2.
//=============================================================================

PUBLIC VOID  spp_SaicProstep2(INT16* RxBuffer, UINT32* R_imag_vec, INT16* M_vec,UINT8 Tsc, UINT8 Y_SHIFT , UINT8* sv, UINT8* sv_out);


#endif

//======================================================================
// spp_DecodeStealingBits
//----------------------------------------------------------------------
/// Decode Stealing Flags of Normal Bursts.
///
/// @param RxBuffer UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param StealMode BOOL. Takes following values:
/// - TRUE for PDCH Steal flags type,
/// - FALSE for TCH Steal flags type.
/// .
/// @param StealingResult UINT8*. Output pointer.
/// @param Threshold INT16. Decision threshold (for TCH only).
//======================================================================
PUBLIC VOID spp_DecodeStealingBits(UINT32* RxBuffer,
                                   BOOL StealMode,
                                   UINT8* StealingResult,
                                   INT16 Threshold);

//======================================================================
// spp_DeinterleaveBlock
//----------------------------------------------------------------------
/// DeInterleaving of a data block
///
/// @param BufferIn UINT32*. INPUT. Pointer to equalized bursts buffer.
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 0 : TYPE_1A. Valid for TCH/FS, TCH/EFS, FACCH/F, TCH/F2.4,
/// TCH/AFS (speech, ratscch, sid_first).
/// - 1 : TYPE_1B. Valid for SACCH, SDCCH, TCH/AFS(sid_update), PDTCH(CS-1 to CS-4),
/// BCCH, PCH, AGCH, PACCH, PBCCH, PAGCH, PPCH, PNCH, PTCCH/D.
/// - 2 : TYPE_1C. Valid for FACCH/H.
/// - 3 : TYPE_2A. Valid for TCH/HS and TCH/AHS (speech, ratscch, sid_first).
/// - 4 : TYPE_2B. Valid for TCH/AHS (sid_update).
/// - 5 : TYPE_3. Valid for TCH/F14.4, TCH/F9.6, TCH/F4.8, TCH/H4.8
/// and TCH/H2.4.
/// - 6 : TYPE_H1. Valid for EGPRS HEADER TYPE2 in Rx.
/// - 7 : TYPE_H2. Valid for EGPRS HEADER TYPE2 in Tx.
/// - 8 : TYPE_H3. Valid for EGPRS HEADER TYPE1 in Rx.
/// - 9 : TYPE_H4. Valid for EGPRS HEADER TYPE1 in Tx.
/// - 10 : TYPE_D1. Valid for EGPRS MCS5/6.
/// - 11 : TYPE_D2. Valid for EGPRS MCS7.
/// - 12 : TYPE_D3. Valid for EGPRS MCS8/9.
/// @param BurstOffset UINT8. INPUT. Offset position for deinterleaving (see ITLV
/// documentation).
/// @param BufferOut UINT32*. OUTPUT. Output pointer.
//======================================================================
PUBLIC VOID spp_DeinterleaveBlock(UINT32* BufferIn,
                                  UINT8 InterleaverType,
                                  UINT8 BurstOffset,
                                  UINT32* BufferOut);

//======================================================================
// spp_DecodeBlock
//----------------------------------------------------------------------
/// Decoding of a data block
///
/// @param BufferIn UINT32*. Pointer to the deinterleaved data frame.
/// @param ChMode UINT16. Channel Type (PAL API format).
/// @param AmrFrameType UINT8. Type of AMR frame (set to 0 excepted for AMR).
/// @param AmrMode UINT8. Codec Mode (set to 0 excepted for AMR).
/// @param BufferOut UINT32*. Output pointer.
/// @param Usf UINT8. Uplink State Flag.
/// @param UncachedBuffers : uncached working memory space
/// @param Res SPP_N_BLOCK_RESULT_T*. Pointer to the Block Result structure.
//======================================================================
PUBLIC VOID spp_DecodeBlock(UINT32* BufferIn,
                            UINT16 ChMode,
                            UINT8 AmrFrameType,
                            UINT8 AmrMode,
                            UINT32* BufferOut,
                            UINT8 Usf,
                            SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                            SPP_N_BLOCK_RESULT_T* Res);

//======================================================================
// spp_GetUSF
//----------------------------------------------------------------------
/// Uplink State Flag extraction
///
/// @param BufferIn UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param CS UINT8. Coding scheme.
///
/// @return UINT8. Uplink State Flag
//======================================================================
PUBLIC UINT8 spp_GetUSF(UINT32* BufferIn,
                        UINT8 CS);

//======================================================================
// spp_DetectDummyPCH
//----------------------------------------------------------------------
/// Dummy PCH detection
///
/// @param BufferIn UINT32* Pointer to the beginning of the softbits.
/// @param Idx      UINT8   Index of the received burst
/// @param Thresh_a UINT8   Threshold for vector A detection
/// @param Thresh_b UINT8   Threshold for vector B detection
//
/// @return         UINT8   0xFF: false, 0x1: only A, 0x2: AB detected
//======================================================================
UINT8 spp_DetectDummyPCH(UINT32 *Buffer,
                         UINT8 Idx,
                         UINT8 Thresh_a,
                         UINT8 Thresh_b);

//======================================================================
// spp_GetMonPower
//----------------------------------------------------------------------
/// Calculate Power for Monitoring (and Interference) windows.
///
/// @param MonIdx UINT8. Index of the monitoring (or interference) window.
/// @param RxBuffer UINT32*. Pointer to the Rx buffer.
/// @param power UINT32*. Pointer to the resulting calculated power.
//======================================================================
PUBLIC VOID spp_GetMonPower(UINT8 MonIdx,
                            UINT32* RxBuffer,
                            UINT32* power);

//======================================================================
// Decoding of AMR Inband Data (DTX and RATSCCH frames)
//----------------------------------------------------------------------
/// This function decodes inband data for AFS or AHS
/// deinterleaved softbit frames (SPEECH, SIDUPDATE, RATSCCH)
///
/// @param softBit : Pointer to the decoded data
/// @param mode : type of frame: 0 -> AFS, 1 -> AHS, 2 -> SIDUPDATE/RATSCCH
///
/// @return UINT8. Inband Data.
//======================================================================
PUBLIC UINT8 spp_GetAmrIc(UINT8 *softBit,
                          UINT8 mode);

//======================================================================
// Search AMR frame markers
//----------------------------------------------------------------------
/// This function searches AMR frame markers and returns the most
/// likely frame type. It decode also the associated in band data.
///
/// @param BufferIn UINT32*. Pointer to the deinterleaved decoded data.
/// @param id1 UINT8*. Pointer to ther first decoded in band data.
/// @param id2 UINT8*. Pointer to the second decoded in band data.
/// @param mode UINT8. TCH rate: 0 -> AFS, 1 -> AHS.
///
/// @return UINT8. Most likely frame type.
//======================================================================
PUBLIC UINT8 spp_SearchAmrMarker(UINT32 *BufferIn,
                                 UINT8 *id1,
                                 UINT8 *id2,
                                 UINT8 mode);


//=============================================================================
// spp_GetAmrMetric
//-----------------------------------------------------------------------------
// This function return amr metric
// @param softBit UINT8*. Pointer to the decoded data.
// @param rate FR/HR
// @param ic inband data
// @param biterror BER
//=============================================================================
PUBLIC UINT8 spp_GetAmrMetric(UINT8* softBit, UINT8 rate, UINT8 ic, UINT8 *biterror);

//======================================================================
// spp_EgprsBlindDetection
//----------------------------------------------------------------------
/// Performs modulation blind detection and TOf estimation:
///.
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param burstIdx (UINT8): burst index number
/// @param tsc UINT8 : training sequence Id
/// @param modulation (SPP_MODULATION_TYPE_T*): Pointer to modulation result
/// @param TOf_in_Qb (UINT8*): Pointer to timing offset result in quarter of bit
/// @param TOf_sample (UINT8*): Pointer to timing offset result
//======================================================================
PUBLIC VOID spp_EgprsBlindDetection(UINT32* RxBuffer,
                                    UINT8 burstIdx,
                                    UINT8 tsc,
                                    UINT8* modulation,
                                    UINT8* TOf_in_Qb,
                                    UINT8* TOf_sample);

//======================================================================
// spp_EgprsEqualizeNBurst
//----------------------------------------------------------------------
/// Performs normal burst equalization including:
/// - DC offset estimation and correction,
/// - IQ imbalance estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
///
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param Tsc (UINT8): Training sequence number
/// @param EqBufferOut (UINT32*): Pointer to the equalized output
/// @param UncachedBuffers : uncached working memory space
/// @param Res (SPP_N_BURST_RESULT_T*): Pointer to the Normal Burst Result structure
//======================================================================
PUBLIC VOID spp_EgprsEqualizeNBurst(UINT32* RxBuffer,
                                    UINT8 Tsc,
                                    UINT32* EqBufferOut,
                                    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                    SPP_N_BURST_RESULT_T* Res);





///@}

///@defgroup gsm_tx_func Tx processing functions
///@{
//-----------------------
// Transmission Functions
//-----------------------
#ifdef CHIP_DIE_8955
//======================================================================
// spp_EncodeABurst
//----------------------------------------------------------------------
/// Access Burst encoding
///
/// @param BufferIn UINT32*. Pointer to the data to be encoded.
/// @param Bsic UINT8. Base Station BSIC.
/// @param size UINT8. 8 or 11 bits Access Burst.
/// @param ts UINT8. ts:0,1,2
/// @param BufferOut UINT32*. Encoded data output pointer.
//======================================================================
PUBLIC VOID spp_EncodeABurst(UINT32* BufferIn,
                             UINT8 Bsic,
                             UINT8 size,
                             UINT8 ts,
                             UINT32* BufferOut);
#else
//======================================================================
// spp_EncodeABurst
//----------------------------------------------------------------------
/// Access Burst encoding
///
/// @param BufferIn UINT32*. Pointer to the data to be encoded.
/// @param Bsic UINT8. Base Station BSIC.
/// @param size UINT8. 8 or 11 bits Access Burst.
/// @param BufferOut UINT32*. Encoded data output pointer.
//======================================================================
PUBLIC VOID spp_EncodeABurst(UINT32* BufferIn,
                             UINT8 Bsic,
                             UINT8 size,
                             UINT32* BufferOut);
#endif

//======================================================================
// spp_EncodeBlock
//----------------------------------------------------------------------
/// Normal BLOCK encoding
///
/// @param InTxBuffer UINT32*. Pointer to the data to be encoded.
/// @param ChMode UINT16. Channel Type (PAL API format).
/// @param AmrFrameType UINT8. Type of AMR frame (set to 0 excepted for AMR).
/// @param AmrMode UINT8. Codec Mode (set to 0 excepted for AMR).
/// @param AmrInBandData UINT8*. AMR in Band Data (set to NULL excepted for AMR).
/// @param BurstOffset UINT8. Offset position for interleaving. Interleaving
/// is performed over a number of Tx bursts. The Offset position corresponds to
/// the offset number of bursts in the interleaving buffer. For example, in
/// FR mode, BurstOffset can be 0 or 4 as one coded block is interleaved over
/// 8 consecutive bursts, and each coded block has 4 bursts worth of payload.
/// (see ITLV documentation).
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 0 : TYPE_1A. Valid for TCH/FS, TCH/EFS, FACCH/F, TCH/F2.4,
/// TCH/AFS (speech, ratscch, sid_first).
/// - 1 : TYPE_1B. Valid for SACCH, SDCCH, TCH/AFS(sid_update), PDTCH(CS-1 to CS-4),
/// BCCH, PCH, AGCH, PACCH, PBCCH, PAGCH, PPCH, PNCH, PTCCH/D.
/// - 2 : TYPE_1C. Valid for FACCH/H.
/// - 3 : TYPE_2A. Valid for TCH/HS and TCH/AHS (speech, ratscch, sid_first).
/// - 4 : TYPE_2B. Valid for TCH/AHS (sid_update).
/// - 5 : TYPE_3. Valid for TCH/F14.4, TCH/F9.6, TCH/F4.8, TCH/H4.8
/// and TCH/H2.4.
/// .
/// @param BufferOut UINT32*. Encoded data output pointer. The function
/// stores the output (the interleaved and encoded bits) in the Tx interleaver buffer.
/// BufferOut should point to the beginning of the Tx interleaver buffer.
//======================================================================
PUBLIC VOID spp_EncodeBlock(UINT32* InTxBuffer,
                            UINT16 ChMode,
                            UINT8 AmrFrameType,
                            UINT8 AmrMode,
                            UINT8* AmrInBandData,
                            UINT8 BurstOffset,
                            UINT8 InterleaverType,
                            UINT32* BufferOut);

//======================================================================
// spp_BuildNBurst
//----------------------------------------------------------------------
/// Normal Burst building and mapping
///
/// @param TxBuffer UINT32*. Pointer to the encoded data.
/// @param Tsc UINT8. Training sequence number.
/// @param StealFlag UINT8. Stealing Flags.
/// @param BufferOut UINT32*. Burst output pointer.
//======================================================================
PUBLIC VOID spp_BuildNBurst(UINT32* TxBuffer,
                            UINT8 Tsc,
                            UINT8 StealFlag,
                            UINT32* BufferOut);

//======================================================================
// spp_Modulation
//----------------------------------------------------------------------
/// This function handles the modulation of the (multiple) burst(s)
/// constituting the full Tx window.
/// The following is true for Granite TC and Greenstone :
/// resulting modulated structure :
/// - Preamble
/// - 1st burst (156 bits)
/// - eventual 2nd burst (156 bits)
/// - eventual 3nd burst (156 bits)
/// - eventual 4nd burst (156 bits)
/// - Postamble (enough for 32 bits)
///
/// Preamble are filled with '1'
/// Postamble are filled with '1'
/// Eventual "Holes" in burst Bitmap are filled with '1' because
/// multislot transmission is continuous from GMSK point of view
///
/// For Emerald:
/// depending on modulation one symbol is either 1 or 3 bits.
/// Modulation can change for each slot. 'Holes' are not sent.
/// resulting modulated structure if burst N is the first active
/// burst or if burst N-1 is inactive:
///
/// - bstN definition for RF_IF
/// - Preamble symb                 |
/// - bstN active part (148 symb)   |
///                                 | in burst N modulation
/// then if bstN+1 is active:       |
/// - (8-guardStart) symb           |
/// - bstN+1 definition for RF_IF
/// - guardStart symb               |
/// - bstN+1 active part (148 symb) | in burst N+1 modulation
/// - ... and so on until inactive burst
///
/// else if bstN+1 is inactive:
/// - Postamble symb                | in burst N modulation
/// - End of transmit for RF_IF
///
/// @param BufferIn pointer to the mapped burst(s) data
/// @param Bitmap Tx window bitmap allocation (first active bit is
/// considered as 1st burst)
/// @param preamble Burst Mapping parameters
/// @param postamble Burst Mapping parameters
/// @param guardStart Burst Mapping parameters
/// @param BufferOut Pointer to the output sw fifo (if relevant
//         for HW implementation)
/// @param BstMod Type of the modulation, 8PSK or GMSK.
/// @param NormalBst 0:Access Burst 1:Normal Burst
/// @todo  change modulation field to a bitmap to handle multislot.
/// @todo  do a structure for Burst Mapping parameters
/// @todo  include the burst type (AB/NB) in the modulation bitmap...
//======================================================================

PUBLIC VOID spp_Modulation(UINT32* BufferIn,
                           UINT8 Bitmap,
                           UINT16 preamble,
                           UINT16 postamble,
                           UINT16 guardStart,
                           UINT32* BufferOut,
                           SPP_MODULATION_TYPE_T BstMod,
                           BOOL NormalBst);



//-----------------------
// Ciphering Functions
//-----------------------
///@defgroup gsm_ciphering_func GSM/GPRS Miscellanous Functions
///@{

//======================================================================
// spp_CipherCipherNB
//----------------------------------------------------------------------
/// This function ciphers 114 bits for normal burst.
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
///
//======================================================================
PUBLIC VOID spp_CipherCipherNb(UINT32* BufferIn, UINT32* BufferOut);

//======================================================================
// spp_CipherCipherNB
//----------------------------------------------------------------------
/// This function deciphers 114 bits for normal burst.
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
///
//======================================================================
PUBLIC VOID spp_CipherDecipherNb(UINT32* BufferIn, UINT32* BufferOut);

//======================================================================
// Initialize the count register
//----------------------------------------------------------------------
/// This function initializes the count register.
///
/// @param count UINT32. Counter register.
//======================================================================
PUBLIC UINT8 spp_CipherCountInit(UINT32 count);

//======================================================================
// spp_CipherStart
//----------------------------------------------------------------------
/// Start the encipherment.
///
/// @param algo UINT8.
///
/// @return UINT8. Error or no error???
//======================================================================
PUBLIC UINT8 spp_CipherStart(UINT8 algo);

/// @}


//-----------------------
// Miscellanous Functions
//-----------------------
///@defgroup gsm_misc_func GSM/GPRS Miscellanous Functions
///@{

// =============================================================================
// spp_FullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function returns the number of completely received (and not treated)
/// bursts in the active frame.
/// @return
// =============================================================================
PUBLIC UINT8 spp_FullBurstReady(VOID);

// =============================================================================
// spp_ResetFullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function resets the number of completely received bursts in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_ResetFullBurstReady(VOID);

// =============================================================================
// spp_OneFullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function flags that a burst has been completely received in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_OneFullBurstReady(VOID);

// =============================================================================
// spp_OneFullBurstDone
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function flags that a burst has been fully processed in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_OneFullBurstDone(VOID);



/// @}





#endif

