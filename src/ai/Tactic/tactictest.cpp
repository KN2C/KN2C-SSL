#include "tactictest.h"

TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
    a = 0;
}

RobotCommand TacticTest::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    rc.maxSpeed = 2;
    rc.fin_pos.dir = 0;
    Vector2D kick_pos =  (wm->ball.pos.loc - Vector2D(-3025,0));
    kick_pos.setLength(ROBOT_RADIUS);
    kick_pos = wm->ball.pos.loc + kick_pos;

    Vector2D tmp = wm->ball.pos.loc - kick_pos;
    Vector2D tmp2 = wm->ball.pos.loc - wm->ourRobot[id].pos.loc ;
    rc.fin_pos.loc = wm->ball.pos.loc - (tmp + tmp2)/2;
    rc.useNav = false;
    rc.fin_pos.dir = (Vector2D(-3025,0) - wm->ourRobot[id].pos.loc  ).dir().radian();
    //    switch (a)
    //    {
    //    case 0:
    //        rc.fin_pos.loc = {0,1000};
    //        //rc.fin_pos.dir = M_PI;
    //        break;
    //    case 1:
    //        rc.fin_pos.loc = {1000,1000};
    //       // rc.fin_pos.dir = 0;
    //        //rc.fin_pos.loc = kick_pos;
    //        //rc.fin_pos.dir = (Vector2D(3025,0) - wm->ourRobot[id].pos.loc  ).dir().radian();
    //        break;
    //    case 2:
    //        rc.fin_pos.loc = {1000,1000};
    //        break;
    //    case 3:
    //        rc.fin_pos.loc = {1000,-1000};
    //        break;
    //    }

    double werr1 = (wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian()
                          - wm->ourRobot[id].pos.dir;
    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;

    if((wm->ourRobot[id].pos.loc-wm->ball.pos.loc).length()
            <ROBOT_RADIUS+BALL_RADIUS && fabs(werr1*AngleDeg::RAD2DEG)<7)
    {
        qDebug()<<"test:KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
        rc.kickspeedx=2;
    }
    else
    {
        //qDebug()<<"---";
    }
    //qDebug()<<werr1*AngleDeg::RAD2DEG;

    //cout <<a<<" "<< (wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG<<endl;
    //if(fabs((wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG)<20) a=1;
    //if((wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length() < 10) a = (a+1) % 2;

    rc.isBallObs = true;
    rc.isKickObs = true;
    return rc;
}
