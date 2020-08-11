#include "DataLoader.h"

//In the constructor, iterate through the whole dataset and construct the index, which is image_path
DataLoader::DataLoader(bool test, 
                       bool toshuffle, 
					   int batchsize, 
					       filesystem::path directory) 
					  : directory(directory), 
					    toshuffle(toshuffle), 
						batchsize(batchsize)
{
	if (test)
	{ //test mode
		directory /= "testing";
	}
	else
	{ //train mode
		directory /= "training";
	}
			

	for (const auto& p : filesystem::directory_iterator(directory))
	{	//non-recursive iterator, shallowly iterate through every class
		classpath.push_back(p.path());
	}

	num_classes = classpath.size();

	for (const auto& path : classpath)//iterate through every class
	{
		vector<filesystem::path> subpath;//array of image path
		size_t class_entry_count{0};
		
		//***************
		for (const auto& file : filesystem::directory_iterator(path)) 
		{	//iterate through every image in the certain class
			++class_entry_count;
			subpath.push_back(file.path());
		}

		length_classes.push_back(class_entry_count);
		image_path.push_back(subpath);
	}

	for (const auto& p : length_classes)
	{
		dataset_length += p; //dataset_length = sum of lengths of all classes
	}

	permutation.resize(dataset_length); //Resize the vector,fill the "long array" with zero
	iota(permutation.begin(), permutation.end(), 0); //fill the vector with 0, 1, 2, ... , dataset_length-1
		
	if (toshuffle)
	{
		random_device rd; //random seed
		default_random_engine generator(rd());
		shuffle(permutation.begin(), permutation.end(), generator);//permute the "long array"
	}
}
filesystem::path DataLoader::getProcessingImagePath(size_t index) {
	return path_generator(permutation[index]).first;
}
