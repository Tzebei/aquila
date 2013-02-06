/**
 * @file RawPcmFile.h
 *
 * Reading raw PCM binary data from file.
 *
 * This file is part of the Aquila DSP library.
 * Aquila is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package Aquila
 * @version 3.0.0-dev
 * @author Zbigniew Siciarz
 * @date 2007-2013
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 3.0.0
 */

#ifndef RAWPCMFILE_H
#define RAWPCMFILE_H

#include "../global.h"
#include "SignalSource.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>

namespace Aquila
{
    /**
     * A class to read raw PCM binary data from file.

     * No headers are allowed in the file.
     *
     * Any numeric type will be converted on the fly to SampleType. Sample
     * rate must be known prior to opening the file as the constructor expects
     * sample frequency as its second argument.
     */
    template <typename Numeric = SampleType>
    class AQUILA_EXPORT RawPcmFile : public SignalSource
    {
    public:
        /**
         * Creates the data source.
         *
         * @param filename full path to .txt file
         * @param sampleFrequency sample frequency of the data in file
         */
        RawPcmFile(std::string filename, FrequencyType sampleFrequency):
            SignalSource(sampleFrequency)
        {
            std::fstream fs;
            fs.open(filename.c_str(), std::ios::in | std::ios::binary);
            // get file size by seeking to the end and telling current position
            fs.seekg(0, std::ios::end);
            std::streamsize fileSize = fs.tellg();
            // seek back to the beginning so read() can access all content
            fs.seekg(0, std::ios::beg);
            std::size_t samplesCount = fileSize / sizeof(Numeric);
            // read raw data into a temporary buffer
            Numeric* buffer = new Numeric[samplesCount];
            fs.read((char*)buffer, fileSize);
            // copy and implicit conversion to SampleType
            m_data.assign(buffer, buffer + samplesCount);
            delete [] buffer;
            fs.close();
        }
    };
}

#endif // RAWPCMFILE_H
