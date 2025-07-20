#ifndef HELPERS_HXX
#define HELPERS_HXX
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <mdspan>
#include <memory>
#include <numeric>
#include <ranges>
#include <unordered_map>

#include "bmp.hxx"

template <typename T>
struct IsMdspan : std::false_type
{
};

template <typename T, typename E, typename LP, typename AP>
struct IsMdspan<std::mdspan<T, E, LP, AP>> : std::true_type
{
};

template <typename T>
static constexpr bool IS_MD_SPAN = IsMdspan<T>::value;

// Standalone recursive function for Cartesian product
template <std::size_t Dimension = 0, typename Tuple = std::tuple<>, typename Extents>
constexpr auto cartesian_product(Tuple Current, const Extents& Current_Extents)
{
  constexpr auto Rank = std::tuple_size_v<Extents>;

  if constexpr (Dimension == Rank) {
    // Base case: Wrap the current tuple as a single-element view
    return std::views::single(Current);
  }
  else {
    // Recursive case: Iterate over the current dimension and append indices
    return std::views::join(std::views::transform(
        std::views::iota(std::tuple_element_t<Dimension, Extents>{0}, std::get<Dimension>(Current_Extents)),
        [=](auto Index) {
          return cartesian_product<Dimension + 1>(std::tuple_cat(Current, std::make_tuple(Index)), Current_Extents);
        }));
  }
}

// Main md_indices function
template <typename MDS>
  requires IS_MD_SPAN<MDS>  // Assumes is_mdspan_v is defined elsewhere
constexpr auto index_mdspan(const MDS& Mds)
{
  // Extract extents into a tuple for easier access
  constexpr auto Rank = MDS::extents_type::rank();
  auto Extents = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    return std::tuple{Mds.extents().extent(Is)...};
  }(std::make_index_sequence<Rank>{});

  // Generate Cartesian product
  return cartesian_product<0>(std::tuple<>{}, Extents);
}

auto copy_mdspan = [](auto const& src, auto& dst) {
  assert(src.extent(0) == dst.extent(0) && src.extent(1) == dst.extent(1));
  std::copy(src.data_handle(), src.data_handle() + (src.extent(0) * src.extent(1)), dst.data_handle());
};

////
/// type conversion lambdas
//
inline auto int_to_float = [](std::uint8_t Value) {
  return static_cast<float>(Value);
};
inline auto float_to_int = [](float Value) {
  return static_cast<std::uint8_t>(Value);
};

// enum Class and FNS to track action to take
//
// enum class
enum class PositionType : std::uint8_t
{
  UL,
  UR,
  LL,
  LR,
  TR,
  BR,
  LE,
  RE,
  GC
};

// FNS
//
// Class to manage pixel positions and state transitions in an image
class PixelPositionFSM
{
private:
  std::size_t Height_;            // Image height
  std::size_t Width_;             // Image width
  std::size_t Current_Position_;  // Current position in 1D space
  PositionType Current_State_;    // Current state of the pixel position

  // Transition table for default state changes
  const std::unordered_map<PositionType, PositionType> STATE_TRANSITION_TABLE = {
      {PositionType::UL, PositionType::TR},  // UL → TR
      {PositionType::TR, PositionType::UR},  // TR → UR
      {PositionType::UR, PositionType::LE},  // UR → LE
      {PositionType::LE, PositionType::GC},  // LE → GC
      {PositionType::GC, PositionType::RE},  // GC → RE
      {PositionType::RE, PositionType::LE},  // RE → LE (loop)
      {PositionType::LL, PositionType::BR},  // LL → BR
      {PositionType::BR, PositionType::LR}   // BR → LR
  };

public:
  // Constructor initializes the dimensions and starts at state UL
  PixelPositionFSM(std::size_t Height, std::size_t Width)
      : Height_(Height), Width_(Width), Current_Position_(0ul), Current_State_(PositionType::UL)
  {
  }

  // Getter for the current state
  inline auto operator()() const
  {
    return Current_State_;
  }

