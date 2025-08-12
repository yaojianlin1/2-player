#include "MP4player.h"
#include <QWidget>
#include <QMainWindow>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "QFileDialog"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QAudioOutput>

MP4player::MP4player(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_MP4player)
{
    ui->setupUi(this);
    //创建中央视频显示控件
    this->player = new QMediaPlayer(this);

    this->videoWidget = new QVideoWidget(this);

    QAudioOutput *audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    player->setVideoOutput(videoWidget);

    videoWidget->setStyleSheet("background: #000;");//黑色背景
    videoWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setCentralWidget(videoWidget);

    //创建底部控制栏容器
    QWidget *controlBar = new QWidget(this);
    QHBoxLayout *controlLayout = new QHBoxLayout(controlBar);

    //控件初始化
    QPushButton *openButton = new QPushButton("📁 选择文件",controlBar);
    this->playButton = new QPushButton("▶ 播放",controlBar);
    QSlider *positionSlider = new QSlider(Qt::Horizontal,controlBar);
    positionSlider->setStyleSheet("QSlider::handle:horizontal{background:white;}");
    positionSlider->setRange(0,1000);
    positionSlider->setValue(0);

    QSlider *volumeSlider = new QSlider(Qt::Horizontal,controlBar);
    volumeSlider->setRange(0,100);
    volumeSlider->setValue(50);
    QLabel *volumeIcon = new QLabel("🔊",controlBar);


    this->timeLabel = new QLabel("00:00 / 00:00",this);
    timeLabel->setStyleSheet("color: gray;font-family:monospace");
    timeLabel->setFixedWidth(120);


    //设置控件尺寸
    openButton->setFixedWidth(100);
    playButton->setFixedWidth(90);
    volumeIcon->setFixedWidth(30);

    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addSpacing(10);
    controlLayout->addWidget(timeLabel);
    controlLayout->addWidget(positionSlider,1);//进度条占剩余空间
    controlLayout->addSpacing(10);
    controlLayout->addWidget(volumeIcon);
    controlLayout->addWidget(volumeSlider,0,Qt::AlignVCenter);

    //将控制栏放入状态栏区域
    statusBar()->addWidget(controlBar);
    statusBar()->setMinimumHeight(60);
    statusBar()->setStyleSheet("QStatusBar { background: #f0f0f0; border-top: 1px solid #ccc; }");

    resize(800,600);
    setWindowTitle("MP4 视频播放器");
    setStyleSheet("QPushButton { padding:5px;}");



    connect(openButton,&QPushButton::clicked,this,&MP4player::button_OpenFile);
    connect(playButton,&QPushButton::clicked,this,&MP4player::button_play_clicked);

    
connect(player, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error, const QString &errorString) {
    QMessageBox::warning(this, "播放错误", "无法播放该文件：" + errorString);
});
    connect(player,&QMediaPlayer::durationChanged,this,[positionSlider,this](qint64 duration){
        positionSlider->setRange(0,static_cast<int>(duration));
        int totalSeconds = duration/1000;
        int minutes = totalSeconds/60;
        int seconds = totalSeconds % 60;
        QString totaltime = QString("%1:%2")
                .arg(minutes,2,10,QChar('0'))
                .arg(seconds,2,10,QChar('0'));
        QString current = this->timeLabel->text().split(" / ").first();
        timeLabel->setText(current + " / " + totaltime);
    });
    
    connect(player, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
    int currentSeconds = position / 1000;
    int minutes = currentSeconds / 60;
    int seconds = currentSeconds % 60;
    QString currentTime = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));

    // 获取总时间部分
    QString total = timeLabel->text().split(" / ").last();
    timeLabel->setText(currentTime + " / " + total);
});
    

    connect(player,&QMediaPlayer::positionChanged,this,[positionSlider](qint64 position){
        positionSlider->setValue(static_cast<int>(position));
    });
    connect(positionSlider,&QSlider::sliderReleased,this,[positionSlider,this](){
        player->setPosition(positionSlider->value());
    });



    connect(volumeSlider,&QSlider::valueChanged,this,[audioOutput](int value){
        audioOutput->setVolume(value/100.0);
    });

}

void MP4player::button_OpenFile(){
    QString fileName = QFileDialog::getOpenFileName(
        this,   //父窗口
        "选择一个视频文件",  //对话框标题
        "", //起始目录
        "视频文件(*.mp4 *.avi *.mkv *.mov);;所有文件(*)" // 文件过滤器
    );

    if(!fileName.isEmpty()){
        this->player->setSource(QUrl::fromLocalFile(fileName));
        this->playButton->setText("⏸");
        player->play();
    }
}

void MP4player::button_play_clicked(){
    switch (player->playbackState())
    {
    case QMediaPlayer::PlayingState:
        /* code */
        player->pause();
        playButton->setText("▶");
        break;
    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
        player->play();
        this->playButton->setText("⏸");
        break;
    default:
        break;
    }
}



MP4player::~MP4player()
{
    delete ui; 
}