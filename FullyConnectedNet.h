/*The core component*/

#include <iostream>
#include <utility>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using std::vector;
using std::pair;

pair<double, Mat> softmax_loss(Mat a, Mat b);
pair<Mat, pair<Mat, Mat>> affine_forward(Mat x, Mat w);
pair<Mat, Mat> affine_backward(Mat dout, pair<Mat, Mat> cache);
pair<Mat, Mat> relu_forward(Mat x);
Mat relu_backward(Mat dout, Mat cache);
pair<Mat, pair<pair<Mat, Mat>, Mat>> affine_relu_forward(Mat x, Mat w);
pair<Mat, Mat> affine_relu_backward(Mat dout, pair<pair<Mat, Mat>, Mat> cache);

class FullyConnectedNet
{
	friend class Solver;
    vector<int> hiddendims;
    vector<Mat> weights;
    int num_layers; //hiddendims.size() == num_layers - 1
        //the last element of hiddendims is hiddendims[num_layers - 2]
    int input_dim{784}; //784 = 1*28*28 in the MNIST dataset
    int num_classes{0};
    double reg{0.0};
    double weight_scale{1e-2};
    int dtype{CV_64F}; //double precision data type

public:
    FullyConnectedNet(vector<int> hiddendims,
                      int input_dim = 784, //28*28*1 grayscale
                      int num_classes = 10,
                      double reg = 0.0,
                      double weight_scale = 1e-2,
                      int dtype = CV_64F);
    void print()const;
    void addLayer(int hiddendim);
	int predict(Mat x)const;
    Mat loss(Mat x)const;
    pair<double, vector<Mat>> loss(Mat x, Mat y);//train mode
};