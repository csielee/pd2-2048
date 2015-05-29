#include "game.h"
#include "ui_game.h"
#include <Qt>
#include <QLabel>
#include <QFont>
#include <QString>
#include <QKeyEvent>
#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <QPicture>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QDialog>
#include <algorithm>

game::game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
    srand(time(NULL));
    f.setPixelSize(20);
    f.setFamily("Arial");
    srand(time(NULL));

    Bestsocre=0;
    ui->Best_score->setFont(f);
    ui->New_score->setFont(f);

    menu_background.load(":/game/img/img/menu.png");

    mode=0;
    capower=1;


}

game::~game()
{
    delete ui;
}

void game::on_start_clicked()
{
    menu_background.load(":/game/img/img/game.png");

    for(int i=0;i<ui->gamelayout->rowCount();i++)
        for(int j=0;j<ui->gamelayout->columnCount();j++)
            ui->gamelayout->removeItem(ui->gamelayout->itemAtPosition(i,j));

    n=ui->choose_n->value();
    qDebug()<<n<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            plate[i][j]=new QLabel(this);
            plate[i][j]->setFont(f);
            ui->gamelayout->addWidget(plate[i][j],i,j);
        }
    }
    ui->m->setCurrentWidget(ui->game_2);
    plate_clear();
    plate_new();
    plate_show();
    plate_show();


    avengers.clear();
    capower=1;
    avengers.getthing(mode);
    if(avengers.whos==4)
        capower=3;

    //show things
    ui->people_img->setPixmap(avengers.head[avengers.whos]);
    ui->people_talk->setText(avengers.talk[avengers.whos]);
    QString str=ui->people_step->text();
    ui->people_step->setText(replaceNum(str,avengers.count));



}

void game::on_exit_clicked()
{
    close();
}

void game::on_back_clicked()
{
    ui->m->setCurrentWidget(ui->menu);
    menu_background.load(":/game/img/img/menu.png");
}

void game::on_restart_clicked()
{
    plate_clear();
    plate_new();
    plate_show();

    avengers.clear();
    capower=1;
    avengers.getthing(mode);
    if(avengers.whos==4)
        capower=3;

    //show things
    ui->people_img->setPixmap(avengers.head[avengers.whos]);
    ui->people_talk->setText(avengers.talk[avengers.whos]);
    QString str=ui->people_step->text();
    ui->people_step->setText(replaceNum(str,avengers.count));
}

/*鍵盤事件*/
void game::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        if(plate_move('w'))
        {
            plate_new();
            avengers.count--;
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if(plate_move('a'))
        {
            plate_new();
            avengers.count--;
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if(plate_move('s'))
        {
            plate_new();
            avengers.count--;
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if(plate_move('d'))
        {
            plate_new();
            avengers.count--;
        }
        break;
    default:
        break;
    }

    if(avengers.count==0)
    {
        avengers.power(avengers.whos,plate_number,n,score);
        if(avengers.whos==4)
            capower=1;

        avengers.getthing(mode);

        //show things
        ui->people_img->setPixmap(avengers.head[avengers.whos]);
        ui->people_talk->setText(avengers.talk[avengers.whos]);

        if(avengers.whos==4)
            capower=3;

    }

    QString str=ui->people_step->text();
    ui->people_step->setText(replaceNum(str,avengers.count));

    int y=plate_check();
    if(y)
    {
        if(y==1)
            plate_gameover();

        if(y==2)
        {
            plate_gamewin();
            on_restart_clicked();
        }
    }
    plate_show();


}

void game::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,menu_background);
}

int game::plate_clear()
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            plate_number[i][j]=0;
        }
    }

    score=0;

    return 0;
}

int game::plate_show()
{
    QPixmap p;
    QString str(":/game/img/img/0.png");
    h=plate[0][0]->height();
    w=plate[0][0]->width();
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            str=replaceNum(str,plate_number[i][j]);
            p.load(str);

            p = p.scaled(h,h,Qt::KeepAspectRatio);
            plate[i][j]->setPixmap(p);
        }
    }

    ui->New_score->setNum(score);
    if(score>Bestsocre)
        Bestsocre=score;
    //最高
    ui->Best_score->setNum(Bestsocre);


    return 0;
}

