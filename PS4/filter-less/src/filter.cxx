#include <getopt.h>
#include <cstdlib>

#include <memory>

#include "include/bmp.hxx"
#include "include/helpers.hxx"

////
/// this File has been kept as close to the C implentation as possible
/// The changes made are the use of unique_ptr for the Image instead of calloc and Image=nullptr; instead of free.
/// This breaks RAII the std::unique_ptr would automatically free when it goes out of scope.
/// The addition of the named lambda apply_filter to clarify exactly which filter is going to be applied.
/// Since helpers.hxx implements the edges() filter. It would be a trivial exercise to add support for it here. That is
/// not CS50's model, so I do not do that.
/// In a sense this is still the C-based driver from CS50. Aside from that the
/// above, the only change is to names of idenfiers and or functions to conform to the style guide set in CONTRIBUTIND.md
/// in the REPo base
//

int main(int argc, char* argv[])
{
  // Define allowable filters
  const char* AVAILABLE_FILTERS = "bgrs";

  // Get a filter flag and check validity
  char Filter = getopt(argc, argv, AVAILABLE_FILTERS);
  if (Filter == '?') {
    printf("Invalid filter.\n");
    return 1;
  }

  // Ensure only one filter
  if (getopt(argc, argv, AVAILABLE_FILTERS) != -1) {
    printf("Only one filter allowed.\n");
    return 2;
  }

  // Ensure proper usage
  if (argc != optind + 2) {
    printf("Usage: ./filter [flag] infile outfile\n");
    return 3;
  }

  // Remember filenames
  char* In_File = argv[optind];
  char* Out_File = argv[optind + 1];

  // Open the input file
  FILE* In_Ptr = fopen(In_File, "r");
  if (In_Ptr == nullptr) {
    printf("Could not open %s.\n", In_File);
    return 4;
  }

  // Open the output file
  FILE* Out_Ptr = fopen(Out_File, "w");
  if (Out_Ptr == nullptr) {
    fclose(In_Ptr);
    printf("Could not create %s.\n", Out_File);
    return 5;
  }

  // Read infile's BITMAPFILEHEADER
  BITMAPFILEHEADER Bitmap_File_Header = {};
  fread(&Bitmap_File_Header, sizeof(BITMAPFILEHEADER), 1, In_Ptr);

  // Read infile's BITMAPINFOHEADER
  BITMAPINFOHEADER Bitmap_Info_Header = {};
  fread(&Bitmap_Info_Header, sizeof(BITMAPINFOHEADER), 1, In_Ptr);

  // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
  if (Bitmap_File_Header.bfType != 0x4d42 || Bitmap_File_Header.bfOffBits != 54 || Bitmap_Info_Header.biSize != 40 ||
      Bitmap_Info_Header.biBitCount != 24 || Bitmap_Info_Header.biCompression != 0) {
    fclose(Out_Ptr);
    fclose(In_Ptr);
    printf("Unsupported file format.\n");
    return 6;
  }

  // Get image's dimensions
  int Height = abs(Bitmap_Info_Header.biHeight);
  int Width = Bitmap_Info_Header.biWidth;

  // Allocate memory for the image need to convert to C++ Use vector?
  auto Image  = std::make_unique<RGBTRIPLE[]>(Height * Width);

  if (Image == nullptr) {
    printf("Not enough memory to store image.\n");
    fclose(Out_Ptr);
    fclose(In_Ptr);
    return 7;
  }

  // Determine padding for scanlines
  int Padding = (4 - (Width * sizeof(RGBTRIPLE)) % 4) % 4;

  auto Image_Span = std::mdspan(Image.get(), Height, Width);

  for (const auto [Current_Row, Current_Column] : index_mdspan(Image_Span)) {
    // Iterate over infile's scanlines
    if (Current_Column == 0) {
      // the first column of read and seek line
      // Read row into a pixel array
      fread((void*)&Image_Span[Current_Row, Current_Column], sizeof(RGBTRIPLE), Width, In_Ptr);

      // Skip over padding
      fseek(In_Ptr, Padding, SEEK_CUR);
    }
  }

  // Define the apply_filter lambda
  auto apply_filter = [&](char Filter_To_Apply, auto& The_Image) {
    switch (Filter_To_Apply) {
      case 'b':
        blur(The_Image);
        break;
      case 'g':
        grey_scale(The_Image);
        break;
      case 'r':
        reflect(The_Image);
        break;
      case 's':
        sepia(The_Image);
        break;
      default:
        printf("Error: Invalid filter provided.\n");
        exit(1);
    }
  };
  // Apply the selected filter
  apply_filter(Filter, Image_Span);
  // Write outfile's BITMAPFILEHEADER
  fwrite(&Bitmap_File_Header, sizeof(BITMAPFILEHEADER), 1, Out_Ptr);

  // Write outfile's BITMAPINFOHEADER
  fwrite(&Bitmap_Info_Header, sizeof(BITMAPINFOHEADER), 1, Out_Ptr);

  // Write new pixels to outfile
  for (const auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    if (Col_Pos == 0) {  // on beginning of line
      fwrite((void*)&Image_Span[Row_Pos, Col_Pos], sizeof(RGBTRIPLE), Width, Out_Ptr);

      // Write padding at the end of the row
      for (int k = 0; k < Padding; k++) { fputc(0x00, Out_Ptr); }
    }
  }
  // Free memory for image
  Image = nullptr;

  // Close files
  fclose(In_Ptr);
  fclose(Out_Ptr);
  return 0;
}
