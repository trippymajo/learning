#include <opencv2/opencv.hpp>
#include <iostream>
#include <conio.h>

using namespace std;

#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

namespace firstEx
{
  uint g_curRow = 0;
  uint g_curCol = 0;
  // We know the final type - using template to avoid at<>()
  cv::Mat_<uchar> g_matFirstInput = cv::Mat_<uchar>::zeros(20, 10);

  //1. Create a 500 x 500 single channel uchar image with every pixel equal to zero.
  //a.Create an ASCII numeric typewriter where you can type numbers into your
  //computer and have the number show up in a 20 - pixel - high by 10 - pixel - wide
  //block.As you type, the numbers will display from left to right
  //b.Allow for arrow keys to edit each number.
  //c.Create a key that will convert the resulting image into a color image, each
  //number taking on a different color.

  /// @brief input mode, allowing changing items of matrix
  void inputMode()
  {
    int curVal = g_matFirstInput(g_curRow, g_curCol);
    while (true)
    {
      int input = -1;
      cout << "[Input Mode] Write number in range of [0, 255]" << endl;
      cout << "[Input Mode] Current number is: " << curVal << endl;
      cin >> input;

      if (input >= 0 && input <= 255)
      {
        g_matFirstInput(g_curRow, g_curCol) = static_cast<uchar>(input);
        return;
      }
    }
  }

  /// @brief print matrix
  void paintMatrix()
  {
    for (int i = 0; i < g_matFirstInput.rows; ++i)
    {
      for (int z = 0; z < g_matFirstInput.cols; ++z)
      {
        cout << static_cast<int>(g_matFirstInput(i, z));

        if (i == g_curRow && z == g_curCol)
          cout << "*";

        cout << " ";
      }
      cout << endl;
    }
  }

  /// @brief Navigation in the matrix
  void navigationMode()
  {
    while (true)
    {
      paintMatrix();

      int inputKey;
      cout << "Cuttent position is " << g_curRow << ", " << g_curCol << endl;
      cout << "[Navigation] Press <Enter> to write number, <Arrows> to navigate, <Esc> to proceed to image generation" << endl;
      inputKey = _getch();
      switch (inputKey)
      {
      case KEY_ENTER:
        inputMode();
        break;

      case KEY_ESC:
        return;

      case KEY_LEFT:
        if (g_curCol > 0)
          --g_curCol;
        break;

      case KEY_RIGHT:
        if (g_curCol < 9)
          ++g_curCol;
        break;

      case KEY_UP:
        if (g_curRow > 0)
          --g_curRow;
        break;

      case KEY_DOWN:
        if (g_curRow < 19)
          ++g_curRow;
        break;

      default:
        break;
      }
    }
  }

  void firstEx()
  {
    // Making own Matrix process
    navigationMode();

    // Image 500x500 filled with 0 uchar 1 channel (grayscale)
    cv::Mat matResizedGray;
    cv::resize(g_matFirstInput, matResizedGray, cv::Size(500, 500), 0, 0, cv::INTER_LINEAR);

    // Now 3 channel (BGR)
    cv::Mat matResizedColor;
    cv::cvtColor(matResizedGray, matResizedColor, cv::COLOR_GRAY2BGR);

    // Show output Grayscale 1 channel
    cv::imshow("Grayscale", matResizedGray);
    // Show output Grayscale 3 channel
    cv::imshow("Color", matResizedColor);
    cv::waitKey(0);
  }
  
}// end firstEx region

namespace secondEx
{
  //We want to create a function that makes it efficient to sum up rectangular
  //regions in an image by creating a statistics image where each “pixel” holds the
  //sum of the rectangle from that point to the image origin.These are called integral
  //images and by using just 4 points from the integral image, you can determine the
  //sum of any rectangle in the image.
  //    a.Create a 100 x 200 single - channel uchar image with random numbers.Create
  //a 100 x 200 single - channel float “integral image” with all members equal to
  //zero.
  //    b.Fill in each element of the integral image with the corresponding sum of the
  //rectangle from that pixel to the origin in the original uchar image.
  //    c.How can you do part b) very efficiently in one pass using the integral numbers
  //you’ve already calculated in the integral image plus the new number
  //being added in the original image ? Implement this efficient method.
  //    d.Use the integral image to rapidly calculate the sum of pixels in any rectangle
  //in the original image.
  //    e.How can you modify the integral image so that you can compute the sum of a
  //45 - degree rotated rectangle in the original image very efficiently ? Describe the
  //algorithm.
  
