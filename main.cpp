#include <iostream>
#include <cmath>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <iterator>
#include <random>

using namespace cv;
using namespace std;
//NOTE: on line 164,201 !!!





//Helper functions
int* random_numbers(int min,int max){
    const int range_from  = min;
    const int range_to    = max;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    int* random_array = new int[40];
    for (int i = 0; i < 40; ++i){
        random_array[i] = distr(generator);
    }
    return random_array;
}


cv::Mat passImage(cv::Mat img){
    return img;
}

cv::Mat passImagePath(string path)
{
    cv::Mat tmp = imread(path,1);
    return tmp;
}



// function functions ?????
float check_contrast_value(float rgb1[], float rgb2[]) {
    float x1, x2, r1,g1,b1,r2,g2,b2,tmp,contrast_ratio;

    for (int i = 0; i < 3; i++) {
        x1 = rgb1[i];
        x2 = rgb2[i];
        x1 = x1/255.0;
        x2 = x2/255.0;

        //check if normalized values are between 0 and 1 if not throw exception
        if ((0.0 <= x1 <= 1.0) || (0.0 <= x2 <= 1.0)) {
            if (x1 <= 0.03928) {
                x1 = x1 / 12.92;
                if (i == 0) {
                    r1 = x1;
                }
                if (i == 1) {
                    g1 = x1;
                }
                if (i == 2) {
                    b1 = x1;
                }
            } else if (x1 > 0.03928) {
                tmp = ((x1 + 0.055) / 1.055);
                x1 = pow(tmp, 2.4);
                if (i == 0) {
                    r1 = x1;
                }
                if (i == 1) {
                    g1 = x1;
                }
                if (i == 2) {
                    b1 = x1;
                }
            }
            if (x2 <= 0.03928) {
                x2 = x2 / 12.92;
                if (i == 0) {
                    r2 = x2;
                }
                if (i == 1) {
                    g2 = x2;
                }
                if (i == 2) {
                    b2 = x2;
                }
            } else if (x2 > 0.03928) {
                tmp = ((x2 + 0.055) / 1.055);
                x2 = pow(tmp, 2.4);
                if (i == 0) {
                    r2 = x2;
                }
                if (i == 1) {
                    g2 = x2;
                }
                if (i == 2) {
                    b2 = x2;
                }
            }

        } else{
            throw invalid_argument("value not in range of 0 -1");
        }
    } //loop ends here;


    x1 = ((r1 * 0.2126)+ (g1 * 0.7152)+ (b1 * 0.0722));
    x1 = x1+0.05;
    x2 = ((r2 * 0.2126)+ (g2 * 0.7152)+ (b2 * 0.0722));
    x2 = x2+0.05;

    if (x1>x2){
         contrast_ratio = x1/x2;
    } else if(x2>=x1){
        contrast_ratio = x2/x1;
    }


    return contrast_ratio;
}


float ImageContrast(Mat received_img, float coords[], float color[]){
    int x_max,y_max,x1,y1,x2,y2 = 0;
    float contrast_ratio_arr[40];
    float textColor[3];
    float pixelColor[3];
    float b,g,r, arr_avg = 0;
    float arr_sum = 0;
    int RandPixelPosX[40];
    int RandPixelPosY[40];

    x1 = int(coords[0]);y1 = int(coords[1]);x2 = int(coords[2]);y2 = int(coords[3]);
    Mat img = received_img;

    for(int i =0;i<3; i++){
        textColor[i] = float(color[i]);
    }

    Mat image_part (img, Rect(x1, y1, x2, y2) );
    x_max = image_part.rows;
    y_max = image_part.cols;

    int* arr_ptr_X = random_numbers(0,x_max);
    for (int i=0;i<40;i++){
        RandPixelPosX[i] = arr_ptr_X[i];
    }

    int* arr_ptr_Y = random_numbers(0,y_max);
    for (int i=0;i<40;i++){
        RandPixelPosY[i] = arr_ptr_Y[i];
    }

    for(int i = 0; i<40; i++){
        r = image_part.at<cv::Vec3b>(RandPixelPosX[i],RandPixelPosY[i])[0];
        g = image_part.at<cv::Vec3b>(RandPixelPosX[i],RandPixelPosY[i])[1];
        b = image_part.at<cv::Vec3b>(RandPixelPosX[i],RandPixelPosY[i])[2];
        //since opencv reads color channels as BGR, i switched the places to feed RGB to check_contrast_value
        pixelColor[0] = float(b);
        pixelColor[1] = float(g);
        pixelColor[2] = float(r);

        //call check_contrast_value
       contrast_ratio_arr[i] = check_contrast_value(pixelColor,textColor);
        arr_sum = arr_sum + contrast_ratio_arr[i];
        arr_avg = arr_sum/40;

    }
    return arr_avg;
}


int main() {
    float contrast_ratio = 0.0;
    //TEST CASES
    float test_color1[3] = {126, 211, 33};
    float test_color2[3] = {189, 31, 31};

    float test_color3[3] = {249, 137, 125};
    float test_color4[3] = {219, 4, 4};
    float test_color5[3] = {126, 211, 33};
    float test_color6[3] = {189, 31, 31};

    float black[3] = {0,0,0};
    float white[3] = {255, 255, 255};
    float white_bgr[3] = {255,255,255};

    contrast_ratio = check_contrast_value(test_color3,black);
    std::cout << contrast_ratio << std::endl;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    float test_rect_coords[4] = {10, 45, 35, 65};
//   read test image to use in ImageContrast
    Mat test_img = imread("./test.jpg",1);
//    when using with opencv pass BGR values instead of RGB
    float tmpx = ImageContrast(test_img,test_rect_coords,white_bgr);
    cout<<tmpx<<" is the average contrast"<<endl;
    return 0;
}