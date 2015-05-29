#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QFont>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>

//復仇者事件
class things{
public:
    things();
    int step[8][2];
    int mode[8][3];
    int whos,count,r;
    void power(int s, int plate_number[][10], int n, int &score);
    QString talk[8];
    QPixmap head[8];
    int record[5];
    void getthing(int m);
    void clear();
};


namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = 0);
    ~game();

private slots:
    void on_start_clicked();

    void on_exit_clicked();

    void on_back_clicked();

    void on_restart_clicked();

    void on_mode_easy_clicked();

    void on_mode_normal_clicked();

    void on_mode_hard_clicked();

protected:
    void keyPressEvent(QKeyEvent *);
    void paintEvent(QPaintEvent* event);

private:
    Ui::game *ui;

    QLabel* plate[10][10];
    int plate_number[10][10];
    int n;
    int mode;
    int score,Bestsocre;
    int h,w;
    int plate_clear();//清空
    int plate_show();//顯示
    int plate_move(char d);//移動
    int plate_new();//隨機產生
    int plate_check();//檢查
    int plate_gameover();//遊戲結束
    int plate_gamewin();//遊戲獲勝
    QString replaceNum(QString, int &);
    QFont f;

    things avengers;
    int capower;
    QPixmap menu_background;

};

#endif // GAME_H
