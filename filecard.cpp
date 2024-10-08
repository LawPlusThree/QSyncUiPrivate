#include "filecard.h"

#include<QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>
#include"ElaCheckBox.h"
#include"ElaIconButton.h"
#include"ElaProgressBar.h"
FileCard::FileCard(QString f, quint64 d,double s,int p,int syncStatus,int Id)
{
    // 创建ElaText对象来显示文件名，而不是直接在_checkBox中显示
    filename = new ElaText(f, this);

    if(syncStatus==1)//上传
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowUp,15);
        syncBtn->setEnabled(true);
    }
    else//下载
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowDown,15);
        syncBtn->setEnabled(true);
    }
    syncBtn->setFixedSize(15, 15);

    QString size;
    QString dataStr;
    double data=d;
    data/=1024;
    if(data<1024)
    {
        dataStr=QString::number(data,'f',1);
        size=dataStr+"KB";
    }
    else
    {
        data/=1024;
        if(data<1024)
        {
            dataStr=QString::number(data,'f',1);
            size=dataStr+"MB";
        }
        else
        {
            data/=1024;
            dataStr=QString::number(data,'f',1);
            size=dataStr+"GB";
        }
    }
    datasize = new ElaText(size, this);

    QString speedStr;
    QString Speed;
    if(s>1024)
    {
        s/=1024;
        QString speedStr=QString::number(s,'f',1);
        Speed=speedStr+"MB/s";
    }
    else
    {
        QString speedStr=QString::number(s,'f',1);
        Speed=speedStr+"KB/s";
    }
    speed = new ElaText(Speed, this);

    progress = p;
    id=Id;
    fullText = f;

    filename->setWordWrap(false);//禁止换行
    datasize->setWordWrap(false);
    speed->setWordWrap(false);
    //progress->setWordWrap(false);

    datasize->setAlignment(Qt::AlignCenter);
    speed->setAlignment(Qt::AlignCenter);
    //progress->setAlignment(Qt::AlignCenter);

    // 创建一个没有文字的_checkBox
    _checkBox = new ElaCheckBox("", this);
    _checkBox->setFixedSize(25, 25);
    QVBoxLayout* checkBoxArea = new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox, 0, Qt::AlignCenter); // 将_checkBox放在布局的最左侧

    // 设置文件名文字的布局
    filename->setTextSize(16);
    filename->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(filename, 0, Qt::AlignLeft); // 将文件名放在左侧
    filename->installEventFilter(this);
    filename->setCursor(Qt::PointingHandCursor); // 设置鼠标手势为手形，提示用户可以点击

    datasize->setTextSize(16);
    datasize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    datasize->setFixedSize(100,20);
    QVBoxLayout* dataSizeArea = new QVBoxLayout();
    dataSizeArea->addWidget(datasize, 0, Qt::AlignCenter);
    //dataSizeArea->SetFixedSize(50, 20);

    proBar = new ElaProgressBar(this);
    proBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    proBar->setFixedSize(140, 20);
    proBar->setMinimum(0);
    proBar->setMaximum(100);
    proBar->setValue(progress);
    speed->setTextSize(16);
    speed->setFixedWidth(80);

    QWidget*upBar=new QWidget();
    QHBoxLayout* upBarArea = new QHBoxLayout(upBar);
    upBarArea->addWidget(syncBtn,0,Qt::AlignRight);
    upBarArea->addWidget(speed,0,Qt::AlignLeft);
    QVBoxLayout* proBarArea = new QVBoxLayout();
    //upBar->setFixedWidth(110);
    proBarArea->addWidget(upBar,0,Qt::AlignLeft);
    proBarArea->addWidget(proBar, 0, Qt::AlignCenter);
    proBarArea->setAlignment(Qt::AlignCenter);

    pauseBtn = new ElaIconButton(ElaIconType::CirclePause, 20);
    relieveBtn = new ElaIconButton(ElaIconType::CircleXmark, 20);
    pauseBtn->setFixedSize(30, 30);
    relieveBtn->setFixedSize(30, 30);

    QHBoxLayout* actionArea = new QHBoxLayout();
    actionArea->addWidget(pauseBtn);
    actionArea->addWidget(relieveBtn);
    actionArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout* FileCardArea = new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea); // 将_checkBox布局添加到卡片布局中
    FileCardArea->addWidget(filenameWidget); // 将文件名布局添加到卡片布局中，紧跟_checkBox
    FileCardArea->addLayout(dataSizeArea);
    FileCardArea->addLayout(proBarArea);
    FileCardArea->addLayout(actionArea);
    FileCardArea->setStretchFactor(checkBoxArea, 25);
    FileCardArea->setStretchFactor(filenameWidget, 500);
    FileCardArea->setStretchFactor(dataSizeArea, 110);
    FileCardArea->setStretchFactor(proBarArea, 130);
    FileCardArea->setStretchFactor(actionArea, 60);

    connect(relieveBtn, &ElaIconButton::clicked, this, &FileCard::on_relieveBtn_clicked);
    connect(pauseBtn, &ElaIconButton::clicked, this, &FileCard::on_pauseBtn_clicked);
    filenameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
}