  // Increment operator to transition between states
  auto operator++()
  {
    auto New_Position = Current_Position_ + 1;
    auto Current_Row = Current_Position_ / Width_;
    [[maybe_unused]] auto Current_Row_Begin = Current_Row * Width_;
    auto Current_Row_End = ((Current_Row + 1) * Width_) - 1;

    // Helper lambda to perform assertions
    auto assertState = [&](PositionType state) {
      switch (state) {
        case PositionType::UL:
          assert(Current_Position_ == Current_Row_Begin);
          assert(Current_Row == 0ul);
          break;

        case PositionType::TR:
          assert(Current_Position_ > Current_Row_Begin);
          assert(Current_Position_ < Current_Row_End);
          assert(Current_Row == 0ul);
          break;

        case PositionType::UR:
          assert(Current_Position_ == Current_Row_End);
          assert(Current_Row == 0ul);
          break;

        case PositionType::LE:
          assert(Current_Position_ == Current_Row_Begin);
          assert(Current_Row > 0ul);
          assert(Current_Row < (Height_ - 1ul));
          break;

        case PositionType::GC:
          assert(Current_Position_ > Current_Row_Begin);
          assert(Current_Position_ < Current_Row_End);
          assert(Current_Row > 0ul);
          assert(Current_Row < (Height_ - 1ul));
          break;

        case PositionType::RE:
          assert(Current_Position_ == Current_Row_End);
          assert(Current_Row < (Height_ - 1ul));
          break;

        case PositionType::LL:
          assert(Current_Position_ == Current_Row_Begin);
          assert(Current_Row == (Height_ - 1ul));
          break;

        case PositionType::BR:
          assert(Current_Position_ > Current_Row_Begin);
          assert(Current_Position_ < Current_Row_End);
          assert(Current_Row == (Height_ - 1ul));
          break;

        default:
          break;
      }
    };

    // Perform assertions for the current state
    assertState(Current_State_);

    // Handle transitions using a switch statement with adornments
    switch (Current_State_) {
      case PositionType::UL:
        [[unlikely]]  // UL → TR happens only once at the very beginning
        Current_State_ = PositionType::TR;
        break;

      case PositionType::TR:
        [[unlikely]]  // TR → UR happens only once at the end of the first row
        if (New_Position == Current_Row_End) {
          Current_State_ = PositionType::UR;
        }
        break;

      case PositionType::UR:
        [[unlikely]]  // UR → LE happens only once after the first row
        Current_State_ = PositionType::LE;
        break;

      case PositionType::LE:
        [[likely]]  // LE → GC happens frequently for most left-edge pixels
        Current_State_ = PositionType::GC;
        break;

      case PositionType::GC:
        [[likely]]  // GC → RE happens frequently for most general-case pixels
        if (New_Position == Current_Row_End) {
          Current_State_ = PositionType::RE;
        }
        break;

      case PositionType::RE:
        if (Current_Row == Height_ - 2ul) {
          [[unlikely]]  // RE → LL happens only once for the second-to-last row
          Current_State_ = PositionType::LL;
        }
        else {
          [[likely]]  // RE → LE happens frequently for most right-edge pixels
          Current_State_ = PositionType::LE;
        }
        break;

      case PositionType::LL:
        [[unlikely]]  // LL → BR happens only once at the beginning of the last row
        Current_State_ = PositionType::BR;
        break;

      case PositionType::BR:
        [[unlikely]]  // BR → LR happens only once at the end of the last row
        if (New_Position == Current_Row_End) {
          Current_State_ = PositionType::LR;
        }
        break;

      default:
        [[unlikely]]  // Terminal state (LR) or unexpected state
        break;
    }
    ++Current_Position_;  // Increment the position
    return *this;
  }
};

////
/// Convert Image to greyscale
//
auto grey_scale(auto& Image_Span) -> void
{
  for (auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    // Lambda to calculate the grayscale value
    auto grey_formula = [](std::uint8_t Blue, std::uint8_t Green, std::uint8_t Red) -> std::uint8_t {
      return float_to_int(std::round((int_to_float(Blue) + int_to_float(Green) + int_to_float(Red)) / 3.0f));
    };
    // Destructure the RGBTRIPLE at the current position into Blue, Green, and Red
    auto& [Blue, Green, Red] = Image_Span[Row_Pos, Col_Pos];
    // Use the lambda to compute the grayscale value
    auto Grey = grey_formula(Blue, Green, Red);
    // Assign the grayscale value to all three color channels;
    Image_Span[Row_Pos, Col_Pos] = RGBTRIPLE{Grey, Grey, Grey};
  }
}

