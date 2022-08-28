#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

#include "AgbTypes.h"
#include "Xcept.h"

class Rom {
public:
    Rom(const std::filesystem::path& filePath, bool verificationEnabled = true);
    Rom(const uint8_t *data, size_t size, bool verificationEnabled = true);
    Rom(const Rom&) = delete;
    Rom& operator=(const Rom&) = delete;

    const uint8_t& operator[](size_t pos) const {
        return data[pos];
    }

    void CheckPosition(size_t pos) const {
        if (pos >= size)
            throw Xcept("Index %lu is out of range", pos);
    }

    int8_t ReadS8(size_t pos) const {
        CheckPosition(pos);
        return static_cast<int8_t>(data[pos]);
    }

    uint8_t ReadU8(size_t pos) const {
        CheckPosition(pos);
        return data[pos];
    }

    int16_t ReadS16(size_t pos) const {
        CheckPosition(pos);
        return static_cast<int16_t>(ReadU16(pos));
    }

    uint16_t ReadU16(size_t pos) const {
        CheckPosition(pos);
        uint32_t retval = data[pos + 1];
        retval <<= 8;
        retval |= data[pos];
        return static_cast<uint16_t>(retval);
    }

    int32_t ReadS32(size_t pos) const {
        CheckPosition(pos);
        return static_cast<int32_t>(ReadU32(pos));
    }

    uint32_t ReadU32(size_t pos) const {
        CheckPosition(pos);
        uint32_t retval = data[pos + 3];
        retval <<= 8;
        retval |= data[pos + 2];
        retval <<= 8;
        retval |= data[pos + 1];
        retval <<= 8;
        retval |= data[pos + 0];
        return retval;
    }

    size_t ReadAgbPtrToPos(size_t pos) const {
        uint32_t ptr = ReadU32(pos);
        if (!ValidPointer(ptr))
            throw Xcept("Cannot parse pointer at [%08zX]=%08X", pos, ptr);
        return ptr - AGB_MAP_ROM;
    }

    const void *GetPtr(size_t pos) const {
        CheckPosition(pos);
        return &data[pos];
    }

    size_t Size() const {
        return size;
    }

    bool ValidPointer(uint32_t ptr) const {
        if (ptr - AGB_MAP_ROM >= size)
            return false;
        if (ptr - AGB_MAP_ROM + 1 >= size)
            return false;
        return true;
    }

    std::string ReadString(size_t pos, size_t limit) const;
    std::string GetROMCode() const;

private:
    void verify();
    void loadFile(const std::filesystem::path& filePath);

    std::vector<uint8_t> data_vector;
    size_t size;
    const uint8_t *data;
};
