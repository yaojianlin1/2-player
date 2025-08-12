#pragma once
#include "ui_MP4player.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QPushButton>
#include <QVideoWidget>
#include <QLabel>
class MP4player : public QMainWindow {
    Q_OBJECT
    
public:
    MP4player(QWidget* parent = nullptr);
    ~MP4player();
private slots:
    void button_OpenFile();
    void button_play_clicked();


private:
    Ui_MP4player* ui;
    QMediaPlayer *player;
    QPushButton *playButton;
    QVideoWidget  *videoWidget;
    QLabel *timeLabel;
};