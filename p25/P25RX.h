/**
* Digital Voice Modem - DSP Firmware
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / DSP Firmware
*
*/
//
// Based on code from the MMDVM project. (https://github.com/g4klx/MMDVM)
// Licensed under the GPLv2 License (https://opensource.org/licenses/GPL-2.0)
//
/*
*   Copyright (C) 2015,2016,2017 by Jonathan Naylor G4KLX
*   Copyright (C) 2017-2019 Bryan Biedenkapp N2PLL
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#if !defined(__P25_RX_H__)
#define __P25_RX_H__

#include "Defines.h"
#include "p25/P25Defines.h"

namespace p25
{
    // ---------------------------------------------------------------------------
    //  Constants
    // ---------------------------------------------------------------------------

    enum P25RX_STATE {
        P25RXS_NONE,
        P25RXS_BURST,
        P25RXS_LDU,
        P25RXS_PDU
    };

    // ---------------------------------------------------------------------------
    //  Class Declaration
    //      Implements receiver logic for P25 mode operation.
    // ---------------------------------------------------------------------------

    class DSP_FW_API P25RX {
    public:
        /// <summary>Initializes a new instance of the P25RX class.</summary>
        P25RX();

        /// <summary>Helper to reset data values to defaults.</summary>
        void reset();

        /// <summary>Sample P25 values from the air interface.</summary>
        void samples(const q15_t* samples, uint16_t* rssi, uint8_t length);

    private:
        P25RX_STATE m_state;

        uint32_t m_bitBuffer[P25_RADIO_SYMBOL_LENGTH];
        q15_t m_buffer[P25_LDU_FRAME_LENGTH_SAMPLES];

        uint16_t m_bitPtr;
        uint16_t m_dataPtr;

        uint16_t m_minSyncPtr;
        uint16_t m_maxSyncPtr;

        uint16_t m_burstStartPtr;
        uint16_t m_burstSyncPtr;

        uint16_t m_lduStartPtr;
        uint16_t m_lduEndPtr;
        uint16_t m_lduSyncPtr;

        uint8_t m_averagePtr;

        q31_t m_maxCorr;
        uint16_t m_lostCount;
        uint8_t m_countdown;

        q15_t m_centre[16U];
        q15_t m_centreVal;
        q15_t m_threshold[16U];
        q15_t m_thresholdVal;

        uint32_t m_rssiAccum;
        uint16_t m_rssiCount;

        uint16_t m_nac;
        uint8_t m_duid;

        /// <summary>Helper to perform initial sample processing.</summary>
        void processNone(q15_t sample);

        /// <summary>Helper to process "burst" P25 samples.</summary>
        void processBurst(q15_t sample);
        /// <summary>Helper to process LDU P25 samples.</summary>
        void processLdu(q15_t sample);
        /// <summary>Helper to process PDU P25 samples.</summary>
        void processPdu(q15_t sample);

        /// <summary>Frame synchronization correlator.</summary>
        bool correlateSync();

        /// <summary></summary>
        void calculateLevels(uint16_t start, uint16_t count);
        /// <summary></summary>
        void samplesToBits(uint16_t start, uint16_t count, uint8_t* buffer, uint16_t offset, q15_t centre, q15_t threshold);
    };
} // namespace p25

#endif // __P25_RX_H__