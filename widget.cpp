#include "widget.h"

//constructor
Widget::Widget(QWidget* parent)
	: QWidget(parent)
{
	mainlayout = new QVBoxLayout;
	createBar();
	create1stLayer();
	create2ndLayer();
	create3rdLayer();
	setLayout(mainlayout);
	setWindowTitle(tr("Neural NetWork Playground"));
}

//Qt can automatically collect garbage memory, so empty destructor
Widget::~Widget()
{
}

//set default size of the window
QSize Widget::sizeHint() const
{
	return QSize(800, 400);
}

//Create menu bar
void Widget::createBar()
{

	aboutAction = new QAction(tr("&About..."), this);
	connect(aboutAction, &QAction::triggered, this, &Widget::about);
	menu_bar = new QMenuBar();
	file_menu = menu_bar->addMenu(tr("&File"));
	edit_menu = menu_bar->addMenu(tr("&Edit"));
	help_menu = menu_bar->addMenu(tr("&Help"));
	help_menu->addAction(aboutAction);
	exit_action = new QAction(tr("&Exit"), this);
	connect(exit_action, &QAction::triggered, this, &Widget::exit);
	file_menu->addAction(exit_action);
	mainlayout->addWidget(menu_bar);
}

//Create the top layout
void Widget::create1stLayer()
{

	runButton = new QPushButton(tr("Run"));
	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	reset = new QPushButton(tr("Reset"));
	connect(reset, SIGNAL(clicked()), this, SLOT(resetAll()));
	epoch = new QLabel(tr("Epoch"));
	epoch_value = new QLineEdit(tr("Enter number of epoch (like 10)"));
	learning_rate = new QLabel(tr("Learning Rate"));
	learning_rate_value = new QLineEdit(tr("Enter learning rate (like 1e-3)"));
	regularation_strength = new QLabel(tr("Regularation Strength"));
	regularation_strength_value = new QLineEdit(tr("Enter regularation strength (like 1)"));
	batchsize = new QLabel(tr("Batchsize"));
	batchsize_value = new QLineEdit(tr("Enter batchsize (like 32)"));
	weight_scale = new QLabel(tr("Weight Scale"));
	weight_scale_value = new QLineEdit(tr("Enter weight scale (like 2e-6)"));
	lr_decay = new QLabel(tr("Learning Rate Decay"));
	lr_decay_value = new QLineEdit(tr("Learning Rate Decay Coefficient (like 0.99)"));

	rightupperlayout = new QGridLayout;
	rightupperlayout->addWidget(epoch, 0, 0);
	rightupperlayout->addWidget(epoch_value, 1, 0);
	rightupperlayout->addWidget(learning_rate, 0, 1);
	rightupperlayout->addWidget(learning_rate_value, 1, 1);
	rightupperlayout->addWidget(regularation_strength, 0, 2);
	rightupperlayout->addWidget(regularation_strength_value, 1, 2);
	rightupperlayout->addWidget(batchsize, 2, 0);
	rightupperlayout->addWidget(batchsize_value, 3, 0);
	rightupperlayout->addWidget(weight_scale, 2, 1);
	rightupperlayout->addWidget(weight_scale_value, 3, 1);
	rightupperlayout->addWidget(lr_decay, 2, 2);
	rightupperlayout->addWidget(lr_decay_value, 3, 2);

	toplayout = new QHBoxLayout;
	toplayout->addWidget(runButton);
	toplayout->addWidget(reset);
	toplayout->addLayout(rightupperlayout);

	mainlayout->addLayout(toplayout);
}


