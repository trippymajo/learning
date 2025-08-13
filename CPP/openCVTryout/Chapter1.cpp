#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

bool g_fRun = true;
int g_dSliderPos = 0;

int main(int argc, char* argv[]) 
{
  namedWindow("Input", WINDOW_AUTOSIZE);
  namedWindow("Output", WINDOW_AUTOSIZE);

  VideoCapture capture(argv[1]);
  double fps = capture.get(CAP_PROP_FPS);

  Size size(
    (int)capture.get(CAP_PROP_FRAME_WIDTH),
    (int)capture.get(CAP_PROP_FRAME_HEIGHT)
  );

  cv::createTrackbar("Downsample Factor", "Input", &g_dSliderPos, 7);

  VideoWriter writer;
  writer.open(argv[2], VideoWriter::fourcc('H', '2', '6', '4'), fps, size);
  Mat pyrdown_frame, bgr_frame, output_frame;

  for (;;) 
  {
    if (g_fRun)
    {
      capture >> bgr_frame;

      if (bgr_frame.empty())
        break; // end if done

      output_frame = bgr_frame;
      // 1 pos = 2x downsample
      // 7 pos (maximum) = 8x downsample
      if (g_dSliderPos > 0)
      {
        for (int i = g_dSliderPos; i > 0; --i)
        {
          pyrDown(output_frame, pyrdown_frame);
          output_frame = pyrdown_frame;
        }
        // Resizing frame to the original size;
        resize(output_frame, output_frame, size);
      }

      imshow("Input", bgr_frame);
      imshow("Output", output_frame);
      writer << output_frame;
    }

    char c = (char)waitKey(10);
    if (c == 's') // single step
    {
      g_fRun = false;
      cout << "Single step, run = " << g_fRun << endl;
    }
    if (c == 'r') // run mode
    {
      g_fRun = true;
      cout << "Run mode, run = " << g_fRun << endl;
    }
    if (c == 27) // ESC
      break;
  }
  capture.release();
  writer.release();

  return 0;
}