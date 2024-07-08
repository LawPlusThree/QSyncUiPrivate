#include "historysync_view.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include <QVBoxLayout>
#include"historycard.h"
#include"ElaScrollArea.h"

HistorysyncPage::HistorysyncPage(QWidget* parent)
    : ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("历史同步"); // 设置窗口标题

    // 创建一个 ElaProgressBar 对象，用作进度条
    _progressBar = new ElaProgressBar(this);
    // 设置QProgressBar的大小策略为水平方向可扩展，垂直方向固定
    _progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 设置最小尺寸，可以适当调整宽度以减少填充效果
    _progressBar->setMinimumSize(100, 20); // 调整最小宽度为100，最小高度20
    // 设置进度条的最小值和最大值
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0); // 表示不确定的进度
    QWidget* progressBarArea = new QWidget();
    progressBarArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    progressBarArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作进度条的容器
    //ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    // 为进度条容器设置水平布局
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    progressBarLayout->setContentsMargins(50, 0, 55, 0); // 设置布局的边距
    // 创建一个 ElaText 对象，用于显示进度条的文本标签
    ElaText* progressBarText = new ElaText("总进度：", this);
    // 设置进度条文本标签的字体大小为 15
    progressBarText->setTextSize(15);
    // 将进度条文本标签和进度条控件添加到布局中
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);

    QWidget* catalogueArea = new QWidget();
    catalogueArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    catalogueArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    //ElaScrollPageArea* catalogueArea = new ElaScrollPageArea(this);
    QHBoxLayout* catalogueLayout = new QHBoxLayout(catalogueArea);
    catalogueLayout->setContentsMargins(10, 0, 15, 0);
    ElaText* catalogueText1 = new ElaText("文件名", this);
    catalogueText1->setTextSize(15);
    catalogueText1->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(15);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText4 = new ElaText("同步状态", this);
    catalogueText4->setTextSize(15);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText5 = new ElaText("已完成", this);
    catalogueText5->setTextSize(15);
    catalogueText5->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    catalogueLayout->addWidget(catalogueText1);
    catalogueLayout->addWidget(catalogueText2);
    catalogueLayout->addWidget(catalogueText4);
    catalogueLayout->addWidget(catalogueText5);

    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明
    HistoryCard*HistoryCardArea1=new HistoryCard("文件1","3.5GB","2024.7.1",0);
    HistoryCard*HistoryCardArea2=new HistoryCard("文件2","3.5GB","2024.7.2",0);
    HistoryCard*HistoryCardArea3=new HistoryCard("文件3","3.5GB","2024.7.3",1);
    HistoryCard*HistoryCardArea4=new HistoryCard("文件4","3.5GB","2024.7.4",0);
    HistoryCard*HistoryCardArea5=new HistoryCard("文件5","3.5GB","2024.7.5",1);
    HistoryCard*HistoryCardArea6=new HistoryCard("文件6","3.5GB","2024.7.6",1);
    HistoryCard*HistoryCardArea7=new HistoryCard("文件7","3.5GB","2024.7.7",1);
    HistoryCard*HistoryCardArea8=new HistoryCard("文件8","3.5GB","2024.7.8",0);
    HistoryCard*HistoryCardArea9=new HistoryCard("文件9","3.5GB","2024.7.9",0);
    HistoryCard*HistoryCardArea10=new HistoryCard("文件10","3.5GB","2024.7.10",1);

    QWidget* filesWidget=new QWidget();
    filesLayout=new QVBoxLayout(filesWidget);
    filesLayout->addWidget(HistoryCardArea1);
    filesLayout->addWidget(HistoryCardArea2);
    filesLayout->addWidget(HistoryCardArea3);
    filesLayout->addWidget(HistoryCardArea4);
    filesLayout->addWidget(HistoryCardArea5);
    filesLayout->addWidget(HistoryCardArea6);
    filesLayout->addWidget(HistoryCardArea7);
    filesLayout->addWidget(HistoryCardArea8);
    filesLayout->addWidget(HistoryCardArea9);
    filesLayout->addWidget(HistoryCardArea10);
    filesLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(filesWidget); // 设置scrollArea的内容部件
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea);

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

HistorysyncPage::~HistorysyncPage()
{
}
