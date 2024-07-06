#ifndef LOGINWIN_H
#define LOGINWIN_H

#include"ElaWidget.h"
#include"resetwin.h"
#include"signinwin.h"
#include "mycore.h"

class loginwin:public ElaWidget
{
    Q_OBJECT
public:
    loginwin(QWidget *parent = nullptr);
    ~loginwin();
    resetwin*resetWin=new resetwin();
    signinwin*signinWin=new signinwin();

    ElaLineEdit*accountLine;
    ElaLineEdit*passwordLine;

private slots:
    void on_resetBtn_clicked();
    void on_signinBtn_clicked();
    void on_loginBtn_clicked();
private:
    Core *core;

};

#endif // LOGINWIN_H