////
/// Convert image to sepia
//
auto sepia(auto& Image_Span) -> void
{
  // Precompute sepia coefficients
  // 0-2 blue to sepia
  // 3-5 green to sepia
  // 6-8 red to sepia
  static constexpr std::array Sepia_Coefficients = {0.131f, 0.534f, 0.272f, 0.168f, 0.686f,
                                                    0.349f, 0.189f, 0.769f, 0.393f};
  // using std::mdspan to map std::array into a 2d array,
  // Each row corresponds to one output channel (in BGR order)
  // Each row contains coefficients applied to input channels (B, G, R), in that order
  static constexpr std::mdspan Sepia_Kernel = std::mdspan(Sepia_Coefficients.cbegin(), 3, 3);

  for (auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    // Note: RGBTRIPLE stores pixels in BGR order: {Blue, Green, Red}
    // Structured bindings match that: Blue = .rgbtBlue, etc.
    auto& [Blue, Green, Red] = Image_Span[Row_Pos, Col_Pos];
    auto float_color_to_uint8_t = [](float Color) {
      return static_cast<std::uint8_t>(std::clamp(std::round(Color), 0.0f, 255.0f));
    };

    // Apply precomputed coefficients for Sepia transformation
    Image_Span[Row_Pos, Col_Pos] = RGBTRIPLE{
        float_color_to_uint8_t(Blue * Sepia_Kernel[0, 0] + Green * Sepia_Kernel[0, 1] + Red * Sepia_Kernel[0, 2]),
        float_color_to_uint8_t(Blue * Sepia_Kernel[1, 0] + Green * Sepia_Kernel[1, 1] + Red * Sepia_Kernel[1, 2]),
        float_color_to_uint8_t(Blue * Sepia_Kernel[2, 0] + Green * Sepia_Kernel[2, 1] + Red * Sepia_Kernel[2, 2])};
  }
}

////
/// XOR-swap for RGBTRIPLE channels (Blue, Green, Red)
///
// This named lambda swaps two RGBTRIPLEs using XOR logic without temporary storage.
// While this technique is elegant and minimal, it is not faster than std::swap
// on modern CPUs and has correctness caveats when lhs == rhs.
//
// Retained here for educational value — it demonstrates reversible bitwise logic
// and offers insight into data movement without auxiliary variables.
//
// Note: The main reflect() function uses std::swap due to better optimizer support
// and slightly faster performance in benchmarking.
//
inline auto xor_swap_RGBTRIPLE = [](RGBTRIPLE& lhs, RGBTRIPLE& rhs) {
  lhs.rgbtBlue ^= rhs.rgbtBlue;
  rhs.rgbtBlue ^= lhs.rgbtBlue;
  lhs.rgbtBlue ^= rhs.rgbtBlue;

  lhs.rgbtGreen ^= rhs.rgbtGreen;
  rhs.rgbtGreen ^= lhs.rgbtGreen;
  lhs.rgbtGreen ^= rhs.rgbtGreen;

  lhs.rgbtRed ^= rhs.rgbtRed;
  rhs.rgbtRed ^= lhs.rgbtRed;
  lhs.rgbtRed ^= rhs.rgbtRed;
};

////
/// Reflect image horizontally
//
auto reflect(auto& Image_Span) -> void
{
  for (auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    std::size_t Width = Image_Span.extent(1);
    if (Col_Pos < (Width / 2))  // we need to swap if not reached midway point
    {
      // xor_swap_RGBTRIPLE(Image_Span[Row_Pos, Col_Pos], Image_Span[Row_Pos, (Width - 1) - Col_Pos]);
      std::swap(Image_Span[Row_Pos, Col_Pos], Image_Span[Row_Pos, (Width - 1) - Col_Pos]);
    }
  }
}