int game::plate_move(char d)
{
    int count=0;
    switch (d) {
    case 'w':
        for(int i=1;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]!=0)
                {
                    int x=i;
                    while(plate_number[x-1][j]==0)
                    {
                        x--;
                        if(x-1<0)
                            break;
                    }
                    //move
                    if(x!=i)
                    {
                        plate_number[x][j]=plate_number[i][j];
                        plate_number[i][j]=0;
                        count++;
                    }
                }
            }
        }
        for(int i=0;i<n-1;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]!=0)
                {
                    if(plate_number[i+1][j]==plate_number[i][j])
                    {
                        plate_number[i][j]+=plate_number[i+1][j];
                        score+=(plate_number[i][j]*capower);
                        for(int k=i+1;k<n-1;k++)
                            plate_number[k][j]=plate_number[k+1][j];

                        plate_number[n-1][j]=0;
                        count++;
                    }
                }
            }
        }
        break;
    case 'a':
        for(int j=1;j<n;j++)
        {
            for(int i=0;i<n;i++)
            {
                if(plate_number[i][j]!=0)
                {
                    int x=j;
                    while(plate_number[i][x-1]==0)
                    {
                        x--;
                        if(x-1<0)
                            break;
                    }
                    //move
                    if(x!=j)
                    {
                        plate_number[i][x]=plate_number[i][j];
                        plate_number[i][j]=0;
                        count++;
                    }
                }
            }
        }
        for(int j=0;j<n-1;j++)
        {
            for(int i=0;i<n;i++)
            {
                if(plate_number[i][j]!=0)
                {
                    if(plate_number[i][j]==plate_number[i][j+1])
                    {
                        plate_number[i][j]+=plate_number[i][j+1];
                        score+=(plate_number[i][j]*capower);
                        for(int k=j+1;k<n-1;k++)
                            plate_number[i][k]=plate_number[i][k+1];

                        plate_number[i][n-1]=0;
                        count++;
                    }
                }
            }
        }
        break;
    case 's':
        for(int i=n-2;i>-1;i--)
        {
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]!=0)
                {
                    int x=i;
                    while(plate_number[x+1][j]==0)
                    {
                        x++;
                        if(x+1>n-1)
                            break;
                    }
                    //move
                    if(x!=i)
                    {
                        plate_number[x][j]=plate_number[i][j];
                        plate_number[i][j]=0;
                        count++;
                    }
                }
            }
        }
        for(int i=n-1;i>0;i--)
        {
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]!=0)
                {
                    if(plate_number[i-1][j]==plate_number[i][j])
                    {
                        plate_number[i][j]+=plate_number[i-1][j];
                        score+=(plate_number[i][j]*capower);
                        for(int k=i-1;k>0;k--)
                            plate_number[k][j]=plate_number[k-1][j];

                        plate_number[0][j]=0;
                        count++;
                    }
                }
            }
        }
        break;
    case 'd':
        for(int j=n-2;j>-1;j--)
        {
            for(int i=0;i<n;i++)
            {
                if(plate_number[i][j]!=0)
                {
                    int x=j;
                    while(plate_number[i][x+1]==0)
                    {
                        x++;
                        if(x+1>n-1)
                            break;
                    }
                    //move
                    if(x!=j)
                    {
                        plate_number[i][x]=plate_number[i][j];
                        plate_number[i][j]=0;
                        count++;
                    }
                }
            }
        }
        for(int j=n-1;j>0;j--)
        {
            for(int i=0;i<n;i++)
            {
                if(plate_number[i][j]!=0)
                {
                    if(plate_number[i][j-1]==plate_number[i][j])
                    {
                        plate_number[i][j]+=plate_number[i][j-1];
                        score+=(plate_number[i][j]*capower);
                        for(int k=j-1;k>0;k--)
                            plate_number[i][k]=plate_number[i][k-1];

                        plate_number[i][0]=0;
                        count++;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return count;
}

int game::plate_new()
{
    srand(time(NULL));
    int count=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(plate_number[i][j]==0)
                count++;
        }

    if(!count)
        return count;

    int position=rand()%count+1;
    count=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(plate_number[i][j]==0)
            {
                count++;
                if(count==position)
                {
                    int r=rand()%10;

                    if(r==0)
                        plate_number[i][j]=4;
                    else
                        plate_number[i][j]=2;

                    break;
                }
            }
        }

    return count;
}

int game::plate_check()
{
    int count=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(plate_number[i][j]==0)
                count++;

            if(plate_number[i][j]==2048)//win
                return 2;
        }

    if(count==0)
    {
        int y=1;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(i-1>=0)
                    if(plate_number[i][j]==plate_number[i-1][j])
                    {
                        y=0;
                        break;
                    }
                if(i+1<=n-1)
                    if(plate_number[i][j]==plate_number[i+1][j])
                    {
                        y=0;
                        break;
                    }
                if(j-1>=0)
                    if(plate_number[i][j]==plate_number[i][j-1])
                    {
                        y=0;
                        break;
                    }
                if(j+1<=n-1)
                    if(plate_number[i][j]==plate_number[i][j+1])
                    {
                        y=0;
                        break;
                    }
            }
            if(!y)
                break;
        }

        return y;
    }

    return 0;
}