  void calcIntegralRotated(const cv::Mat_<uchar>& orig, cv::Mat_<float>& integral)
  {
    for (int y = 0; y < orig.rows; ++y) 
    {
      for (int x = 0; x < orig.cols; ++x) 
      {
        //R(x,y) = I(x,y) + R(x-1, y-1) + R(x-1,y) + R(x, y-1) - R(x-1,y-2);
        int pixelVal = orig(y, x);
        int top = (y > 0) ? integral(y - 1, x) : 0.0f;
        int left = (x > 0) ? integral(y, x - 1) : 0.0f;
        int topLeft = (y > 0 && x > 0) ?integral(y - 1, x - 1) : 0.0f;
        int topLeftDiagonal = (y > 1 && x > 0) ? integral(y - 2, x - 1) : 0.0f;

        integral(y, x) = pixelVal + top + left + topLeft - topLeftDiagonal;
      }
    }
  }

  float calcSumInputRect(const cv::Mat_<float>& integral, const cv::Rect& rect)
  {
    float retVal = 0.0f;
    // Check if Rect lies in original image (original same size as integral so check in integral)
    // Sumation within rectangle = IImage(Bottom Right) + IImage(Top Left) - IImage(Bottom Left) - IImage(Top Right)
    if (rect.x >= 0 && rect.y >= 0 && rect.x + rect.width <= integral.cols && rect.y + rect.height <= integral.rows)
    { // Row = y, Column = x. Here inclusive indexes, so we should do -1.
      retVal += integral(rect.y + rect.height - 1, rect.x + rect.width - 1); // Bottom Right

      if (rect.y > 0 && rect.x > 0)
        retVal += integral(rect.y - 1, rect.x - 1); // Top Left

      if (rect.x > 0)
        retVal -= integral(rect.y + rect.height - 1, rect.x - 1); // Bottom Left

      if (rect.y > 0)
        retVal -= integral(rect.y - 1, rect.x + rect.width - 1); // Top Right
    }

    return retVal;
  }

  /// @brief Effectively calculates an integral image of input image.
  /// @param orig - input image
  /// @param [OUT] integral - outputting integral image
  /// @remarks https://en.wikipedia.org/wiki/Summed-area_table
  void calcIntegralEffective(const cv::Mat_<uchar>& orig, cv::Mat_<float>& integral)
  {
    // I(x,y) = i(x,y) + I(x,y-1) + I(x-1,y) - I(x-1, y-1)
    for (int y = 0; y < orig.rows; ++y)
    {
      for (int x = 0; x < orig.cols; ++x)
      {
        float pixelVal = static_cast<float>(orig(y, x));
        float left = (x > 0) ? integral(y, x - 1) : 0.0f;
        float top = (y > 0) ? integral(y - 1, x) : 0.0f;
        float topLeft = (y > 0 && x > 0) ? integral(y - 1, x - 1) : 0.0f;

        integral(y, x) = pixelVal + top + left - topLeft;
      }
    }
  }

  /// @brief Calculates an integral image of input image.
  /// @param orig - input image
  /// @param [OUT] integral - outputting integral image
  void calcIntegralBrute(const cv::Mat_<uchar>& orig, cv::Mat_<float>& integral)
  { // O (n^4)
    for (int i = 0; i < orig.rows; ++i)
    {
      for (int z = 0; z < orig.cols; ++z)
      {
        float lastSum = 0.0f;
        for (int x = 0; x <= i; ++x) 
        {
          for (int y = 0; y <= z; ++y) 
            lastSum += orig(x, y);
        }

        integral(i, z) = lastSum;
      }
    }
  }

  void createRandImage(cv::Mat_<uchar>& mat)
  {
    cv::RNG randomizer;
    randomizer.fill(mat, cv::RNG::UNIFORM, 0, UCHAR_MAX);
  }

  void secondEx()
  {
    // a. Random values in 1-channel uchar image
    cv::Mat_<uchar>orig(100, 200);
    createRandImage(orig);
    cout << "Original image:" << endl;
    cout << orig << endl;

    // b. brute force summing integral type
    cv::Mat_<float>integralUneffective = cv::Mat_<float>::zeros(100, 200);
    calcIntegralBrute(orig, integralUneffective);

    // c. calculate integral image;
    cv::Mat_<float>integralEffective = cv::Mat_<float>::zeros(100, 200);
    calcIntegralEffective(orig, integralEffective);
    cout << "Integral image:" << endl;
    cout << integralEffective << endl;

    // d. Calculate sum of any rectangle
    cv::Rect sampleRect (40, 30, 44, 60);
    cout << sampleRect << endl;
    cout << "calculated sum of the Rect:" << endl;
    cout << calcSumInputRect(integralEffective, sampleRect) << endl;

    // e. Calculated Rotated integral image
    // W.I.P TODO Rotated area sum calculation, also feed the rotated rectangle.
    cv::Mat_<float>integralRotated = cv::Mat_<float>::zeros(100, 200);
    calcIntegralRotated(orig, integralRotated);
    cout << "Roatated integral image:" << endl;
    cout << integralEffective << endl;
  }
}

int main(int argc, char* argv[]) 
{
  //firstEx::firstEx();
  secondEx::secondEx();
  return 0;
}