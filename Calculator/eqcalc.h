#ifndef EQCALC_H
#define EQCALC_H

#include "interface.h"

#include <QObject>

class EqCalc : public QObject
{
    Q_OBJECT

public:
    bool detectErrors(QString &str);
    double evalExpression(QString str);

private:
    void remSpaces(QString &str);
    void simplifyPosNegSign(QString &str);
    bool detectAdjacentMultDev(QString str);
    bool detectHangingSignOperator(QString str);
    bool detectEndOperator(QString str);

    bool isDigit(char c);
    bool isSignAfterMultDev(QChar c);

    QString getSubString(QString &str, int &index);
    double getNextOperand(QString &str, int &index);
    double getNextTerm(QString &str, int &index);

signals:
    void errorOccurred(QString error);
};

extern EqCalc ec;
#endif // EQCALC_H
