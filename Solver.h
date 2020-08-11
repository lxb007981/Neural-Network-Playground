#include "FullyConnectedNet.h"
#include "DataLoader.h"
#include <fstream>
#include <string>

class Solver {
	/*
	A Solver encapsulates all the logic necessary for training classification
	models except data loading. The Solver performs stochastic gradient descent.

	The solver accepts both training and testing data and labels so it can
	periodically check classification accuracy on both training and testing
	data to watch out for overfitting.

	To train a model, we first construct a Solver class, passing the
	model, dataloader with dataset, and various options (learning rate, etc) to the
	constructor. We then call the train() function to run the optimization
	procedure and train the model.

	After the train() function returns, model.weights will contain the parameters
	that performed best on the testing set over the course of training.
	In addition, the variable solver.train_loss_history will contain a list of all
	losses encountered during training.
    */
	
	friend class Widget;
	
	FullyConnectedNet*modelp;
	DataLoader* trainDataLoaderp;
	DataLoader* testDataLoaderp;
	
	double lr_decay{ 1.0 };
	double lr_rate{ 1e-3 };
	
	int num_epochs{ 10 };
	int epoch{ 0 };
	
	vector<double> train_loss_history;
	vector<double> test_loss_history;
	double best_test_acc{ -1.0 };
	vector<Mat> best_weights;
	
	//Private functions that will be used in train()
	void _reset();
	double _step(size_t batch_index);

private:
	double _testDuringTraining(size_t index);
	//void _save_checkpoint();//TODO
	void copyWeights(vector<Mat>src,vector<Mat>dest);

public:
	Solver(double lr_decay,
	       double lr_rate, 
		   int num_epochs, 
		       FullyConnectedNet* modelp,
			   DataLoader* trainDataLoaderp,
			   DataLoader* testDataLoaderp);			   
	
	double check_accuracy(FullyConnectedNet* modelp, DataLoader* testDataLoaderp);
	void train();
};