void FileCard::on_relieveBtn_clicked()
{
    emit Relieve(id);
}

void FileCard::on_pauseBtn_clicked()
{
    if(pauseBtn->getAwesome()==ElaIconType::CirclePause)
        pauseBtn->setAwesome(ElaIconType::CirclePlay);
    else
        pauseBtn->setAwesome(ElaIconType::CirclePause);
    emit Pause();
}

/*void FileCard::modify(int d,int s,int p)
{
    QString size=QString("%1GB").arg(d);
    datasize->setText(size);
    QString Speed=QString("%1mb/s").arg(s);
    speed->setText(Speed);
    progress = p;
    proBar->setValue(progress);
}*/

void FileCard::modify(quint64 totalSize,quint64 currentSize)
{

    quint64 d=totalSize;
    int p=((double)currentSize/(double)totalSize)*100;
    double s=0;
    preSize=Size;
    Size=currentSize;
    qDebug()<<preSize<<"   "<<Size;
    if(!currentTime.isValid())
    {
        s=0;
        currentTime=QDateTime::currentDateTime();
    }
    else
    {
        preTime=currentTime;
        QDateTime _currentTime=QDateTime::currentDateTime();
        int time=preTime.msecsTo(_currentTime);
        if(time<=500||currentSize<preSize){
            return ;
        }
        else{
            currentTime=_currentTime;
        }
        qDebug()<<time;
        if(time==0)
        {
            s=s;
        }
        else{
            s=((double)(Size-preSize))/time;
        }
    }

    QString size;
    QString dataStr;
    double data=d;
    data/=1024;
    if(data<1024)
    {
        dataStr=QString::number(data,'f',1);
        size=dataStr+"KB";
    }
    else
    {
        data/=1024;
        if(data<1024)
        {
            dataStr=QString::number(data,'f',1);
            size=dataStr+"MB";
        }
        else
        {
            data/=1024;
            dataStr=QString::number(data,'f',1);
            size=dataStr+"GB";
        }
    }

    QString speedStr;
    QString Speed;
    if(s>1024)
    {
        s/=1024;
        QString speedStr=QString::number(s,'f',1);
        Speed=speedStr+"MB/s";
    }
    else
    {
        QString speedStr=QString::number(s,'f',1);
        Speed=speedStr+"KB/s";
    }

    datasize->setText(size);
    speed->setText(Speed);
    progress = p;
    proBar->setValue(progress);
}

void FileCard::processing(int p)
{
    progress=p;
    if(pauseBtn->getAwesome()==ElaIconType::CirclePause)
        proBar->setValue(progress);
}

bool FileCard::ischecked()
{
    return _checkBox->isChecked();
}

ElaCheckBox* FileCard::getCheckBox() const
{
    return _checkBox;
}

void FileCard::selectCheckBox(bool checked) {
    _checkBox->setChecked(checked);
}

bool FileCard::eventFilter(QObject *watched, QEvent *event) {
    if (watched == filename && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QString filePath = this->fullText;
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
            return true; // 事件处理完毕，不再传递
        }
    }
    return QObject::eventFilter(watched, event); // 对于其他事件，保持默认处理
}