////
/// Find edges image
//
auto edges(auto& Image_Span) -> void
{
  ////
  /// Setup Sobel GX and GY kernels
  //
  constexpr std::array<float, 9> GX_Kernel{-1.0f, 0.0f, +1.0f, -2.0f, 0.0f, +2.0f, -1.0f, 0.0f, +1.0f};
  constexpr std::array<float, 9> GY_Kernel{-1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, +1.0f, +2.0f, +1.0f};
  ////
  /// Lambda for applying the Sobel kernel
  //
  auto g_sobel_kernel = [&](const auto& Pixel_Array, const auto& Convolution) {
    std::tuple G_Sobel_Val{0.0f, 0.0f, 0.0f};  // Initialize tuple to black
    std::size_t Pos = 0;
    auto& [G_Sobel_Blue, G_Sobel_Green, G_Sobel_Red] = G_Sobel_Val;
    for (const auto& Current_Pixel : Pixel_Array) {
      auto& [PixelBlue, PixelGreen, PixelRed] = Current_Pixel;
      G_Sobel_Blue += Convolution.at(Pos) * PixelBlue;
      G_Sobel_Green += Convolution.at(Pos) * PixelGreen;
      G_Sobel_Red += Convolution.at(Pos) * PixelRed;
      ++Pos;
    };
    // Square blue, green, and red
    G_Sobel_Blue = std::pow(G_Sobel_Blue, 2.0f);
    G_Sobel_Green = std::pow(G_Sobel_Green, 2.0f);
    G_Sobel_Red = std::pow(G_Sobel_Red, 2.0f);
    return G_Sobel_Val;
  };
  ////
  /// Combine and round GX and GY
  //
  auto Combine_GX_GY = [](float GX, float GY) {
    return std::round(std::sqrt(GX * GX + GY * GY));
  };
  ////
  /// Lambda for combining GX and GY results
  //
  auto sobel_formula = [&](const auto& Pixel_Array) {
    auto GX = g_sobel_kernel(Pixel_Array, GX_Kernel);
    auto GY = g_sobel_kernel(Pixel_Array, GY_Kernel);
    auto& [Blue_GX, Green_GX, Red_GX] = GX;
    auto& [Blue_GY, Green_GY, Red_GY] = GY;

    std::tuple Intermediate{0.0f, 0.0f, 0.0f};
    auto& [Pixel_Blue, Pixel_Green, Pixel_Red] = Intermediate;

    // Combine GX and GY using square root
    Pixel_Blue = Combine_GX_GY(Blue_GX, Blue_GY);
    Pixel_Green = Combine_GX_GY(Green_GX, Green_GY);
    Pixel_Red = Combine_GX_GY(Red_GX, Red_GY);

    RGBTRIPLE Ret_Val = {};
    auto& [Ret_Blue, Ret_Green, Ret_Red] = Ret_Val;

    // Cap values to 255
    Ret_Blue = Pixel_Blue > 255.0f ? 255 : float_to_int(Pixel_Blue);
    Ret_Green = Pixel_Green > 255.0f ? 255 : float_to_int(Pixel_Green);
    Ret_Red = Pixel_Red > 255.0f ? 255 : float_to_int(Pixel_Red);

    return Ret_Val;
  };

  // Create unique_ptr to an array of RGBTRIPLE to use as a Reference copy
  auto Image_Ref = std::make_unique<RGBTRIPLE[]>(Image_Span.extent(0) * Image_Span.extent(1));
  auto Img_Ref_Span = std::mdspan(Image_Ref.get(), Image_Span.extent(0), Image_Span.extent(1));
  assert(Image_Span.extent(0) == Img_Ref_Span.extent(0) && Image_Span.extent(1) == Img_Ref_Span.extent(1));

  ////
  /// Copy image to reference span
  //
  copy_mdspan(Image_Span, Img_Ref_Span);
  ///
  // std::copy(
  //     Image_Span.data_handle(),
  //     Image_Span.data_handle() + Image_Span.extent(0) * Image_Span.extent(1),
  //     Img_Ref_Span.data_handle()
  //     );
  ///
  // for (auto [Row, Column] : index_mdspan(Image_Span)) { Img_Ref_Span[Row, Column] = Image_Span[Row, Column]; }

  const auto Image_Height = Image_Span.extent(0);
  const auto Image_Width = Image_Span.extent(1);
  PixelPositionFSM Current_Pixel_State(Image_Height, Image_Width);

  // Process each pixel in the image
  for (auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    constexpr RGBTRIPLE BLACK_TRIPLE{0, 0, 0};

    // Handle different pixel positions (UL, TR, UR, etc.)
    std::array pixel_to_compute = [&]() {
      switch (Current_Pixel_State()) {
        [[unlikely]] case PositionType::UL:
          return std::array{BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos + 1, Col_Pos],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        [[unlikely]] case PositionType::TR:
          return std::array{BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        [[unlikely]] case PositionType::UR:
          return std::array{BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos + 1, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos],
                            BLACK_TRIPLE};
        [[unlikely]] case PositionType::LE:
          return std::array{BLACK_TRIPLE, Img_Ref_Span[Row_Pos - 1, Col_Pos], Img_Ref_Span[Row_Pos - 1, Col_Pos + 1],
                            BLACK_TRIPLE, Img_Ref_Span[Row_Pos, Col_Pos],     Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            BLACK_TRIPLE, Img_Ref_Span[Row_Pos + 1, Col_Pos], Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        [[likely]] case PositionType::GC:
          return std::array{Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos],
                            Img_Ref_Span[Row_Pos - 1, Col_Pos + 1], Img_Ref_Span[Row_Pos, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos, Col_Pos],         Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos - 1], Img_Ref_Span[Row_Pos + 1, Col_Pos],
                            Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        [[unlikely]] case PositionType::RE:
          return std::array{Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos], BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos - 1],     Img_Ref_Span[Row_Pos, Col_Pos],     BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos + 1, Col_Pos - 1], Img_Ref_Span[Row_Pos + 1, Col_Pos], BLACK_TRIPLE};
        [[unlikely]] case PositionType::LL:
          return std::array{BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos - 1, Col_Pos],
                            Img_Ref_Span[Row_Pos - 1, Col_Pos + 1],
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE};
        [[unlikely]] case PositionType::BR:
          return std::array{Img_Ref_Span[Row_Pos - 1, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos - 1, Col_Pos],
                            Img_Ref_Span[Row_Pos - 1, Col_Pos + 1],
                            Img_Ref_Span[Row_Pos, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            Img_Ref_Span[Row_Pos, Col_Pos + 1],
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE};
        [[unlikely]] case PositionType::LR:
          return std::array{Img_Ref_Span[Row_Pos - 1, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos - 1, Col_Pos],
                            BLACK_TRIPLE,
                            Img_Ref_Span[Row_Pos, Col_Pos - 1],
                            Img_Ref_Span[Row_Pos, Col_Pos],
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE,
                            BLACK_TRIPLE};
        default:
          [[noreturn]] throw std::runtime_error("Unexpected PositionType");
      }
    }();
    // Apply the Sobel formula
    Image_Span[Row_Pos, Col_Pos] = sobel_formula(pixel_to_compute);
    // Update pixel state
    ++Current_Pixel_State;
  }
}

