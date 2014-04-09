#include "renderarea.h"

RenderArea::RenderArea(Soccer* sc) :
    QWidget()
{
    _sc = sc;
    brush_field = new QBrush(QPixmap(":/resources/images/Field.jpg"));
    brush_ball = new QBrush(QColor::fromRgb(255,200,0),Qt::SolidPattern);
    brush_yrobot = new QBrush(QColor::fromRgb(255,255,0),Qt::SolidPattern);
    brush_brobot = new QBrush(QColor::fromRgb(50,50,255),Qt::SolidPattern);
    _timer.start(40);
    connect(&_timer,SIGNAL(timeout()), this, SLOT(refresh()));
}

void RenderArea::refresh()
{
    update();
}

void RenderArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Settings
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw Field
    painter.setBrush(*brush_field);
    painter.drawRect(0,0,740,540);

    // FPS
    painter.drawText(20,20,QString::number(_fps.FPS()));

    painter.translate(CENTER_X,CENTER_Y);

    // Draw Robots
    painter.setPen(QColor::fromRgb(0,0,0));

    painter.setBrush(*brush_yrobot);
    for(int i=0;i<12;i++)
        if(_sc->wm->oppRobot[i].isValid)
        {
            painter.drawChord(_sc->wm->oppRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->oppRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(_sc->wm->oppRobot[i].pos.dir));
            painter.drawText(_sc->wm->oppRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->oppRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,QString::number(i));
        }

    painter.setBrush(*brush_brobot);
    for(int i=0;i<12;i++)
        if(_sc->wm->ourRobot[i].isValid)
        {
            painter.drawChord(_sc->wm->ourRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->ourRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(_sc->wm->ourRobot[i].pos.dir));
            painter.drawText(_sc->wm->ourRobot[i].pos.loc.x/WORLD_SCALE-ROBOT_R,-_sc->wm->ourRobot[i].pos.loc.y/WORLD_SCALE-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,QString::number(i));
        }

    // Draw Ball
    if(_sc->wm->ball.isValid)
    {
        painter.setPen(QColor::fromRgb(200,100,0));
        painter.setBrush(*brush_ball);
        painter.drawEllipse(QPoint(_sc->wm->ball.pos.loc.x/WORLD_SCALE,-_sc->wm->ball.pos.loc.y/WORLD_SCALE),BALL_R,BALL_R);
    }
    // TEST

    //painter.drawChord(100-ROBOT_R,100-ROBOT_R,ROBOT_D,ROBOT_D,ANGLE_TO_CHORD(0));
    //painter.drawText(100-ROBOT_R,100-ROBOT_R,ROBOT_D,ROBOT_D,Qt::AlignCenter,"8");

    _fps.Pulse();
}
