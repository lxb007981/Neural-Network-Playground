/*	
 *  The widget serves as the mainwindow.
 *	It requires user input and set up the neural network.
 *	The training status is shown here.
 */

#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <buttons.h>
#include <switch.h>
#include <QComboBox>
#include "qcustomplot.h"
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <vector>
#include "Solver.h"



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
	QSize sizeHint() const override;
private slots:
    void browse();
    void createLayer();
    void removeLayer();
    void switchMode(int);
    void resetAll();
    void changeShowImage(QString);
    void changeWave();
    void about();
    void exit();
    void run();
	void visualizeWeights(int);

private:
    QComboBox *directoryComboBox;
    QPushButton* runButton;
    QPushButton* reset;
    QLabel *epoch;
	QLineEdit*epoch_value;
    QLabel *learning_rate;
    QLineEdit *learning_rate_value;
    QLabel *regularation_strength;
    QLineEdit *regularation_strength_value;
    QLabel *batchsize;
    QLineEdit *batchsize_value;
	QLabel* lr_decay;
	QLineEdit* lr_decay_value;
	QCheckBox* shuffleCheckbox; 
	QLabel* weight_scale;
	QLineEdit* weight_scale_value;
    QPushButton *addButton;
    QPushButton *minusButton;
    QLabel *hiddenlayer;
    QVBoxLayout *mainlayout;
    QHBoxLayout *toplayout;
    QGridLayout *rightupperlayout;
    QHBoxLayout *middlelayout;
    QHBoxLayout *bottomlayout;
    QVBoxLayout *bottomleftlayout;
    Switch * testmode;
    QPushButton* browseButton;
    QVBoxLayout* bottommiddlelayout;
    QHBoxLayout* internallayout;
    QVBoxLayout* bottomrightlayout;
    QGridLayout* losslayout;
    QLabel* output;
    QLabel* test_loss;
    QLabel* train_loss;

    QLabel* uploadImage = nullptr;
    QWidget* showWave = nullptr;

    QCustomPlot *loss_Waveformdisplay = nullptr;

    QLabel* test_loss_value;
    QLabel* train_loss_value;
    QHBoxLayout* bottommiddlebottomlayout;

    /*  store loss data to paint  */
    QVector<double> xData;			//x-date
    QVector<double> testLossData;	//y-data
    QVector<double> trainLossData;	//y-data
  

    double testX = 1;	//x-axis

	std::vector<int> my_hidden_layer_dims;


    QMenuBar* menu_bar;
    QMenu* file_menu;
    QMenu* edit_menu;
    QMenu* help_menu;
    QAction* exit_action;
    QAction* aboutAction;

    void create1stLayer();
    void create2ndLayer();
    void create3rdLayer();
    void createBar();    
	void showWaveInit();

	/*To be setup in runtime*/
	DataLoader* trainDataLoaderp{nullptr};
	DataLoader* testDataLoaderp{ nullptr };
	FullyConnectedNet* netp{ nullptr };
	Solver* solverp{ nullptr };
};	
#endif // WIDGET_H
