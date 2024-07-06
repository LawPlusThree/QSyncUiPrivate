#include "signinwin.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include<QPixmap>
#include<QBrush>
#include<QPalette>
#include<QColor>
#include<QMessageBox>

#include"ElaPushButton.h"
#include"ElaText.h"
#include"ElaImageCard.h"

signinwin::signinwin(QWidget* parent):ElaWidget(parent)
{
    setWindowTitle("珞珈云");
    QWidget*area=new QWidget();
    this->setWindowModality(Qt::ApplicationModal);
    this->setCentralWidget(area);
    this->hide();
    this->setFixedSize(250,600);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);

    ElaText*text=new ElaText("注册账号");
    text->setTextSize(16);
    QHBoxLayout*textArea=new QHBoxLayout();
    textArea->addWidget(text,0,Qt::AlignCenter);

    QImage image;
    ElaImageCard*avatar=new ElaImageCard();
    avatar->setCardImage(image);
    avatar->setFixedSize(150,150);
    QHBoxLayout*avatarArea=new QHBoxLayout();
    avatarArea->addWidget(avatar,0,Qt::AlignCenter);

    IDLine=new ElaLineEdit();
    IDLine->setFixedSize(270,30);
    IDLine->setPlaceholderText("用户ID");
    QHBoxLayout*IDArea=new QHBoxLayout();
    IDArea->addWidget(IDLine,0,Qt::AlignCenter);

    accoutLine=new ElaLineEdit();
    accoutLine->setFixedSize(270,30);
    accoutLine->setPlaceholderText("账号");
    QHBoxLayout*accoutArea=new QHBoxLayout();
    accoutArea->addWidget(accoutLine,0,Qt::AlignCenter);

    passwordLine=new ElaLineEdit();
    passwordLine->setFixedSize(270,30);
    passwordLine->setPlaceholderText("密码");
    passwordLine->setEchoMode(QLineEdit::Password);
    QHBoxLayout*passwordArea=new QHBoxLayout();
    passwordArea->addWidget(passwordLine,0,Qt::AlignCenter);

    passagainLine=new ElaLineEdit();
    passagainLine->setFixedSize(270,30);
    passagainLine->setPlaceholderText("再次确认密码");
    passagainLine->setEchoMode(QLineEdit::Password);
    QHBoxLayout*passagainArea=new QHBoxLayout();
    passagainArea->addWidget(passagainLine,0,Qt::AlignCenter);

    ElaPushButton*signinBtn=new ElaPushButton("注册");
    signinBtn->setFixedSize(270,30);
    signinBtn->setStyleSheet("background-color:rgb(0,204,255)");
    QHBoxLayout*signinBtnArea=new QHBoxLayout();
    signinBtnArea->addWidget(signinBtn,0,Qt::AlignCenter);

    QVBoxLayout*signinWinArea=new QVBoxLayout(area);
    signinWinArea->addLayout(textArea,Qt::AlignCenter);
    signinWinArea->addLayout(avatarArea,Qt::AlignCenter);
    signinWinArea->addLayout(IDArea,Qt::AlignCenter);
    signinWinArea->addLayout(accoutArea,Qt::AlignCenter);
    signinWinArea->addLayout(passwordArea,Qt::AlignCenter);
    signinWinArea->addLayout(passagainArea,Qt::AlignCenter);
    signinWinArea->addLayout(signinBtnArea,Qt::AlignCenter);
    area->setLayout(signinWinArea);

    connect(signinBtn,&ElaPushButton::clicked,this, &signinwin::on_signinBtn_clicked);
}

void signinwin::on_signinBtn_clicked()
{
    if(IDLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入用户ID！");
    }
    else if(accoutLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入账号！");
    }
    else if(passwordLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入密码！");
    }
    else if(passagainLine->text()=="")
    {
        QMessageBox::information(this, "错误","请再次确认密码！");
    }
    else if(passwordLine->text()!=passagainLine->text())
    {
        QMessageBox::information(this, "错误","两次输入的密码不一致！");
    }
    else
    {
            //测试用户
            //User enrolluser("newuser@example.com","123456");
            User enrolluser(IDLine->text(),accoutLine->text(),passwordLine->text());
            if(enrolluser.enroll())
                QMessageBox::information(this, "成功","注册成功");
            else
                QMessageBox::critical(this, "失败","注册失败");
            emit goback();
    }
}

signinwin::~signinwin()
{
}

void signinwin::closeEvent(QCloseEvent *event)
{
    emit goback();
}