int game::plate_gameover()
{
    QDialog over;
    over.setWindowTitle("遊戲結束");
    //over.resize(100,200);
    QGridLayout *g = new QGridLayout;
    QLabel *a = new QLabel,*b = new QLabel("你已經死了!乖乖重來吧!(請關閉視窗繼續)");
    QPixmap p;
    QString str(":/game/img/img/gameover.png");
    qDebug()<<p.load(str);
    QFont f;
    f.setPointSize(40);
    b->setFont(f);
    a->setPixmap(p);
    g->addWidget(a,0,0,1,1);
    g->addWidget(b,1,0,1,1);
    over.setLayout(g);
    QIcon i(":/game/img/img/444.png");
    over.setWindowIcon(i);

    return over.exec();

}

int game::plate_gamewin()
{
    QDialog win;
    win.setWindowTitle("遊戲獲勝");
    QGridLayout *g = new QGridLayout;
    QLabel *a=new QLabel;
    QPixmap p(":/game/img/img/gamewin.jpg");
    p=p.scaled(p.height()/1.5,p.width()/1.5,Qt::KeepAspectRatio);
    a->setPixmap(p);
    g->addWidget(a,0,0,1,1);
    win.setLayout(g);
    QIcon i(":/game/img/img/icon.png");
    win.setWindowIcon(i);

    return win.exec();

}

QString game::replaceNum(QString str,int& i)
{
    for(int j=0;j<str.length();j++)
    {
        if(str.at(j)<='9' && str.at(j)>='0')
        {
            int x=j;
            int n=0;
            while(str.at(j)<='9' && str.at(j)>='0')
            {
                n++;
                j++;
            }
            QString num;
            num.setNum(i);
            str.replace(x,n,num);
            break;
        }
    }

    return str;
}

void game::on_mode_easy_clicked()
{
    mode=0;
}

void game::on_mode_normal_clicked()
{
    mode=1;
}

void game::on_mode_hard_clicked()
{
    mode=2;
}


