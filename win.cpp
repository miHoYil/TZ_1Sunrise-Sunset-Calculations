#include "win.h"
#include "main.cpp"
#include <QDate>
#include <iostream>
#include <QDebug>

class MyDoubleValidator : public QDoubleValidator
{
public:
    MyDoubleValidator(double bottom, double top, int decimals, QObject * parent) :
        QDoubleValidator(bottom, top, decimals, parent)
    {
    }

    QValidator::State validate(QString &s, int &i) const
        {
            if (s.isEmpty() || s == "-") {
                return QValidator::Intermediate;
            }

            QChar decimalPoint = locale().decimalPoint();

            if(s.indexOf(decimalPoint) != -1) {
                int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

                if (charsAfterPoint > decimals()) {
                    return QValidator::Invalid;
                }
            }

            bool ok;
            double d = locale().toDouble(s, &ok);

            if (ok && d >= bottom() && d <= top()) {
                return QValidator::Acceptable;
            } else {
                return QValidator::Invalid;
            }
        }
};

Win::Win(QWidget *parent) : QWidget(parent)
{
   // Codec = QTextCodec::codecForUtfText("Windows-1258");
    setWindowTitle(("Sunrise/Sunset Calculations"));

    QString SunImageFileName = "assets/sun.png";
    QImage SunImage;
    SunImage.load(SunImageFileName);
    SunImage = SunImage.scaled(75,75);
    SunImageLabel = new QLabel(this);
    SunImageLabel->setPixmap(QPixmap::fromImage(SunImage));

    QString MoonImageFileName = "assets/moon.png";
    QImage MoonImage;
    MoonImage.load(MoonImageFileName);
    MoonImage = MoonImage.scaled(75,75);
    MoonImageLabel = new QLabel(this);
    MoonImageLabel->setPixmap(QPixmap::fromImage(MoonImage));

    Frame = new QFrame(this);
    Frame->setFrameShadow(QFrame::Raised);
    Frame->setFrameShape(QFrame::Panel);

    InputDataLabel = new QLabel("Enter the Date",this);
    DataEdit = new QDateEdit(this);


    InputLatitudeLabel = new QLabel("Latitude");
    InputLatitudeEdit = new QLineEdit("",this);
    MyDoubleValidator* LatitudeValidator;
    LatitudeValidator = new MyDoubleValidator(-90.0,90.0,4,InputLatitudeEdit);
    InputLatitudeEdit->setValidator(LatitudeValidator);

    InputLongitudeLabel = new QLabel("Longitude");
    InputLongitudeEdit = new QLineEdit("",this);
    MyDoubleValidator* LongitudeValidator;
    LongitudeValidator = new MyDoubleValidator(-180.0,180.0,4,InputLongitudeEdit);
    LongitudeValidator->setNotation(MyDoubleValidator::StandardNotation);
    InputLongitudeEdit->setValidator(LongitudeValidator);

    InputHourLabel = new QLabel("Hour");
    InputHourEdit = new QLineEdit("",this);
    InputHourEdit->setValidator(new QIntValidator(0,23,InputHourEdit));

    InputTimezoneLabel = new QLabel("Timezone (UTC)");
    InputTimezoneEdit = new QLineEdit("",this);
    InputTimezoneEdit->setValidator(new QIntValidator(-12,14,InputTimezoneEdit));

    OutputSunriseLabel = new QLabel("Sunrise",this);
    OutputSunriseEdit = new QLineEdit("",this);
    OutputSunriseEdit->setReadOnly(true);

    OutputSunsetLabel = new QLabel("Sunset",this);
    OutputSunsetEdit = new QLineEdit("",this);
    OutputSunsetEdit->setReadOnly(true);

    NextButton = new QPushButton("Next Date",this);
    ExitButton = new QPushButton("Exit",this);

    QGridLayout *Gridlayout = new QGridLayout;
    Gridlayout->setContentsMargins(5,5,5,5);
    Gridlayout->setSpacing(5);

    Gridlayout->addWidget(InputHourLabel,0,0,1,1);
    Gridlayout->addWidget(InputHourEdit,0,1,1,2);

    Gridlayout->addWidget(InputLatitudeLabel,1,0,1,1);
    Gridlayout->addWidget(InputLatitudeEdit,1,1,1,2);

    Gridlayout->addWidget(InputLongitudeLabel,2,0,1,1);
    Gridlayout->addWidget(InputLongitudeEdit,2,1,1,2);


    Gridlayout->addWidget(InputTimezoneLabel,3,0,1,1);
    Gridlayout->addWidget(InputTimezoneEdit,3,1,1,2);

    Gridlayout->addWidget(OutputSunriseLabel,4,0,1,2);
    Gridlayout->addWidget(SunImageLabel,5,0,1,2);
    Gridlayout->addWidget(OutputSunriseEdit,5,2,1,1);

    Gridlayout->addWidget(OutputSunsetLabel,6,0,1,2);
    Gridlayout->addWidget(MoonImageLabel,7,0,1,2);
    Gridlayout->addWidget(OutputSunsetEdit,7,2,1,1);


    QVBoxLayout *VLayout1 = new QVBoxLayout(Frame);
    VLayout1->addWidget(InputDataLabel);
    VLayout1->addWidget(DataEdit);
    VLayout1->addLayout(Gridlayout);
    VLayout1->addStretch();

    QVBoxLayout *VLayout2 = new QVBoxLayout();
    VLayout2->addWidget(NextButton);
    VLayout2->addWidget(ExitButton);
    VLayout2->addStretch();

    QHBoxLayout *HLayout1 = new QHBoxLayout(this);
    HLayout1->addWidget(Frame);
    HLayout1->addLayout(VLayout2);



    this->resize(600,300);
    begin();

    connect(ExitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(NextButton,SIGNAL(clicked(bool)),this,SLOT(begin()));
   //connect(DataEdit,SIGNAL(editingFinished()),this,SLOT(calc()));
    connect(InputLatitudeEdit,SIGNAL(returnPressed()),this,SLOT(calc()));
    connect(InputLongitudeEdit,SIGNAL(returnPressed()),this,SLOT(calc()));
    connect(InputHourEdit,SIGNAL(returnPressed()),this,SLOT(calc()));
    connect(InputTimezoneEdit,SIGNAL(returnPressed()),this,SLOT(calc()));
}

void Win::begin()
{

    NextButton->setEnabled(false);
    NextButton->setDefault(false);

    DataEdit->setEnabled(true);
    InputLatitudeEdit->setEnabled(true);
    InputLongitudeEdit->setEnabled(true);
    InputHourEdit->setEnabled(true);
    InputTimezoneEdit->setEnabled(true);

    DataEdit->clear();
    InputLatitudeEdit->clear();
    InputLongitudeEdit->clear();
    InputHourEdit->clear();
    InputTimezoneEdit->clear();

    SunImageLabel->setVisible(false);
    MoonImageLabel->setVisible(false);
    OutputSunsetLabel->setVisible(false);
    OutputSunriseLabel->setVisible(false);
    OutputSunsetEdit->setVisible(false);
    OutputSunriseEdit->setVisible(false);
    OutputSunriseLabel->setEnabled(true);
    OutputSunsetLabel->setEnabled(true);
    DataEdit->setFocus();
}

void Win::calc()
{
    bool Ok=true,Ok1=true,Ok2=true,Ok3=true;
    QString str;

    str = InputLatitudeEdit->text();
    float latitude = str.toDouble(&Ok);

    str = InputLongitudeEdit->text();
    float longitude = str.toDouble(&Ok1);

    str = InputHourEdit->text();
    int hour = str.toInt(&Ok2);

    str = InputTimezoneEdit->text();
    int timezone = str.toInt(&Ok3);

        // float latitude = 59.8940,longitude = 30.2640;
    if(Ok && Ok1 && Ok2 && Ok3)
    {
    QDate date = DataEdit->date();

    DoubleFullTime SunRiseSetTime = CalcNOAASunRiseSet(longitude,latitude,date,hour,timezone);

    QString SunriseLine;
    if(SunRiseSetTime.srHour < 10 && SunRiseSetTime.srHour > 0 ) SunriseLine = "0"+QString::number(SunRiseSetTime.srHour);
        else SunriseLine = QString::number(SunRiseSetTime.srHour);
    if(SunRiseSetTime.srMin < 10 && SunRiseSetTime.srMin > 0) SunriseLine+=":0"+QString::number(SunRiseSetTime.srMin);
        else SunriseLine+= ":"+QString::number(SunRiseSetTime.srMin);

    SunriseLine += "   (+"+QString::number(timezone)+"h UTC)";

    QString SunsetLine;
    if(SunRiseSetTime.ssHour < 10 && SunRiseSetTime.ssHour > 0 ) SunsetLine = "0"+QString::number(SunRiseSetTime.ssHour);
        else SunsetLine = QString::number(SunRiseSetTime.ssHour);
    if(SunRiseSetTime.ssMin < 10 && SunRiseSetTime.ssMin > 0) SunsetLine+=":0"+QString::number(SunRiseSetTime.ssMin);
        else SunsetLine+= ":"+QString::number(SunRiseSetTime.ssMin);

    SunsetLine += "   (+"+QString::number(timezone)+"h UTC)";

        OutputSunriseEdit->setText(SunriseLine);
        OutputSunsetEdit->setText(SunsetLine);

        InputLatitudeEdit->setEnabled(false);
        InputLongitudeEdit->setEnabled(false);
        InputHourEdit->setEnabled(false);
        DataEdit->setEnabled(false);
        InputTimezoneEdit->setEnabled(false);

        OutputSunsetLabel->setVisible(true);
        OutputSunriseLabel->setVisible(true);
        OutputSunriseEdit->setVisible(true);
        OutputSunsetEdit->setVisible(true);
        SunImageLabel->setVisible(true);
        MoonImageLabel->setVisible(true);

        NextButton->setDefault(true);
        NextButton->setEnabled(true);
        NextButton->setFocus();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Information,"Sunrise/Sunset Calculations","One of the Lines is Empty",QMessageBox::Ok);
        msgBox.exec();
    }
}
