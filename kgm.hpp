#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

size_t encSuccess = 0, encFailure = 0;
size_t decSuccess = 0, decFailure = 0;

size_t fileSizeG(std::fstream& file)
{
    file.seekg(0, std::ios::end);
    size_t fs = (size_t)file.tellg();
    file.seekg(0, std::ios::beg);
    return fs;
}

std::vector<uint8_t> VprHeader = {
    0x05, 0x28, 0xBC, 0x96, 0xE9, 0xE4, 0x5A, 0x43,
    0x91, 0xAA, 0xBD, 0xD0, 0x7A, 0xF5, 0x36, 0x31 };
std::vector<uint8_t> KgmHeader = {
    0x7C, 0xD5, 0x32, 0xEB, 0x86, 0x02, 0x7F, 0x4B,
    0xA8, 0xAF, 0xA6, 0x8E, 0x0F, 0xFF, 0x99, 0x14 };
std::vector<uint8_t> VprMaskDiff = {
    0x25, 0xDF, 0xE8, 0xA6, 0x75, 0x1E, 0x75, 0x0E,
    0x2F, 0x80, 0xF3, 0x2D, 0xB8, 0xB6, 0xE3, 0x11, 0x00 };

std::vector<uint8_t> MaskV2;

std::vector<uint8_t> MaskV2PreDef = {
        0xB8, 0xD5, 0x3D, 0xB2, 0xE9, 0xAF, 0x78, 0x8C, 0x83, 0x33, 0x71, 0x51, 0x76, 0xA0, 0xCD, 0x37,
        0x2F, 0x3E, 0x35, 0x8D, 0xA9, 0xBE, 0x98, 0xB7, 0xE7, 0x8C, 0x22, 0xCE, 0x5A, 0x61, 0xDF, 0x68,
        0x69, 0x89, 0xFE, 0xA5, 0xB6, 0xDE, 0xA9, 0x77, 0xFC, 0xC8, 0xBD, 0xBD, 0xE5, 0x6D, 0x3E, 0x5A,
        0x36, 0xEF, 0x69, 0x4E, 0xBE, 0xE1, 0xE9, 0x66, 0x1C, 0xF3, 0xD9, 0x02, 0xB6, 0xF2, 0x12, 0x9B,
        0x44, 0xD0, 0x6F, 0xB9, 0x35, 0x89, 0xB6, 0x46, 0x6D, 0x73, 0x82, 0x06, 0x69, 0xC1, 0xED, 0xD7,
        0x85, 0xC2, 0x30, 0xDF, 0xA2, 0x62, 0xBE, 0x79, 0x2D, 0x62, 0x62, 0x3D, 0x0D, 0x7E, 0xBE, 0x48,
        0x89, 0x23, 0x02, 0xA0, 0xE4, 0xD5, 0x75, 0x51, 0x32, 0x02, 0x53, 0xFD, 0x16, 0x3A, 0x21, 0x3B,
        0x16, 0x0F, 0xC3, 0xB2, 0xBB, 0xB3, 0xE2, 0xBA, 0x3A, 0x3D, 0x13, 0xEC, 0xF6, 0x01, 0x45, 0x84,
        0xA5, 0x70, 0x0F, 0x93, 0x49, 0x0C, 0x64, 0xCD, 0x31, 0xD5, 0xCC, 0x4C, 0x07, 0x01, 0x9E, 0x00,
        0x1A, 0x23, 0x90, 0xBF, 0x88, 0x1E, 0x3B, 0xAB, 0xA6, 0x3E, 0xC4, 0x73, 0x47, 0x10, 0x7E, 0x3B,
        0x5E, 0xBC, 0xE3, 0x00, 0x84, 0xFF, 0x09, 0xD4, 0xE0, 0x89, 0x0F, 0x5B, 0x58, 0x70, 0x4F, 0xFB,
        0x65, 0xD8, 0x5C, 0x53, 0x1B, 0xD3, 0xC8, 0xC6, 0xBF, 0xEF, 0x98, 0xB0, 0x50, 0x4F, 0x0F, 0xEA,
        0xE5, 0x83, 0x58, 0x8C, 0x28, 0x2C, 0x84, 0x67, 0xCD, 0xD0, 0x9E, 0x47, 0xDB, 0x27, 0x50, 0xCA,
        0xF4, 0x63, 0x63, 0xE8, 0x97, 0x7F, 0x1B, 0x4B, 0x0C, 0xC2, 0xC1, 0x21, 0x4C, 0xCC, 0x58, 0xF5,
        0x94, 0x52, 0xA3, 0xF3, 0xD3, 0xE0, 0x68, 0xF4, 0x00, 0x23, 0xF3, 0x5E, 0x0A, 0x7B, 0x93, 0xDD,
        0xAB, 0x12, 0xB2, 0x13, 0xE8, 0x84, 0xD7, 0xA7, 0x9F, 0x0F, 0x32, 0x4C, 0x55, 0x1D, 0x04, 0x36,
        0x52, 0xDC, 0x03, 0xF3, 0xF9, 0x4E, 0x42, 0xE9, 0x3D, 0x61, 0xEF, 0x7C, 0xB6, 0xB3, 0x93, 0x50,
};

bool loadMaskV2() {
    std::fstream infile("kgm.mask", std::ios::in | std::ios::binary);

    if (!infile.is_open()) {
        std::cerr << "Cannot load mask file." << std::endl;
        return false;
    }

    MaskV2.resize(fileSizeG(infile));

    infile.read((char*)MaskV2.data(), MaskV2.size());
    return true;
}

