#include <string>
#include <cstdio>
#include <cstring>
#include <fstream>

#include "Rom.h"
#include "Xcept.h"
#include "Debug.h"
#include "Util.h"


/*
 * public
 */

Rom::Rom(const std::filesystem::path& filePath, bool verificationEnabled)
{
    loadFile(filePath);
    data = data_vector.data();
    size = data_vector.size();
    if(verificationEnabled) {
        verify();
    }
}

Rom::Rom(const uint8_t *data, size_t size, bool verificationEnabled): size(size), data(data) {
    if(verificationEnabled) {
        verify();
    }
}

std::string Rom::ReadString(size_t pos, size_t limit) const
{
    std::string result;
    for (size_t i = 0; i < limit; i++) {
        char c = static_cast<char>(ReadU8(pos + i));
        if (c == '\0')
            break;
        result += c;
    }
    return result;
}

std::string Rom::GetROMCode() const
{
    return ReadString(0xAC, 4);
}

/*
 * private
 */

void Rom::verify()
{
    // check ROM size
    if (size > AGB_ROM_SIZE || size < 0x200)
        throw Xcept("Illegal ROM size");
    
    // Logo data
    // TODO replace 1 to 1 logo comparison with checksum
    uint8_t imageBytes[] = {
        0x24,0xff,0xae,0x51,0x69,0x9a,0xa2,0x21,0x3d,0x84,0x82,0x0a,0x84,0xe4,0x09,0xad,
        0x11,0x24,0x8b,0x98,0xc0,0x81,0x7f,0x21,0xa3,0x52,0xbe,0x19,0x93,0x09,0xce,0x20,
        0x10,0x46,0x4a,0x4a,0xf8,0x27,0x31,0xec,0x58,0xc7,0xe8,0x33,0x82,0xe3,0xce,0xbf,
        0x85,0xf4,0xdf,0x94,0xce,0x4b,0x09,0xc1,0x94,0x56,0x8a,0xc0,0x13,0x72,0xa7,0xfc,
        0x9f,0x84,0x4d,0x73,0xa3,0xca,0x9a,0x61,0x58,0x97,0xa3,0x27,0xfc,0x03,0x98,0x76,
        0x23,0x1d,0xc7,0x61,0x03,0x04,0xae,0x56,0xbf,0x38,0x84,0x00,0x40,0xa7,0x0e,0xfd,
        0xff,0x52,0xfe,0x03,0x6f,0x95,0x30,0xf1,0x97,0xfb,0xc0,0x85,0x60,0xd6,0x80,0x25,
        0xa9,0x63,0xbe,0x03,0x01,0x4e,0x38,0xe2,0xf9,0xa2,0x34,0xff,0xbb,0x3e,0x03,0x44,
        0x78,0x00,0x90,0xcb,0x88,0x11,0x3a,0x94,0x65,0xc0,0x7c,0x63,0x87,0xf0,0x3c,0xaf,
        0xd6,0x25,0xe4,0x8b,0x38,0x0a,0xac,0x72,0x21,0xd4,0xf8,0x07
    };

    // check logo
   for (size_t i = 0; i < sizeof(imageBytes); i++) {
       if (imageBytes[i] != ReadU8(i + 0x4))
           throw Xcept("ROM verification: Bad Nintendo Logo");
   }

    // check checksum
    uint8_t checksum = ReadU8(0xBD);
    int check = 0;
    for (size_t i = 0xA0; i < 0xBD; i++) {
        check -= ReadU8(i);
    }
    check = (check - 0x19) & 0xFF;
    if (check != checksum)
        throw Xcept("ROM verification: Bad Header Checksum: %02X - expected %02X", (int)checksum, (int)check);
}

void Rom::loadFile(const std::filesystem::path& filePath)
{
    std::ifstream is(filePath, std::ios_base::binary);
    if (!is.is_open()) {
        throw Xcept("Error while opening ROM: %s", strerror(errno));
    }
    is.seekg(0, std::ios_base::end);
    std::ifstream::pos_type size = is.tellg();
    if (size == -1) {
        throw Xcept("Error while seeking in input file");
    }
    if (size > AGB_ROM_SIZE) {
        throw Xcept("Input ROM exceeds 32 MiB file limit");
    }
    is.seekg(0, std::ios_base::beg);
    data_vector.resize(static_cast<size_t>(size));

    // copy file to memory
    is.read(reinterpret_cast<char *>(data_vector.data()), size);
    if (is.bad())
        throw Xcept("read bad");
    if (is.fail()) {
        throw Xcept("read fail");
    }
    is.close();
}