void Widget::create2ndLayer()
{
	addButton = new QPushButton(tr("add"));
	minusButton = new QPushButton(tr("remove"));
	connect(addButton, &QAbstractButton::clicked, this, &Widget::createLayer);
	connect(minusButton, &QAbstractButton::clicked, this, &Widget::removeLayer);
	hiddenlayer = new QLabel(tr("0 Hidden Layer"));
	shuffleCheckbox = new QCheckBox("Shuffle the Data(Recommended)", this);

	middlelayout = new QHBoxLayout;
	middlelayout->addWidget(addButton);
	middlelayout->addWidget(minusButton);
	middlelayout->addWidget(hiddenlayer);
	middlelayout->addWidget(shuffleCheckbox);
	middlelayout->setStretchFactor(addButton, 1);
	middlelayout->setStretchFactor(minusButton, 1);
	middlelayout->setStretchFactor(hiddenlayer, 5);
	middlelayout->setStretchFactor(shuffleCheckbox, 5);
	mainlayout->addLayout(middlelayout);
}

void Widget::create3rdLayer()
{
	bottomlayout = new QHBoxLayout;
	bottomleftlayout = new QVBoxLayout;
	testmode = new Switch(tr("Test"));
	connect(testmode, SIGNAL(stateChanged(int)), this, SLOT(switchMode(int)));
	browseButton = new QPushButton(tr("Browse"));
	connect(browseButton, &QAbstractButton::clicked, this, &Widget::browse);
	directoryComboBox = new QComboBox;
	directoryComboBox->setEditable(true);
	directoryComboBox->addItem(tr("Click Browse button to locate the dataset."));
	//in Test mode, print "Click Browse button to upload a square digit image."
	directoryComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	bottommiddlelayout = new QVBoxLayout;
	bottomrightlayout = new QVBoxLayout;
	internallayout = new QHBoxLayout;
	output = new QLabel(tr("Output: Red/Blue line represents Training/Testing loss"));
	losslayout = new QGridLayout;
	test_loss = new QLabel(tr("Test loss"));
	train_loss = new QLabel(tr("Training loss"));
	test_loss_value = new QLabel(QString::number(0));
	train_loss_value = new QLabel(QString::number(0));
	bottommiddlebottomlayout = new QHBoxLayout;

	bottomleftlayout->addWidget(testmode);
	bottomleftlayout->addWidget(browseButton);

	bottommiddlebottomlayout->addSpacing(800);
	bottommiddlelayout->addSpacing(5);
	bottommiddlelayout->addLayout(internallayout);
	bottommiddlelayout->addLayout(bottommiddlebottomlayout);
	bottomrightlayout->addWidget(output);
	/*  test loss label  */
	losslayout->addWidget(test_loss, 0, 0, 1, 1); 
	//addWidget(QWidget *widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::Alignment())
	
	/*  train loss label  */

	losslayout->addWidget(train_loss, 1, 0, 1, 1);

	losslayout->addWidget(test_loss_value, 0, 1, 1, 1);
	losslayout->addWidget(train_loss_value, 1, 1, 1, 1);

	/*  loss curve  */
	loss_Waveformdisplay = new QCustomPlot(this);

	/*  set size  */
	loss_Waveformdisplay->setMinimumSize(QSize(300, 200));

	loss_Waveformdisplay->yAxis->setVisible(true); //left-axis
	loss_Waveformdisplay->xAxis->setVisible(true); //bottom-axis

	losslayout->addWidget(loss_Waveformdisplay, 0, 3, 1, 1);

	//Change image according to the comboBox input.
	connect(directoryComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(changeShowImage(QString)));

	/*  add image  */
	uploadImage = new QLabel(tr("show image here."));
	uploadImage->setMinimumSize(QSize(300, 300));
	uploadImage->setAlignment(Qt::AlignRight);

	bottomrightlayout->addLayout(losslayout);
	bottomrightlayout->addWidget(uploadImage);

	bottomlayout->addLayout(bottomleftlayout);
	bottomlayout->addLayout(bottommiddlelayout);
	bottomlayout->addLayout(bottomrightlayout);
	bottomrightlayout->setStretchFactor(output, 1);
	bottomrightlayout->setStretchFactor(losslayout, 2);
	bottomrightlayout->setStretchFactor(uploadImage, 5);

	mainlayout->addWidget(directoryComboBox);
	mainlayout->addLayout(bottomlayout);

	this->showWaveInit();
}


