# OpenCV3book folder
## Chapter1.cpp
Basic operations in OpenCV
* Scaling (pyrDown and etc)
* OpenCV Interface (imshow, trackBar and etc)
* Reading/Writing images and videos (imread, videoWriter and etc)

## Chapter2.cpp
* Data structures (Mat, vec, Mat_<> and etc)
* Arithmetic basic operations

> [!TIP]
> Use templates when the size is already known. Use Mat when the incoming image's size is unknown. Prefer template implementation over types.

## Chapter 4.cpp
* Direct changing the data in image
* Computer Vision operations

# Integral image
Will be used later to implement tracking, and area detections.

[Integral image Wiki](https://en.wikipedia.org/wiki/Summed-area_table)  
[Haar response using integral image Wiki](https://en.wikipedia.org/wiki/Viola%E2%80%93Jones_object_detection_framework)

**<ins>Integral image claculation:</ins>**
```
I(x,y) = i(x,y) + I(x,y-1) + I(x-1,y) - I(x-1, y-1)
```
The sum of the any rectangle in image. Is crucial for area detections. Also should remember that while calculating sum it is an inclusive indexing, meaning you should substruct - 1.
**<ins>Sum within rectangle area claculation:</ins>**
```
Sumation within rectangle = IImage(Bottom Right) + IImage(Top Left) - IImage(Bottom Left) - IImage(Top Right)
```
**<ins>Rotated integral calculation:</ins>**
```
R(x,y) = I(x,y) + R(x-1, y-1) + R(x-1,y) + R(x, y-1) - R(x-1,y-2);
```
**<ins>Sum within rectangle rotated at 45 degree area claculation:</ins>**
```

```
