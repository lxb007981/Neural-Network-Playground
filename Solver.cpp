#include "Solver.h"


Solver::Solver(double lr_decay, 
               double lr_rate, 
			   int num_epochs, 
			       FullyConnectedNet* modelp, 
				   DataLoader* trainDataLoaderp, 
				   DataLoader* testDataLoaderp)
	          :lr_decay(lr_decay), 
	           lr_rate(lr_rate), 
	           num_epochs(num_epochs), 
	           modelp(modelp), 
	           trainDataLoaderp(trainDataLoaderp), 
	           testDataLoaderp(testDataLoaderp)
{
    //Constructor of Solver class
}

void Solver::_reset() {
	//Set up some book-keeping variables for optimization
	
	epoch = 0;
	best_test_acc = 0.0;
	best_weights.clear();
	train_loss_history.clear();
	test_loss_history.clear();
}

/*
void Solver::_save_checkpoint() {//TODO
	ofstream myfile;
	string filename{ string{"epoch_"}+to_string(epoch)};
	myfile.open(filename);
	myfile << "Writing this to a file.\n";
	myfile.close();
}*/

double Solver::_step(size_t batch_index) {
	//Make a single gradient update
	//This function will be called by train() later

	//Make a minibatch of training data
	std::pair<Mat, Mat> batch_label{ trainDataLoaderp->operator[](batch_index) };

	//Compute loss and gradient
	pair<double, vector<Mat>> result = modelp->loss(batch_label.first, batch_label.second);	//train_loss_history.push_back(result.first);
	
	size_t size{ result.second.size() };//4

	//Perform a single stochastic gradient descent
	for (int i{ 0 }; i < size; ++i) {
		modelp->weights[i] -= lr_rate * result.second[size - 1 - i];
	}
	
	return result.first;
}

double Solver::_testDuringTraining(size_t index) {
    //Compute the loss
	//This function will be called by train() later
	
	//use index = (iterations)%(num_of_test_iterations)th testDataSet to test the model.	
	double loss{ 0.0 };
	
	//Compute loss on test dataset every 5 iterations during training
	std::pair<Mat, Mat> batch_label{ testDataLoaderp->operator[](index) };
	loss =  modelp->loss(batch_label.first,batch_label.second).first;

	return loss;
}

void Solver::copyWeights(vector<Mat>src, vector<Mat>dest) {
	//Make a deep copy of the optim_config for each parameter

	dest.clear();
	
	for (vector<Mat>::const_iterator it{ src.begin() }; it != src.end(); ++it) {
		dest.push_back((*it).clone());
	}
	
	//std::copy(src.begin(), src.end(), dest);
}

double Solver::check_accuracy(FullyConnectedNet* modelp, DataLoader* testDataLoaderp) {
    //Check accuracy on the provided data
	//need to specify batchsize when initializing testDataLoader
	
	size_t iteration = testDataLoaderp->getNumOfIteration();
	double acc{ 0.0 };
	int count{ 0 };
	
	//Compute predictions in batches
	for (size_t i{ 0 }; i < iteration; ++i) {
		
		std::pair<Mat, Mat> batch_label{ testDataLoaderp->operator[](i) };
		Mat scores{ modelp->loss(batch_label.first) };
		
		for (size_t b{ 0 }; b < testDataLoaderp->getBatchSize(); ++b)
		{
			Point max;
			minMaxLoc(scores.row(b), nullptr, nullptr, nullptr, &max);
			int a = (max.x == batch_label.second.at<uchar>(b, 0));
			count += a;
		}
		
		acc += count / testDataLoaderp->getBatchSize();
	}
	
	//Taking average of the result and return
	acc /= iteration;
	return acc;
}

void Solver::train() {
	
	size_t iterations_per_epoch = trainDataLoaderp->getNumOfIteration();//60000/32=1875
	size_t num_of_test_iterations{ testDataLoaderp->getNumOfIteration() };//1000/32 = 312.5
	
	//size_t num_iterations{ num_epochs * iterations_per_epoch };
		
	for (size_t e{ 0 }; e < num_epochs; ++e)
	{
		//Check train and val accuracy at the first and the last iteration
		//and at the end of each epoch		
		for (size_t t{ 0 }; t < iterations_per_epoch; ++t) {
			
			_step(t);
			
			if (t % 5 == 0) {			
				//Compute the test loss every 5 iterations
				double testLoss = _testDuringTraining(t % num_of_test_iterations);
				test_loss_history.push_back(testLoss);
			}
		}
		
		double test_acc{ check_accuracy(modelp, testDataLoaderp) };
		
		//Keep track of the best model
		if (test_acc > best_test_acc) {
			best_test_acc = test_acc;
			copyWeights(modelp->weights, best_weights);
		}
		
		//At the end of each epoch, decay the learning rate
		lr_rate *= lr_decay;
	}
}