void Widget::showWaveInit()
{

	loss_Waveformdisplay->addGraph();
	loss_Waveformdisplay->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph //test

	loss_Waveformdisplay->addGraph();
	loss_Waveformdisplay->graph(1)->setPen(QPen(Qt::red)); // line color blue for first graph //train
	// configure right and top axis to show ticks but no labels:
	loss_Waveformdisplay->xAxis2->setVisible(true);			//top-axis
	loss_Waveformdisplay->xAxis2->setTickLabels(false);	
	loss_Waveformdisplay->yAxis2->setVisible(true);			//right-axis
	loss_Waveformdisplay->yAxis2->setTickLabels(false);

	// make left and bottom axes always transfer their ranges to right and top axes:
	connect(loss_Waveformdisplay->xAxis, SIGNAL(rangeChanged(QCPRange)), loss_Waveformdisplay->xAxis2, SLOT(setRange(QCPRange)));
	connect(loss_Waveformdisplay->yAxis, SIGNAL(rangeChanged(QCPRange)), loss_Waveformdisplay->yAxis2, SLOT(setRange(QCPRange)));

	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	loss_Waveformdisplay->graph(0)->rescaleAxes();
	// same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
	loss_Waveformdisplay->graph(1)->rescaleAxes(true);
	// Note: we could have also just called customPlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	loss_Waveformdisplay->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

//A slot function to update the loss curve
void Widget::changeWave()
{

	xData.push_back(testX);
	testX++;

	if (xData.size() > 2)
	{
		loss_Waveformdisplay->graph(0)->setData(xData, testLossData);
		loss_Waveformdisplay->graph(0)->rescaleAxes();
		loss_Waveformdisplay->graph(1)->setData(xData, trainLossData);
		loss_Waveformdisplay->replot();
	}
}

//Dynamically add hidden layer
void Widget::createLayer()
{
	bool ok{ false };
	int hiddenlayer_dim = QInputDialog::getInt(this, tr("Add a hidden layer"),
		tr("Please enter the dimension of the hidden layer:"), 10, 1, 2147483647, 1, &ok);
	if (ok)
	{
		QDynamicButton* button = new QDynamicButton(this); // Create a dynamic button object
		connect(button, &QDynamicButton::operate_signal, this, &Widget::visualizeWeights);

		button->setText(tr("L") + QString::number(QDynamicButton::getLayerCount()));
		button->setMaximumWidth(40);
		button->setMinimumHeight(hiddenlayer_dim * 10);
		button->setStyleSheet("QPushButton{background-color: yellow; color:black}");
		my_hidden_layer_dims.push_back(hiddenlayer_dim);

		if (QDynamicButton::getLayerCount() > 1)
		{
			QLabel* arrows = new QLabel(tr("TEST"));
			arrows->setMaximumSize(QSize(100, 100));
			arrows->setPixmap(QPixmap(QString::fromUtf8(":/common/hyphen.jpg")));
			internallayout->addWidget(arrows);
		}

		internallayout->addWidget(button);
		hiddenlayer->setText(QString::number(QDynamicButton::getLayerCount()) + tr(" Hidden Layer"));
	}
}

//Dynamically remove hidden layer
void Widget::removeLayer()
{
	if (QDynamicButton::getLayerCount() == 0)
	{
		return;	// To avoid dereferencing nullptr
	}
	if (QDynamicButton::getLayerCount() > 1)
	{
		QPushButton* button = qobject_cast<QPushButton*>(internallayout->itemAt(internallayout->count() - 1)->widget());
		button->hide();
		delete button;

		QLabel* arrows = qobject_cast<QLabel*>(internallayout->itemAt(internallayout->count() - 1)->widget());
		arrows->hide();
		delete arrows;
	}
	else
	{
		QPushButton* button = qobject_cast<QPushButton*>(internallayout->itemAt(internallayout->count() - 1)->widget());
		button->hide();
		delete button;
	}
	if (QDynamicButton::getLayerCount() > 1)
		hiddenlayer->setText(QString::number(QDynamicButton::getLayerCount()) + tr(" Hidden Layers"));
	else
		hiddenlayer->setText(QString::number(QDynamicButton::getLayerCount()) + tr(" Hidden Layer"));
}

//A slot function to update the image.
void Widget::changeShowImage(QString OpenFile)
{
	QImage image;
	if (OpenFile != "")
	{
		if (image.load(OpenFile))
		{
			image = image.scaled(uploadImage->width(), uploadImage->height(),
				Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

			uploadImage->setPixmap(QPixmap::fromImage(image));
		}
	}
}

//Click the hiddenlayer button to call this function and visualize the weights.
void Widget::visualizeWeights(int ButtonID)
{
	if (!netp)
	{
		QMessageBox nonet;
		nonet.warning(this, tr("Warning!"), tr("The network has not been initialized! Use train mode to train a model and try it again."));
		return;
	}
	Mat image;
	netp->weights[ButtonID - 1].clone().convertTo(image, CV_8U);//ButtonID counts from 1, weights counts from 0
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image);				// Show weights visulized image inside it.
	waitKey(0);
}

void Widget::browse()
{
	if (browseButton->text() == tr("Browse"))
	{
		QString directory = QFileDialog::getExistingDirectory(this,
			tr("Find Files"), QDir::currentPath());

		if (!directory.isEmpty())
		{
			if (directoryComboBox->findText(directory) == -1)
				directoryComboBox->addItem(directory);
			directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
		}
	}

	if (browseButton->text() == tr("Upload"))
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));

		if (!fileName.isEmpty())
		{
			if (directoryComboBox->findText(fileName) == -1)
				directoryComboBox->addItem(fileName);
			directoryComboBox->setCurrentIndex(directoryComboBox->findText(fileName));
		}
	}
}