things::things()
{
    int a[8][2]={15,15,
                 10,15,
                 10,15,
                 20,25,
                 5,5,
                 5,10,
                 10,20,
                 10,15};
    for(int i=0;i<8;i++)
        for(int j=0;j<2;j++)
            step[i][j]=a[i][j];

    int b[8][3]={0,0,1,
                 1,0,1,
                 1,1,0,
                 0,0,1,
                 1,1,0,
                 1,1,0,
                 1,1,1,
                 0,1,1};
    for(int i=0;i<8;i++)
        for(int j=0;j<3;j++)
            mode[i][j]=b[i][j];

    talk[0]="浩克：\n吼！破壞\n效果：打亂所有方塊！";
    talk[1]="奧創：\n這才是我的全部\n效果：用２佈滿場面";
    talk[2]="鋼鐵人：\n賈維斯看你了\n效果：全部都升級";
    talk[3]="索爾：\n為阿斯嘉而戰\n效果：我要打掉最大的";
    talk[4]="隊長：\n復仇者，團結！\n效果：得到分數３倍";
    talk[5]="鷹眼：\n咻！咻！咻！\n效果：打掉３個最小數方塊";
    talk[6]="黑寡婦：\n我來收爛攤子\n效果：方塊從大到小排好";
    talk[7]="幻視：\n我站在生命這一邊\n效果：掃掉任一排、列";

    head[0].load(":/game/img/img/hulk.png");
    head[1].load(":/game/img/img/ultron.png");
    head[2].load(":/game/img/img/ironman.png");
    head[3].load(":/game/img/img/thor.png");
    head[4].load(":/game/img/img/ca.png");
    head[5].load(":/game/img/img/eagle.png");
    head[6].load(":/game/img/img/black.png");
    head[7].load(":/game/img/img/vision.png");


    for(int i=0;i<8;i++)
       head[i]=head[i].scaled(120,120,Qt::KeepAspectRatio);

    for(int i=0;i<5;i++)
        record[i]=-1;

}

void things::power(int s, int plate_number[][10], int n,int& score)
{
    int max=0,x,y;
    int count=0,countn=2;
    int num[n*n],re[n*n];
    int a=rand()%n,b=rand()%2;
    switch(s)
    {
    case 0://浩克
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                num[i*n+j]=plate_number[i][j];

        //rand
        countn=0;
        count=rand()%(n*n);
        while(1)
        {
            y=1;
            for(int j=0;j<countn;j++)
            {
                if(re[j]==count)
                {
                    count=rand()%(n*n);
                    y=0;
                }
            }

            if(y)
            {
            re[countn]=count;
            countn++;
            qDebug()<<count<<"X"<<countn<<endl;
            if(countn==n*n)
                break;
            }
        }


        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                plate_number[i][j]=num[re[i*n+j]];
            }
        break;
    case 1://奧創
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]==0)
                    plate_number[i][j]=2;
            }
        break;
    case 2://鋼鐵人
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]!=0)
                {
                    plate_number[i][j]+=plate_number[i][j];
                    score+=plate_number[i][j];
                }
            }
        break;
    case 3://索爾

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]>max)
                {
                    x=i;
                    y=j;
                    max=plate_number[i][j];
                }
            }

        plate_number[x][y]=0;
        break;
    case 4://隊長
        break;
    case 5://鷹眼

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                if(plate_number[i][j]==countn)
                {
                    plate_number[i][j]=0;
                    count++;
                }
                if(count==3)
                    break;
            }

        break;
    case 6://黑寡婦

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                num[i*n+j]=plate_number[i][j];

        //sort
        std::sort(&num[0],&num[n*n]);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
            {
                plate_number[i][j]=num[n*n-(i*n+j)-1];
            }

        break;
    case 7://幻視

        for(int i=0;i<n;i++)
        {
            if(b)
                plate_number[a][i]=0;
            else
                plate_number[i][a]=0;
        }
        break;
    default:
        break;
    }

}

void things::getthing(int m)
{
    //getthings
    whos=rand()%8;
    int y;
    while(1)
    {
        y=1;
        qDebug()<<whos<<"!?"<<endl;
        for(int i=0;i<r;i++)
        {
            if(record[i]==whos)
            {
                whos=rand()%8;
                y=0;
            }
        }
        if(y)
        {
        if(mode[whos][m]==1)
        {
            record[r]=whos;
            r++;
            qDebug()<<whos<<","<<r<<endl;
            if(r==5)
                clear();
            break;
        }
        else
            whos=rand()%8;
        }

    }


    int range=step[whos][1]-step[whos][0];
    if(range!=0)
        count=rand()%range+step[whos][0];
    else
        count=step[whos][0];
}

void things::clear()
{
    for(int i=0;i<5;i++)
        record[i]=-1;

    r=0;
}
