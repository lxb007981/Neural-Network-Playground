#ifndef BUTTONS_H
#define BUTTONS_H
#include <QPushButton>

/*A class inherits QPushButton to perform dynamic adding and removing*/
class QDynamicButton : public QPushButton
{
    Q_OBJECT
public:
    QDynamicButton(QWidget *parent);
    int getID();        // Function to return a local number buttons
    static int& getLayerCount(){return layer_count;}
    ~QDynamicButton();
public slots:
    void on_clicked(); //emits operate_signal(int buttonID)
private:
	int buttonID{ 0 };   // Local variable number of the button
    static int layer_count;
	
signals:
	//signal carries buttonID, which is helpful in the weights visulization
    void operate_signal(int buttonID);
};


#endif // BUTTONS_H
