#ifndef INTERFACE_H
#define INTERFACE_H


#include <QtWidgets>

class Interface : public QWidget
{
    Q_OBJECT
public:
    explicit Interface(QWidget *parent = 0);

signals:

public slots:
    void on_equation_returnPressed();
    void outputError(QString error);

private:
    void outputResult(QString expr);

    bool furtherErrors;
    QLineEdit *equation;
    QLineEdit *result;

};

#endif // INTERFACE_H
