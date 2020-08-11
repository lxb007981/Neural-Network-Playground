#include <buttons.h>


QDynamicButton::QDynamicButton(QWidget *parent) :
    QPushButton(parent)
{
    ++layer_count;            // Increment of counter by one
    buttonID = layer_count;   // Assigning a button number which will be further work with buttons
    connect(this, &QDynamicButton::clicked, this, &QDynamicButton::on_clicked);
}

QDynamicButton::~QDynamicButton() { --layer_count; } //destroy one layer, decrement of counter by one

// Method to return the value of the button numbers
int QDynamicButton::getID()
{
    return buttonID;
}

void QDynamicButton::on_clicked(){
    emit operate_signal(buttonID);
}

// Initialize static class variable.
int QDynamicButton::layer_count = 0;