////
/// Blur image
//
auto blur(auto& Image_Span) -> void
{
  ////
  /// Create unique_ptr to an array of RGBTRIPLE to use as reference copy
  //
  std::unique_ptr<RGBTRIPLE[]> Image_Ref(new RGBTRIPLE[Image_Span.extent(0) * Image_Span.extent(1)]);

  ////
  /// Create mdspan of Image_Ref with same dimensions as Image_Span
  //
  auto Img_Ref_Span = std::mdspan(Image_Ref.get(), Image_Span.extent(0), Image_Span.extent(1));
  assert(Image_Span.extent(0) == Img_Ref_Span.extent(0) && Image_Span.extent(1) == Img_Ref_Span.extent(1));

  ////
  /// Copy image to reference span
  //
  copy_mdspan(Image_Span, Img_Ref_Span);
  ///
  // std::copy(
  //     Image_Span.data_handle(),
  //     Image_Span.data_handle() + Image_Span.extent(0) * Image_Span.extent(1),
  //     Img_Ref_Span.data_handle()
  //     );
  ///
  // for (auto [Row, Column] : index_mdspan(Image_Span)) { Img_Ref_Span[Row, Column] = Image_Span[Row, Column]; }

  ////
  /// Useful constants
  //
  const auto IMAGE_HEIGHT = Image_Span.extent(0);
  const auto IMAGE_WIDTH = Image_Span.extent(1);

  ////
  /// Pixel state machine to track position
  //
  PixelPositionFSM Current_Pixel_State(IMAGE_HEIGHT, IMAGE_WIDTH);

  ////
  /// Iterate over all pixels
  //
  for (auto [Row_Pos, Col_Pos] : index_mdspan(Image_Span)) {
    ////
    /// Declare named lambdas for channel separation
    //
    auto blue_channel_func = [](int Acc, const RGBTRIPLE& Triple) {
      return Acc + Triple.rgbtBlue;
    };
    auto green_channel_func = [](int Acc, const RGBTRIPLE& Triple) {
      return Acc + Triple.rgbtGreen;
    };
    auto red_channel_func = [](int Acc, const RGBTRIPLE& Triple) {
      return Acc + Triple.rgbtRed;
    };

    ////
    /// Blur one pixel using provided RGB channel lambdas
    //
    auto blur_pixel_formula = [&](auto& Pixels_To_Blur) -> RGBTRIPLE {
      return RGBTRIPLE{
          static_cast<std::uint8_t>(
              std::round(std::accumulate(Pixels_To_Blur.begin(), Pixels_To_Blur.end(), 0, blue_channel_func) /
                         static_cast<float>(Pixels_To_Blur.size()))),
          static_cast<std::uint8_t>(
              std::round(std::accumulate(Pixels_To_Blur.begin(), Pixels_To_Blur.end(), 0, green_channel_func) /
                         static_cast<float>(Pixels_To_Blur.size()))),
          static_cast<std::uint8_t>(
              std::round(std::accumulate(Pixels_To_Blur.begin(), Pixels_To_Blur.end(), 0, red_channel_func) /
                         static_cast<float>(Pixels_To_Blur.size())))};
    };

    ////
    /// Act on position state
    //
    switch (Current_Pixel_State()) {
      [[unlikely]] case PositionType::UL: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos, Col_Pos], Img_Ref_Span[Row_Pos, Col_Pos + 1],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos], Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::TR: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos, Col_Pos - 1], Img_Ref_Span[Row_Pos, Col_Pos],
                                     Img_Ref_Span[Row_Pos, Col_Pos + 1], Img_Ref_Span[Row_Pos + 1, Col_Pos - 1],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos], Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::UR: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos, Col_Pos - 1], Img_Ref_Span[Row_Pos, Col_Pos],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos - 1], Img_Ref_Span[Row_Pos + 1, Col_Pos]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::LE: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos], Img_Ref_Span[Row_Pos - 1, Col_Pos + 1],
                                     Img_Ref_Span[Row_Pos, Col_Pos],     Img_Ref_Span[Row_Pos, Col_Pos + 1],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos], Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[likely]] case PositionType::GC: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos],
                                     Img_Ref_Span[Row_Pos - 1, Col_Pos + 1], Img_Ref_Span[Row_Pos, Col_Pos - 1],
                                     Img_Ref_Span[Row_Pos, Col_Pos],         Img_Ref_Span[Row_Pos, Col_Pos + 1],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos - 1], Img_Ref_Span[Row_Pos + 1, Col_Pos],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::RE: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos],
                                     Img_Ref_Span[Row_Pos, Col_Pos - 1],     Img_Ref_Span[Row_Pos, Col_Pos],
                                     Img_Ref_Span[Row_Pos + 1, Col_Pos - 1], Img_Ref_Span[Row_Pos + 1, Col_Pos]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::LL: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos], Img_Ref_Span[Row_Pos - 1, Col_Pos + 1],
                                     Img_Ref_Span[Row_Pos, Col_Pos], Img_Ref_Span[Row_Pos, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::BR: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos],
                                     Img_Ref_Span[Row_Pos - 1, Col_Pos + 1], Img_Ref_Span[Row_Pos, Col_Pos - 1],
                                     Img_Ref_Span[Row_Pos, Col_Pos],         Img_Ref_Span[Row_Pos, Col_Pos + 1]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;

      [[unlikely]] case PositionType::LR: {
        std::array Pixels_To_Blur = {Img_Ref_Span[Row_Pos - 1, Col_Pos - 1], Img_Ref_Span[Row_Pos - 1, Col_Pos],
                                     Img_Ref_Span[Row_Pos, Col_Pos - 1], Img_Ref_Span[Row_Pos, Col_Pos]};
        Image_Span[Row_Pos, Col_Pos] = blur_pixel_formula(Pixels_To_Blur);
      } break;
    }

    ////
    /// Advance position state
    //
    ++Current_Pixel_State;
  }
}

#endif  // HELPERS_HXX