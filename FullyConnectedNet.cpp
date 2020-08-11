#include "FullyConnectedNet.h"

//Constructor,initialize the variables
FullyConnectedNet::FullyConnectedNet(vector<int> hiddendims,
									 int input_dim, //28*28*1 grayscale
									 int num_classes ,
									 double reg ,
									 double weight_scale ,
									 int dtype ) 
									:hiddendims(hiddendims), 
									 input_dim(input_dim), 
									 reg(reg), 
									 weight_scale(weight_scale), 
									 dtype(dtype)
{
	//
	if (hiddendims.empty()) {

		num_layers = 1;
		Mat first_layer(input_dim, num_classes, CV_64F);
		randn(first_layer, 0, weight_scale);
		weights.push_back(first_layer);
		return;

	}
	
	num_layers = hiddendims.size() + 1;
	Mat first_layer(input_dim, hiddendims[0], CV_64F);
	randn(first_layer, 0, weight_scale);
	
	weights.push_back(first_layer);
	
	//
	for (int i = 0; i < num_layers - 2; ++i)
	{//i == num_layers - 3, i + 1 == num_layers - 2
		weights.push_back(Mat(hiddendims[i], hiddendims[i + 1], CV_64F)); //i == num_layers - 3, i + 1 == num_layers - 2
		randn(weights.back(), 0, weight_scale);
	}

	//
	weights.push_back(Mat(hiddendims[num_layers - 2], num_classes, CV_64F));
	randn(weights.back(), 0, weight_scale);
}

void FullyConnectedNet::print ()const
{
	//
	for (auto it : weights)
	{
		std::cout << it << std::endl;
	}
}

void FullyConnectedNet::addLayer(int hiddendim) 
{
	//add a new layer as the last layer
	
	hiddendims.push_back(hiddendim);
	++num_layers;
	weights.push_back(Mat(hiddendims[num_layers - 2], num_classes, CV_64F));
	randn(weights.back(), 0, weight_scale);
}

Mat FullyConnectedNet::loss(Mat x) const
{	//test mode
	vector<Mat> out;
	out.push_back(x);
	//vector<pair<pair<Mat, Mat>, Mat>> cache;	//no ndde in test mode
	//pair<Mat, Mat> last_layer_cache;			//no ndde in test mode
	Mat scores;
	for (int i = 0; i < num_layers; ++i)
	{
		if (i == num_layers - 1)
		{
			pair<Mat, pair<Mat, Mat>> result = affine_forward(out[i], weights[i]);
			scores = result.first;
			//last_layer_cache = result.second;	//no need in test mode
		}
		else
		{
			pair<Mat, pair<pair<Mat, Mat>, Mat>> result = affine_relu_forward(out[i], weights[i]);
			out.push_back(result.first);
		}
		//cache.push_back(result.second); //no need in test mode
	}
	return scores;
}

pair<double, vector<Mat>> FullyConnectedNet::loss(Mat x, Mat y) //x is input [N, D]
{	//train mode												//y is table [N, 1]
	vector<Mat> out; 
	out.push_back(x);
	vector<pair<pair<Mat, Mat>, Mat>> cache;
	pair<Mat, Mat> last_layer_cache;
	Mat scores;
	
	for (int i = 0; i < num_layers; ++i)
	{
		if (i == num_layers - 1)
		{
			pair<Mat, pair<Mat, Mat>> result = affine_forward(out[i], weights[i]);
			scores = result.first;
			last_layer_cache = result.second;
		}
		else
		{
			pair<Mat, pair<pair<Mat, Mat>, Mat>> result = affine_relu_forward(out[i], weights[i]);
			out.push_back(result.first);
			cache.push_back(result.second);
		}
	}
	
	double loss{0.0};
	pair<double, Mat> softmax_result = softmax_loss(scores, y);
	loss = softmax_result.first;
	Mat dx = softmax_result.second;	
	vector<Mat> grads;
	double reg_loss{0.0};
	
	for (auto i : weights)
	{
		Mat ipow2;
		pow(i, 2, ipow2);
		reg_loss += sum(ipow2).val[0];
	}
	
	Mat dw;
	loss += 0.5 * reg * reg_loss;
	
	for (int i{0}; i <= cache.size(); ++i)
	{ //cache.size() == num_layers - 1
		if (i == 0)
		{
			pair<Mat, Mat> affine_backward_result = affine_backward(dx, last_layer_cache);
			dx = affine_backward_result.first;
			dw = affine_backward_result.second;
		}
		else
		{
			pair<Mat, Mat> affine_relu_backward_result = affine_relu_backward(dx, cache[num_layers - 1 - i]);
			dx = affine_relu_backward_result.first;
			dw = affine_relu_backward_result.second;
		}
		grads.push_back(dw);
	}
	pair<double, vector<Mat>> result(loss, grads);
	return result;
}

int FullyConnectedNet::predict(Mat x)const {//x is [1,D]
	Mat scores{ loss(x) };
	Point max;
	minMaxLoc(scores.row(0), nullptr, nullptr, nullptr, &max);
	return max.x; //the xth class
}

