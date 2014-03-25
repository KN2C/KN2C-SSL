#include "ai.h"

AI::AI(WorldModel *worldmodel, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

}

void AI::Start()
{
    qDebug() << "AI::Start";
    timer.start(AI_TIMER);
}

void AI::Stop()
{
    qDebug() << "AI::Stop";

}

void AI::timer_timeout()
{
    static int a = 0;
    RobotCommand rc;
    rc.maxSpeed = 1.5;
    rc.fin_pos.dir = 0;
    rc.fin_pos.loc = wm->ourRobot[0].pos.loc;
    //    qDebug() << a;
    //        switch (a)
    //        {
    //        case 0:
    //            rc.fin_pos.loc = {0,-1000};
    //            break;
    //        case 1:
    //            rc.fin_pos.loc = {00,1000};
    //            break;
    //        case 2:
    //            rc.fin_pos.loc = {2000,2000};
    //            break;
    //        case 3:
    //            rc.fin_pos.loc = {2000,-2000};
    //            break;
    //        }

    //        if((wm->ourRobot[0].pos.loc - rc.fin_pos.loc).length2() < 1000) a = (a+1) % 2;
    //    rc.fin_pos.dir = M_PI;
    //    switch (a)
    //    {
    //    case 0:
    //        rc.fin_pos.loc = Vector2D(500,0);
    //        break;
    //    case 1:
    //        Vector2D kick_pos =  (wm->ball.pos.loc - Vector2D(3025,0));
    //        kick_pos.setLength(ROBOT_RADIUS+BALL_RADIUS);
    //        kick_pos = wm->ball.pos.loc + kick_pos;
    //        rc.fin_pos.loc = kick_pos;//Vector2D(500,0);//
    //        //rc.fin_pos.dir = (Vector2D(3025,0) - wm->ourRobot[0].pos.loc  ).dir().radian();
    //        if((wm->ourRobot[0].pos.loc-wm->ball.pos.loc).length()<ROBOT_RADIUS && \
    //                fabs(( (wm->ball.pos.loc - wm->ourRobot[0].pos.loc).dir().radian() - wm->ourRobot[0].pos.dir)*AngleDeg::RAD2DEG) < 7 )
    //        { //   qDebug()<<"KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
    //            rc.kickspeedx=1;
    //        }
    //        break;
    //    }
    //    //qDebug()<<"                        "<< ((wm->ball.pos.loc - wm->ourRobot[0].pos.loc).dir().radian() - wm->ourRobot[0].pos.dir)*AngleDeg::RAD2DEG;
    //    if((wm->ourRobot[0].pos.loc - rc.fin_pos.loc).length() < 100) a = 1;
    if(wm->ourRobot[0].isValid)
        wm->ourRobot[0].SendCommand(rc);
}