uint8_t getMask(size_t pos) {
    return MaskV2PreDef[pos % 272] ^ MaskV2[pos >> 4];
}

void encrypt(std::string fileName, std::string type, std::string pswFn = "") {
    if (type != "kgm" && type != "vpr")
        return;
    std::fstream infile(fileName, std::ios::in | std::ios::binary);

    if (!infile.is_open()) {
        std::cerr << "Cannot read file " << fileName << "." << std::endl;
        encFailure++;
        return;
    }

    std::vector<uint8_t> bHeaderLen;
    bHeaderLen.resize(4);
    uint32_t headerLen = 0x2c;
    *((uint32_t*)bHeaderLen.data()) = headerLen;

    std::vector<uint8_t> fileData;
    fileData.resize(fileSizeG(infile));
    infile.read((char*)fileData.data(), fileData.size());

    std::vector<uint8_t> key;
    key.resize(17);
    srand(time(0));
    for (size_t i = 0; i < 0x10; i++)
    {
        key[i] = rand();
    }
    key[16] = 0;

    if (!loadMaskV2())
    {
        encFailure++;
        return;
    }

    if ("vpr" == type) {
        for (size_t i = 0; i < fileData.size(); i++) fileData[i] ^= VprMaskDiff[i % 17];
    }
    for (size_t i = 0; i < fileData.size(); i++) {
        auto msk8 = getMask(i);
        msk8 ^= (msk8 & 0xf) << 4;
        auto med8 = fileData[i] ^ msk8;
        med8 ^= (med8 & 0xf) << 4;
        fileData[i] = key[i % 17] ^ med8;
    }

    std::string outFn = fileName.substr(0, fileName.find_last_of(".")) + "." + type + fileName.substr(1 + fileName.find_last_of("."));
    std::cout << "Output:\n" << outFn << std::endl;
    std::fstream outfile;

    if (pswFn != "")
    {
        outfile.open(pswFn, std::ios::out | std::ios::binary);
    }
    else
    {
        outfile.open(outFn, std::ios::out | std::ios::binary);
    }
    if (!outfile.is_open()) {
        std::cerr << "Cannot write file " << outFn << "." << std::endl;
        encFailure++;
        return;
    }

    outfile.write(type == "kgm" ? (char*)KgmHeader.data() : (char*)VprHeader.data(), 0x10);
    outfile.write((char*)bHeaderLen.data(), 4);
    for (size_t i = 0; i < 8; i++)
    {
        outfile << (uint8_t)rand();
    }
    outfile.write((char*)key.data(), 0x10);
    if (pswFn != "")
    {
        outfile.close();
        outfile.open(outFn, std::ios::out | std::ios::binary);
    }
    outfile.write((char*)fileData.data(), fileData.size());
    encSuccess++;
}

std::string extViaExt(std::string fileName)
{
    if (fileName.find(".kgm") < fileName.size())
    {
        return "." + fileName.substr(4 + fileName.find_last_of("."));
    }
    else if (fileName.find(".vpr") < fileName.size())
    {
        return "." + fileName.substr(4 + fileName.find_last_of("."));
    }
    else
    {
        return ".bin";
    }
}

void decrypt(std::string fileName, std::string pswFn = "") {
    std::fstream infile;
    if (pswFn != "")
    {
        infile.open(pswFn, std::ios::in | std::ios::binary);
    }
    else
    {
        infile.open(fileName, std::ios::in | std::ios::binary);
    }

    if (!infile.is_open()) {
        std::cerr << "Cannot read file " << fileName << "." << std::endl;
        decFailure++;
        return;
    }

    std::vector<uint8_t> bHeaderLen;
    bHeaderLen.resize(4);

    infile.seekg(0x10, std::ios::beg);
    infile.read((char*)bHeaderLen.data(), 4);

    uint32_t headerLen = pswFn == "" ? *((uint32_t*)bHeaderLen.data()) : 0;

    std::vector<uint8_t> key;
    key.resize(17);
    infile.seekg(0x1c, std::ios::beg);
    infile.read((char*)key.data(), 0x10);
    key[16] = 0;

    if (pswFn != "")
    {
        infile.close();
        infile.open(fileName, std::ios::in | std::ios::binary);
    }
    std::vector<uint8_t> fileData;
    fileData.resize(fileSizeG(infile) - headerLen);
    infile.seekg(headerLen, std::ios::beg);
    infile.read((char*)fileData.data(), fileData.size());

    if (!loadMaskV2())
    {
        decFailure++;
        return;
    }

    for (size_t i = 0; i < fileData.size(); i++) {
        auto med8 = key[i % 17] ^ fileData[i];
        med8 ^= (med8 & 0xf) << 4;

        auto msk8 = getMask(i);
        msk8 ^= (msk8 & 0xf) << 4;
        fileData[i] = med8 ^ msk8;
    }
    if (fileName.find(".vpr") < fileName.size()) {
        for (size_t i = 0; i < fileData.size(); i++) fileData[i] ^= VprMaskDiff[i % 17];
    }

    std::string ext = extViaExt(fileName);
    std::string outFn = fileName.substr(0, fileName.find_last_of(".")) + ext;
    std::cout << "Output:\n" << outFn << std::endl;
    std::fstream outfile(outFn, std::ios::out | std::ios::binary);

    if (!outfile.is_open()) {
        std::cerr << "Cannot write file " << outFn << "." << std::endl;
        decFailure++;
        return;
    }

    outfile.write((char*)fileData.data(), fileData.size());
    decSuccess++;
}