// Switch between train mode and test mode
void Widget::switchMode(int state)
{
	if (state == 2)
	{
		browseButton->setText(tr("Upload"));
		directoryComboBox->setCurrentText(tr("After training, click Upload button to upload a square digit image, hit Run button to predict."));
	}
	if (state == 0)
	{
		browseButton->setText(tr("Browse"));
		directoryComboBox->setCurrentText(tr("Click Browse button to locate the dataset."));
	}
}


void Widget::resetAll()
{
	testmode->toggle(Qt::CheckState::Unchecked);
	directoryComboBox->setCurrentText(tr("Click Browse button to locate the dataset."));
	epoch_value->setText(tr("Enter number of epoch (like 10)"));
	learning_rate_value->setText(tr("Enter learning rate (like 1e-3)"));
	regularation_strength_value->setText(tr("Enter regularation strength"));
	batchsize_value->setText(tr("Enter batchsize"));
	lr_decay_value->setText(tr("Learning Rate Decay Coefficient"));
	shuffleCheckbox->setCheckState(Qt::CheckState::Unchecked);
	weight_scale_value->setText(tr("Enter weight scale (like 1e-2)"));
	uploadImage->clear();
}

void Widget::about()
{
	QMessageBox msgBox;
	msgBox.about(this, tr("About"), tr("SHEN, Ye\nContact: yshenat@connect.ust.hk\nLU, Xinbo\nContact: xluap@connect.ust.hk"));
}
void Widget::exit()
{
	close();
}

/*
 *In train mode, click run button to train the network.
 *In test mode, click run button to classify an image.
 */
