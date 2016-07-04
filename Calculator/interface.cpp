#include "interface.h"
#include "eqcalc.h"

#include <QtWidgets>

Interface::Interface(QWidget *parent)
    : QWidget(parent)
{
    QFont font;
    font.setPointSize(15);

    equation = new QLineEdit();
    equation->setFixedHeight(40);
    equation->setMaxLength(50);
    equation->setFont(font);

    result = new QLineEdit();
    result->setFixedHeight(44);
    result->setFrame(false);
    result->setReadOnly(true);
    result->setFont(font);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(equation);
    layout->addWidget(result);

    setLayout(layout);
    setWindowTitle("Calculator");
    QWidget::setFixedSize(400, 100);

    connect(this->equation, SIGNAL(returnPressed()), this, SLOT(on_equation_returnPressed()));
    connect(&ec, SIGNAL(errorOccurred(QString)), this, SLOT(outputError(QString)));
}

void Interface::on_equation_returnPressed()
{
    QString expr = equation->text();
    outputResult(expr);
}

void Interface::outputResult(QString expr)
{
    furtherErrors = false;

    if(!ec.detectErrors(expr))
    {
        double num = ec.evalExpression(expr);
        if(!furtherErrors)
            result->setText(QString::number(num));
    }
    return;
}

void Interface::outputError(QString error)
{
    furtherErrors = true;
    result->setText(error);
}
