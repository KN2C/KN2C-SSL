#include "tactictest.h"

TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTest", worldmodel, parent)
{
    a = 0;
    b = 0;
}

RobotCommand TacticTest::getCommand()
{

    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    param = QString::number(a);

    if(wm->ball.pos.loc.x > 2000)
    {
        rc.fin_pos.loc=Vector2D(0,1300);
    }
    else
    {
    rc.maxSpeed = 1;
    //wm->ourRobot[id].vel.loc.length()
    rc.fin_pos.loc=wm->kn->PredictDestination(wm->ourRobot[id].pos.loc , wm->ball.pos.loc ,\
                                              rc.maxSpeed , wm->ball.vel.loc);
    }
    qDebug()<< wm->ball.vel.loc.length();//rc.fin_pos.loc.x << "  " << rc.fin_pos.loc.y;

//    switch (a)
//    {
//    case 0:
//        rc.fin_pos.loc = {500,1600};//{0,1000};//
//        break;
//    case 1:
//        rc.fin_pos.loc = {500,-1600};//{2000,1000};//
//        break;
//    case 2:
//        rc.fin_pos.loc = {700,-700};
//        break;
//    case 3:
//        rc.fin_pos.loc = {-700,-700};
//        break;
//    case 4:
//        rc.fin_pos.loc = {-700,700};
//        break;
//    }

//    if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 180))
//    {
//        a = (a+1) % 2;
//        //b++;
//       // if(b>6) a=0;
//    }

    rc.useNav = false;
    rc.isBallObs = true;
    rc.isKickObs = true;
    return rc;
}