pair<double, Mat> softmax_loss(Mat a, Mat b)
{
	/*
	Computes the loss and gradient for softmax classification.

	Inputs:
	- x: Input data, of shape (N, C) where x[i, j] is the score for the jth
	  class for the ith input.
	- y: Vector of labels, of shape (N,) where y[i] is the label for x[i] and
	  0 <= y[i] < C

	Returns a pair of:
	- loss: Scalar giving the loss
	- dx: Gradient of the loss with respect to x
	*/

	//make a deep copy, not touch the original memory.
	Mat x = a.clone(); //[N, C] scores for each class
	Mat y = b.clone(); //[N, 1]

	double loss_sum{0};
	vector<Mat> dx_rows;
	for (int i{0}; i < x.rows; ++i)
	{
		double row_max{0.0};
		minMaxLoc(x.row(i), nullptr, &row_max);
		x.row(i) -= row_max; //numeric stability
		Mat exp_row_i;
		exp(x.row(i), exp_row_i);
		Scalar z = sum(exp_row_i);
		Mat log_probs = x.row(i) - log(z.val[0]); //logarithmic arithmetic equals division
		double loss = -log_probs.at<double>(y.at<uchar>(i)); //y is of CV_8U
		loss_sum += loss;
		Mat probs;
		exp(log_probs, probs);
		Mat dx_row = probs.clone();
		dx_row.at<double>(y.at<uchar>(i)) -= 1;
		dx_rows.push_back(dx_row);
	}

	loss_sum /= x.rows; //the loss_sum is computed by a batch of samples
	Mat dx;
	vconcat(dx_rows, dx);
	dx /= x.rows; //The gradients are computed by a batch of samples 

	pair<double, Mat> result(loss_sum, dx);

	return result;
}

pair<Mat, pair<Mat, Mat>> affine_forward(Mat x, Mat w)
{ //return (out, cache)
	/*
	Computes the forward pass for an affine (fully-connected) layer.

	The input x has shape (N, D) and contains a minibatch of N
	examples, where each example x[i] has shape D, D = d_1 * ... * d_k, and
	then transform it to an output vector of dimension M.

	Inputs:
	- x: A numpy array containing input data, of shape (N, D)
	- w: A numpy array of weights, of shape (D, M)


	Returns a pair of:
	- out: output, of shape (N, M)
	- cache: (x, w)
	*/
	Mat out = x * w;
	pair<Mat, Mat> cache(x, w);
	pair<Mat, pair<Mat, Mat>> result(out, cache);
	return result;
}

pair<Mat, Mat> affine_backward(Mat dout, pair<Mat, Mat> cache)
{
	/*
	Computes the backward pass for an affine layer.
	Inputs:
	- dout: Upstream derivative, of shape (N, M)
	- cache: pair of:
	  - x: Input data, of shape (N, D)
	  - w: Weights, of shape (D, M)

	Returns a tuple of:
	- dx: Gradient with respect to x, of shape (N, D)
	- dw: Gradient with respect to w, of shape (D, M)
	*/

	Mat dx = dout * cache.second.t();
	Mat dw = cache.first.t() * dout;
	pair<Mat, Mat> result(dx, dw);
	return result;
}

pair<Mat, Mat> relu_forward(Mat x)
{
	/*
	Computes the forward pass for a layer of rectified linear units (ReLUs).

	Input:
	- x: Inputs, of any shape

	Returns a tuple of:
	- out: Output, of the same shape as x
	- cache: x
	*/

	Mat relu = (x > 0) / 255;
	relu.convertTo(relu, CV_64F);
	Mat out = relu.mul(x);
	Mat cache(x);
	pair<Mat, Mat> result(out, cache);
	return result;
}

Mat relu_backward(Mat dout, Mat cache)
{
	/*
	Computes the backward pass for a layer of rectified linear units (ReLUs).

	Input:
	- dout: Upstream derivatives, of any shape
	- cache: Input x, of same shape as dout

	Returns:
	- dx: Gradient with respect to x
	*/

	Mat relu = (cache > 0) / 255;
	relu.convertTo(relu, CV_64F);
	Mat dx = relu.mul(dout);
	return dx;
}

pair<Mat, pair<pair<Mat, Mat>, Mat>> affine_relu_forward(Mat x, Mat w)
{
	pair<Mat, pair<Mat, Mat>> product = affine_forward(x, w);
	pair<Mat, Mat> relu_product = relu_forward(product.first);
	pair<pair<Mat, Mat>, Mat> cache(product.second, relu_product.second);
	pair<Mat, pair<pair<Mat, Mat>, Mat>> result(relu_product.first, cache);
	return result;
}

pair<Mat, Mat> affine_relu_backward(Mat dout, pair<pair<Mat, Mat>, Mat> cache)
{
	Mat da = relu_backward(dout, cache.second);
	pair<Mat, Mat> backward_result = affine_backward(da, cache.first); //dx, dw
	return backward_result;
}