void Widget::run()
{

	if (testmode->isChecked())
	{
		if (!netp)
		{
			QMessageBox nonet;
			nonet.warning(this, tr("Warning!"), tr("Cannot detect any trained model! Use train mode to train a model and try it again."));
			return;
		}
		if (!(directoryComboBox->currentText().endsWith(tr(".jpg")) || directoryComboBox->currentText().endsWith(tr(".png")) || directoryComboBox->currentText().endsWith(tr(".jpeg"))))
		{
			QMessageBox nonet;
			nonet.warning(this, tr("Warning!"), tr("Cannot detect any image! Click the Upload button to upload an image."));
			return;
		}
		Mat image = imread(directoryComboBox->currentText().toStdString(), IMREAD_GRAYSCALE).reshape(1, 1);
		image.convertTo(image, CV_64F);
		int category = netp->predict(image);
		QMessageBox result;
		QString resultMessage(tr("This image belongs to the number ") + QString::number(category) + tr(" class!"));
		result.about(this, tr("Result"), resultMessage);
		return;
	}
	//clear previous model, dataloader, solver
	if (trainDataLoaderp)
	{
		delete trainDataLoaderp;
	}
	if (testDataLoaderp)
	{
		delete testDataLoaderp;
	}
	if (netp)
	{
		delete netp;
	}
	if (solverp)
	{
		delete solverp;
	}

	std::string directory = directoryComboBox->currentText().toStdString();
	filesystem::path datasetPath(directory);
	if (!std::filesystem::exists(datasetPath))//decide if it's a valid path.
	{
		QMessageBox noDataset;
		noDataset.warning(this, tr("Warning!"), tr("Cannot detect any dataset! Check whether the path is valid. Click the Browse button to choose a dataset."));
		return;
	}

	bool validInput1, validInput2, validInput3, validInput4, validInput5, validInput6;
	int backBatchSize = batchsize_value->text().toInt(&validInput1);
	double lr_rate = learning_rate_value->text().toDouble(&validInput2);
	double reg_strength = regularation_strength_value->text().toDouble(&validInput3);
	double weight_scale = weight_scale_value->text().toDouble(&validInput4);
	int num_of_epochs = epoch_value->text().toInt(&validInput5);
	double lr_rate_decay = lr_decay_value->text().toDouble(&validInput6);
	if (!(validInput1 && validInput2 && validInput3 && validInput4 && validInput5 && validInput6))
	{
		QMessageBox nonvalidInput;
		nonvalidInput.warning(this, tr("Warning!"), tr("The input hyperparameters are not valid!"));
		return;
	}
	bool shuffleData = shuffleCheckbox->checkState() == Qt::Checked ? true : false;

	trainDataLoaderp = new DataLoader(false, shuffleData, backBatchSize, datasetPath);
	testDataLoaderp = new DataLoader(true, shuffleData, backBatchSize, datasetPath);
	int input_dim = trainDataLoaderp->operator[](0).first.cols;
	int num_of_classes = trainDataLoaderp->getNumOfClasses();
	netp = new FullyConnectedNet(my_hidden_layer_dims, input_dim, num_of_classes, reg_strength, weight_scale);

	solverp = new Solver(lr_rate_decay, lr_rate, num_of_epochs, netp, trainDataLoaderp, testDataLoaderp);

	size_t iterations_per_epoch = trainDataLoaderp->getNumOfIteration(); //num_of_iteration = datasetLength / Batchsize
	size_t num_of_test_iterations{ testDataLoaderp->getNumOfIteration() }; 

	for (size_t e{ 0 }; e < num_of_epochs; ++e)
	{
		for (size_t t{ 0 }; t < iterations_per_epoch; ++t)
		{
			double trainLoss = solverp->_step(t);
			double testLoss;
			train_loss_value->setText(QString::number(trainLoss));
			trainLossData.push_back(trainLoss);
			if (t % 5 == 0)
			{ //test every 5 iterations
				testLoss = solverp->_testDuringTraining(t % num_of_test_iterations);
				test_loss_value->setText(QString::number(testLoss));
				testLossData.push_back(testLoss);
			}
			else {
				testLossData.push_back(testLoss);//push testLossData into the vector, make the test_loss_curve synchronized with the train_loss_curve
			}
			changeShowImage(QString::fromStdString(trainDataLoaderp->getProcessingImagePath(t).string()));
			changeWave();  /*  update the loss curve  */
		}
		double test_acc{ solverp->check_accuracy(netp, testDataLoaderp) };
		if (test_acc > solverp->best_test_acc)
		{
			solverp->best_test_acc = test_acc;
			solverp->copyWeights(netp->weights, solverp->best_weights);
		}
		lr_rate *= lr_rate_decay;
	}
	solverp->copyWeights(solverp->best_weights, netp->weights);

	QMessageBox complete;
	complete.about(this, tr("Congrats!"), tr("Training Completed!"));
}
