#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include"loginwin.h"
#include"ElaCheckBox.h"
#include"userinfopage.h"
#include"databasemanager.h"
#include "user.h"
#include"signinwin.h"
#include <QQueue>
class HomePage;
class SyncingPage;
class FileManagePage;
class HistorysyncPage;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class HistoryViewPage;
class modifyInfor_win;
class cancelaccount_win;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    signinwin* signin = new signinwin();
    loginwin* login=new loginwin();
    Q_SLOT void onCloseButtonClicked();
    User*CurrentUser=nullptr;
    DatabaseManager*db;

public slots:
    void onUserLoggedIn(User user);
    void onNeedPassword(const QString& account);
    void insertUserToDatabase(User user);
    void onLoginResponse(const int &code,const QJsonObject &data,const QString &message);
    void onMessage( QString message,QString type);
signals:
    void dbPassword(const QString &);


private:
    HomePage *_homePage{nullptr};
    SyncingPage *_syncingPage{nullptr};
    FileManagePage*_filemanagePage{nullptr};
    HistorysyncPage *_historysyncPage{nullptr};
    HistoryViewPage *_historyviewPage{nullptr};
    T_ElaScreen *_elaScreenPage{nullptr};
    T_Icon *_iconPage{nullptr};
    T_BaseComponents *_baseComponentsPage{nullptr};
    T_TabWidget *_tabWidgetPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
    ElaCheckBox* _checkBox{nullptr};
    UserInfoPage* _userinfopage{nullptr};
    modifyInfor_win* _modifyInfor_win{nullptr};
    cancelaccount_win* _cancelaccount_win{nullptr};

};
#endif // MAINWINDOW_H
