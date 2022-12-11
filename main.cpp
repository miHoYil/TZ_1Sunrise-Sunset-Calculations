#include <QCoreApplication>
#include <QtDebug>
#include <iostream>
#include <QTableView>
#include <QtGui>
#include "win.h"
#include "Utils.h"

DoubleFullTime CalcNOAASunRiseSet(float longitude, float latitude,QDate date, int hour, int timezone)
{
    DoubleFullTime Ans;

    int dayOfYear = date.day() + ((date.month() < 3) ?
    (int)((306 * date.month() - 301) / 10) :
    (int)((306 * date.month() - 913) / 10) + ((date.year() % 4 == 0 && (date.year() % 100 != 0 || date.year() % 400 == 0)) ? 60 : 59));

    float fractionYear =  (date.year() % 4 == 0 && (date.year() % 100 != 0 || date.year() % 400 == 0)) ? // Radians
                                2.0f*M_PI/366.0f*((double)dayOfYear-1+((double)hour-12.0f)/24.0f):
                                2.0f*M_PI/365.0f*((double)dayOfYear-1+((double)hour-12.0f)/24.0f);

    float eqTime = 229.18*(0.000075 + 0.001868*cos(fractionYear) - // Minutes
                            0.032077*sin(fractionYear) - //
                            0.014615*cos(2.0f*fractionYear) - //
                            0.040849*sin(2.0f*fractionYear));

    float decl = 0.006918 - 0.399912*cos(fractionYear) + // Radians
            0.070257*sin(fractionYear) - //
            0.006758*cos(2.0f*fractionYear) + //
            0.000907*sin(2.0f*fractionYear) - //
            0.002697*cos(3.0f*fractionYear) + //
            0.00148*sin(3.0f*fractionYear);

    float haSunrise = acos(cos(DegreesToRadians(90.833))/(cos(DegreesToRadians(latitude))*cos(decl)) - // Radians
                            tan(DegreesToRadians(latitude))*tan(decl));

    float timeZone = float(timezone)*60.0f;


    float srInMin = 720.0f - 4.0f * (longitude + RadiansToDegrees(haSunrise)) - eqTime + timeZone;

    float ssInMin = 720.0f - 4.0f * (longitude + RadiansToDegrees(-haSunrise)) - eqTime + timeZone;

    Ans.srHour = int(srInMin / 60)%24;
    Ans.srMin = (int) srInMin % 60;

    Ans.ssHour = int(ssInMin / 60)%24;
    Ans.ssMin = (int) ssInMin % 60;

    return Ans;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"Russian");
    QApplication app(argc, argv);


        Win MainWindow(0);
        MainWindow.show();

     return app.exec();
}
