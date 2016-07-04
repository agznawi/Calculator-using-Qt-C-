#include "eqcalc.h"

#include <QObject>


void EqCalc::remSpaces(QString &str)
{
    int i = 0, j = 0;
    while((str[i] = str[j++]) != '\0')
        if(str[i] != ' ')
            i++;
    str.resize(i);
    return;
}


void EqCalc::simplifyPosNegSign(QString &str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        int s = 1; bool posNegSign = false;
        for(; str[i] == '+' || str[i] == '-';i++)
        {
            posNegSign = true;
            if(str[i] == '-')
                s *= -1;
            str[i] = ' ';
        }

        if(posNegSign)
            if(s == -1)
                str[i-1] = '-';
            else
                str[i-1] = '+';
    }
    return;
}


bool EqCalc::detectAdjacentMultDev(QString str)
{
    for(int i = 0; str[i] != '\0'; i++)
        if((str[i] == '*' || str[i] == '/') &&
           (str[i+1] == '*' || str[i+1] == '/'))
            return true;
    return false;
}


bool EqCalc::detectHangingSignOperator(QString str)
{
    for(int i = 0; str[i] != '\0'; i++)
        if((str[i] == '+' || str[i] == '-') &&
           (str[i+1] == '*' || str[i+1] == '/'))
            return true;
    return false;
}


bool EqCalc::detectEndOperator(QString str)
{
    char lastC = str[str.length()-1].toLatin1();
    if(lastC == '+' || lastC == '-' ||
       lastC == '*' || lastC == '/')
        return true;
    return false;
}


bool EqCalc::isDigit(char c)
{
    return (c >= '0' && c <= '9');
}


bool EqCalc::isSignAfterMultDev(QChar c)
{
    return(c == '+' || c == '-');
}


QString EqCalc::getSubString(QString &str, int &index)
{
    QString subString;
    int leftParCount = 0;

    for(int idx = 0; str[index] != '\0'; idx++, index++)
    {
        subString += str[index];
        switch(str[index].toLatin1())
        {
        case ')':
            if(!leftParCount)
            {
                subString.resize(idx);
                index++;
                return subString;
            }
            else
                leftParCount--;
            break;
        case '(':
            leftParCount++;
            break;
        }
    }
    emit errorOccurred("Error: Left parentheses mismatch");
    return subString;
}


double EqCalc::getNextOperand(QString &str, int &index)
{
    double operand = 0.0;

    if(str[index] == '(')
    {
        index++;
        QString substr = getSubString(str, index);
        operand = evalExpression(substr);
        return operand;
    }

    while(isDigit(str[index].toLatin1()))
        operand = 10*operand + str[index++].toLatin1()-'0';

    if(str[index] != '.')
        return operand;

    for(double f = 10.0; isDigit(str[++index].toLatin1()); f*=10)
        operand += (str[index].toLatin1()-'0')/f;

    return operand;
}


double EqCalc::getNextTerm(QString &str, int &index)
{
    double term = 0.0;
    short s = 1;
    term = getNextOperand(str, index);

    for(;;)
    {
        switch(str[index].toLatin1())
        {
        case '*':
            index++;
            if(isSignAfterMultDev(str[index]))
                (str[index++] == '-')? s = -1 : s = 1;
            term *= s*getNextOperand(str, index);
            break;
        case '(':
            term *= getNextOperand(str, index);
            break;
        case '/':
            index++;
            if(isSignAfterMultDev(str[index]))
                (str[index++] == '-')? s = -1 : s = 1;
            term /= s*getNextOperand(str, index);
            break;
        default:
            return term;
        }
    }
}


double EqCalc::evalExpression(QString str)
{
    int index = 0;
    double expression = 0.0;

    if(str[index] == '*' || str[index] == '/')
    {
        emit errorOccurred("Error: (Sub)expression starts with * or /");
        return expression;
    }

    expression = getNextTerm(str, index);

    for(;;)
    {
        switch(str[index++].toLatin1())
        {
        case '+':
            expression += getNextTerm(str, index);
            break;
        case '-':
            expression -= getNextTerm(str, index);
            break;
        case '\0':
            return expression;
        case ')':
            emit errorOccurred("Error: Right parentheses mismatch");
            return expression;
        default:
            emit errorOccurred("Error: Invalid character");
            return expression;
        }
    }
}

bool EqCalc::detectErrors(QString &equation)
{
    remSpaces(equation);
    simplifyPosNegSign(equation);
    remSpaces(equation);

    if(equation == "")
    {
        emit errorOccurred("No equation");
        return 1;
    }
    if(detectAdjacentMultDev(equation))
    {
        emit errorOccurred("Error: Multiple adjacent * and /");
        return 1;
    }
    if(detectHangingSignOperator(equation))
    {
        emit errorOccurred("Error: * or / after sign");
        return 1;
    }
    if(detectEndOperator(equation))
    {
        emit errorOccurred("Error: Excessive op at the end");
        return 1;
    }
    return 0;
}
