    #ifndef WIN_H
#define WIN_H

#include <QWidget>
#include <QtGui>
#include <QTextCodec>

class Win : public QWidget
{
    Q_OBJECT
private:
    QFrame *Frame;
   // QTextCodec* Codec;

    QLabel *InputDataLabel;
    QLabel *InputLongitudeLabel;
    QLabel *InputLatitudeLabel;
    QLabel *InputHourLabel;
    QLabel *InputTimezoneLabel;

    QDateEdit *DataEdit;
    QLineEdit *InputLongitudeEdit;
    QLineEdit *InputLatitudeEdit;
    QLineEdit *InputHourEdit;
    QLineEdit *InputTimezoneEdit;

    QLabel *OutputSunriseLabel;
    QLineEdit *OutputSunriseEdit;

    QLabel *OutputSunsetLabel;
    QLineEdit *OutputSunsetEdit;

    QPushButton *NextButton;
    QPushButton *ExitButton;

    QLabel *SunImageLabel;
    QLabel *MoonImageLabel;

public:
    Win(QWidget *parent = 0);

public slots:
    void begin();
    void calc();
};

#endif // WIN_H
