#include "synccore.h"
#include "SyncThread.h"
#include <iostream>
using namespace wtr;
SyncCore::SyncCore(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"sync core";
}

SyncCore::SyncCore(COSConfig config, QObject *parent, QString excludedItems)
    : QObject{parent}, config{config}
{
    requestManager=new NetworkRequestManager(config);
    this->excludedItems=excludedItems;
    connect(requestManager,&NetworkRequestManager::requestFinished,this,[=](int fileTaskId, QNetworkReply::NetworkError error){
        if(error==QNetworkReply::NoError)
        {
            emit finishFileDownloadTask(fileTaskId);
            emit finishFileUploadTask(fileTaskId);
            qDebug()<<"request finished";
        }
        else
        {
            qDebug()<<"request error";
        }
    },Qt::QueuedConnection);
    connect(requestManager,&NetworkRequestManager::requestProgress,this,[=](int fileTaskId, qint64 bytesReceived, qint64 bytesTotal){
        //emit updateFileUploadTask(fileTaskId,bytesReceived,bytesTotal);
        qDebug()<<"request progress"<<bytesReceived<<" "<<bytesTotal;
        emit updateFileDownloadTask(fileTaskId,bytesReceived,bytesTotal);
        emit updateFileUploadTask(fileTaskId,bytesReceived,bytesTotal);
    },Qt::QueuedConnection);
}

bool SyncCore::addTask(SyncTask *task)
{
    QDir listen=task->localPath;
    //Filefunc *filefunc=new Filefunc(this);
    //connect(filefunc, &Filefunc::fileListUpdated, this, &SyncCore::onFileListUpdated, Qt::QueuedConnection);
    //filefunc->readDirectory(listen.absolutePath());
    if(task==nullptr)
        return false;
    if(task->localPath.exists()==false)
    {
        qDebug()<<"local path not exists";
        return false;
    }
    if(task->syncStatus==-1)
    {
        qDebug()<<"paused";
        return false;
    }
    tasks.push_back(task);
    doTask(task);
    return true;
}

void SyncCore::doTask(SyncTask *task)
{

    SyncThread *thread=new SyncThread(task->localPath.absolutePath(),config,task);
    connect(thread,&SyncThread::localTotalSize,this,[=](qint64 size){
        emit taskTotalSize(size,task->getId());
    });
    connect(thread,&SyncThread::upTotalSize,this,[=](qint64 size){
        emit taskUploadSize(size,task->getId());
    });
    connect(thread,&SyncThread::newUploadTask,this,[=](const QString &localPath, qint64 fileTaskId){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (localPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            emit addFileUploadTask(localPath,fileTaskId);
    },Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::newDownloadTask,this,[=](const QString &localPath, qint64 fileTaskId,quint64 totalSize){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (localPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            emit addFileDownloadTask(localPath,fileTaskId,totalSize);
    },Qt::BlockingQueuedConnection);

    connect(thread,&SyncThread::finishUploadTask,this,[=](int fileTaskId){
        emit finishFileUploadTask(fileTaskId);
    },Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::finishDownloadTask,this,[=](int fileTaskId){
        emit finishFileDownloadTask(fileTaskId);
    },Qt::BlockingQueuedConnection);

    connect(thread, &SyncThread::callUploadTask, this, [=](const QString &localPath, const QString &cloudPath, int fileTaskId){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (localPath.contains(line) || cloudPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            requestManager->addPutObjectRequest(localPath,cloudPath,fileTaskId,QMap<QString,QString>()); 
    }, Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::callDownloadTask,this,[=](const QString &localPath, const QString &cloudPath, int fileTaskId){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (localPath.contains(line) || cloudPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            requestManager->addSave2LocalRequest(cloudPath,localPath,fileTaskId);
    },Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::callRenameFileTask,this,[=](const QString &cloudPath, const QString &copyToPath, int fileTaskId){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (copyToPath.contains(line) || cloudPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            requestManager->addPutObjectCopyRequest(cloudPath,copyToPath,fileTaskId);
    },Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::callDeleteFileTask,this,[=](const QString &cloudPath, const QString &versionId, int fileTaskId){
        bool shouldSkip = false;
        for (const QString &line : excludedItems)
        {
            if (cloudPath.contains(line))
            {
                shouldSkip = true;
                break; // 如果发现任一路径包含排除行，则跳出循环
            }
        }
        if (!shouldSkip)
            requestManager->addDeleteObjectRequest(cloudPath,versionId,fileTaskId);
    },Qt::BlockingQueuedConnection);


    thread->start();
}



void SyncCore::onDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
}

void SyncCore::onFileChanged(const QString &path)
{
    qDebug() << "File changed: " << path;
}


