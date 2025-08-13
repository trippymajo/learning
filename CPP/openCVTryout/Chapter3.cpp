#include <opencv2/opencv.hpp>
#include <opencv2/core/softfloat.hpp>
#include <iostream>

using namespace std;

// Ex 1
//a.Choose a negative floating - point number.
//b.Take its absolute value, round it, and then take its ceiling and floor.
//c.Generate some random numbers.
//d.Create a floating - point cv::Point2f and convert it to an integer cv::Point.
//Convert a cv::Point to a cv::Point2f.
void firstEx()
{
  // a
  float fp = -666.06;

  // b
  cout << "Abs of fp = " << cv::abs(fp) << endl;
  cout << "Rounded of fp = " << cvRound(fp) << endl;
  cout << "Ceiling of fp = " << cvCeil(fp) << endl;
  cout << "Floor of fp = " << cvFloor(fp) << endl;

  // c
  cv::RNG rng(cv::getTickCount());
  for (int i = 0; i < 5; ++i)
    cout << "Random " << i+1 << " = " << rng.uniform(-666, 666) << endl;

  // d
  cv::Point2f p2f(12.34, 56.78);
  cv::Point p2i(12, 34);
  cout << "Point 2 Float = " << p2f << endl;
  cout << "Converted Point 2 Float to 2 int = " << (cv::Point)p2f << endl;
  cout << "Point 2 int = " << p2i << endl;
  cout << "Converted Point 2 int to 2 float = " << (cv::Point2f)p2i << endl;
}

//Compact matrix and vector types :
//a.Using the cv::Matx33f and cv::Vec3f objects(respectively), create a 3 x 3
//matrix and 3 - row vector.
//b.Can you multiply them together directly ? If not, why not?
void secondEx()
{
  // a
  cv::Matx33f matx33f (1.0f, 2.0f, 3.0f,
                       4.0f, 5.0f, 6.0f,
                       7.0f, 8.0f, 9.0f);
  cv::Vec3f vec3f     (1.0f, 2.0f, 3.0f);

  cout << matx33f << endl;
  cout << vec3f << endl;

  // Here is autocasting
  cout << "Multiplied = " << matx33f * vec3f << endl;
}

//Compact matrix and vector template types:
//a.Using the cv::Mat<> and cv::Vec<> templates(respectively), create a 3 x 3
//matrix and 3 - row vector.
//b.Can you multiply them together directly ? If not, why not?
//c.Try type - casting the vector object to a 3 x 1 matrix, using the cv::Mat<> template.
//What happens now ?
void thirdEx()
{
  cv::Mat_<int> mat(3, 3);
  mat << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

  cv::Vec<int, 3> vec(1, 2, 3);

  // Need a strict typcasting for templates.
  // cout << "Multiplied = " << mat * vec << endl;
  // 
  //still the porblem. Outdated book.
  //cv::Mat_<int> vecMat(vec, true);

  //cv::Mat_<int> vecMat(3, 1);
  //for (int i = 0; i < 3; ++i) 
  //  vecMat(i, 0) = vec[i]; // Copy values from vec to vecMat

  //cout << "Multiplied = " << mat * vecMat << endl;
}

int main(int argc, char* argv[]) 
{
  firstEx();
  secondEx();
  //thirdEx();
  return 0;
}