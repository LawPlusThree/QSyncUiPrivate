#ifndef HISTORYSYNC_VIEW_H
#define HISTORYSYNC_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"
#include"ElaPushButton.h"

class QVBoxLayout;
class HistoryCardProxy;
class HistorysyncPage : public ElaScrollPage
{
public:
    HistorysyncPage(QWidget* parent = nullptr);
    ~HistorysyncPage();
    QVBoxLayout*filesLayout;
    void addHistory(QString filename, QString datasize,QString time,bool upif);
    QWidget* filenameWidget = new QWidget();
    HistoryCardProxy*_historycardPage;
private:
    ElaProgressBar* _progressBar{nullptr};
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif // HISTORYSYNC_VIEW_H
