// Harvard CS50 recover in C++
#include <array>
#include <format>
#include <fstream>
#include <print>
#include <span>

#include "include/stopwatch.hxx"
namespace recover
{
// utility lambdas remove error prone reinterpret casts from code replace with safer bit_cast
auto inline const_byte_ptr_to_const_char_ptr = [](const std::byte* Ptr) noexcept {
  return std::bit_cast<const char*>(Ptr);
};
auto inline byte_ptr_to_char_ptr = [](std::byte* Ptr) noexcept {
  return std::bit_cast<char*>(Ptr);
};

constexpr auto is_jpeg_header = [](const auto JPEG_Block) noexcept {
  return JPEG_Block[0] == std::byte{0xff} && JPEG_Block[1] == std::byte{0xd8} && JPEG_Block[2] == std::byte{0xff} &&
                 (JPEG_Block[3] & std::byte{0xf0}) == std::byte{0xe0}
             ? true
             : false;
};

const auto read_block = [](auto FAT_Block, std::ifstream& Raw_Disk_Image) {
  // Read a FAT conformant block From rawImage
  return Raw_Disk_Image.read(byte_ptr_to_char_ptr(FAT_Block.data()), FAT_Block.size()) && Raw_Disk_Image.good() ? true
                                                                                                                : false;
};

auto write_block = [](const auto JPEG_Block, std::ofstream& JPEG_File) {
  // Write a JPEG_Block to JPEG_File
  return JPEG_File.write(const_byte_ptr_to_const_char_ptr(JPEG_Block.data()), JPEG_Block.size()) && JPEG_File.good()
             ? true
             : false;
};

}  // namespace recover

int main(int argc, char* argv[]) noexcept
{
  StopWatch RecoveryTime("Time JPG recovery");  // Time the runtime of the process

  // Input sanity check
  if (argc != 2) {
    std::println("usage: recover <input_filename>");
    return 1;
  }

  std::string_view Image_Filename(argv[1]);

  std::ifstream Disk_Image_FAT(Image_Filename.data(), std::ios::binary);  // Open the raw disk image file
  if (!Disk_Image_FAT.is_open()) [[unlikely]] {
    // Could not open the file
    std::println("File {} could not be opened: File does not exist, or you do not have permission to read.",
                 Image_Filename);
    return 1;
  }
  else [[likely]] {
    // DiskImage file opened successfully
    constexpr size_t FAT_BLOCK_SIZE = 512;  // FAT format block size

    // Static to ensure allocation in static storage, not on the runtime stack
    static std::array<std::byte, FAT_BLOCK_SIZE> FAT_Block = {};

    std::size_t JPEG_FileNumber = 0;
    std::ofstream JPEG_File;

    auto Current_Block = std::span{FAT_Block};

    while (recover::read_block(Current_Block, Disk_Image_FAT)) {
      // Possible states
      //   initial state:
      //     no open files -> must still be in FAT
      //   JPEG Recovery state:
      //     Block is a jpeg header -> start of new JPEG
      //       if OutputFile is open close it.
      //       Open Outputfile Write header
      //   JPEG Block Recovery state: We have OpenFile must be JPEG block. Write it.

      if (recover::is_jpeg_header(Current_Block)) {
        // initial state -> JPEG Recovery state: Done with FAT. JPEG recovery begins
        // or
        // JPEG Block Recovery state -> JPEG Recovery state:

        if (JPEG_File.is_open()) {
          // JPEG Block Recovery state -> JPEG Recovery state: We are done with the currently open file. Close it.

          JPEG_File.close();
        }
        JPEG_File.open(std::format("{:0>3}.jpg", JPEG_FileNumber), std::ios::binary);
        if (!JPEG_File.is_open() || !JPEG_File.good()) {
          // JPEG Recovery state -> JPEG Block Recovery state: Open a new file for the new JPEG

          std::println("Could not open {:0>3}.jpg", JPEG_FileNumber);
          return 1;  // Exit if the new file cannot be opened
        }
        ++JPEG_FileNumber;  // Increment the file counter for the next JPEG
      }

      if (JPEG_File.is_open()) [[likely]] {
        // JPEG Block Recovery state: Write the current block to the open JPEG file

        if (!recover::write_block(Current_Block, JPEG_File)) [[unlikely]] {
          return 1;  // Exit if writing a block to the file fails
        }
      }
    }
    if (JPEG_File.is_open()) { JPEG_File.close(); }  // redundant because RAII
    std::println ("Found JPEGs: {}",JPEG_FileNumber);
  }
  if (Disk_Image_FAT.is_open()) { Disk_Image_FAT.close(); }  // redundant because RAII
  return 0;
}