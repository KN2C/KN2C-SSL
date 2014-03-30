#include "tacticgoalie.h"

TacticGoalie::TacticGoalie(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticGoalie::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    rc.fin_pos.dir = ( wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian();

    double werr1 = (wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian()
                          - wm->ourRobot[id].pos.dir;
    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;

    if((wm->ourRobot[id].pos.loc-wm->ball.pos.loc).length()
            <ROBOT_RADIUS+BALL_RADIUS *2.5 && fabs(werr1*AngleDeg::RAD2DEG)<7)
    {
        qDebug()<<"goalie:KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
        rc.kickspeedx=5;
    }
    else
    {
        //qDebug()<<"---";
    }
    //qDebug()<<werr1*AngleDeg::RAD2DEG;

    if(wm->ball.isValid)
    {
        int attackerID = -1;
        double attackerDist = 1000000;
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->oppRobot[i].isValid)
            {
                double dist;
                if((dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc)) < attackerDist)
                {
                    attackerID = i;
                    attackerDist = dist;
                }
            }
        }

        if(attackerID == -1)
        {
            if(wm->ball.vel.loc.length()>0.1)
            {
                Line2D goalLine = Line2D(Vector2D(-2700,-2000), Vector2D(-2700,2000));
                Line2D ballLine = Line2D(Vector2D(wm->ball.pos.loc), Vector2D(wm->ball.pos.loc + wm->ball.vel.loc * 1000));

                Vector2D s = goalLine.intersection(ballLine);

                //cout<<s.x<<" "<<s.y<< " "<<wm->ball.vel.loc.length()*1000<<endl;

                if(s.y > 350 || s.y < -350)
                {
                    rc.fin_pos.loc = wm->ourRobot[id].pos.loc;
                    rc.maxSpeed = 2;
                }
                else
                {
                    rc.fin_pos.loc = s;
                    rc.maxSpeed = 2;
                }
            }
            else
            {
                rc.fin_pos.loc = wm->ourRobot[id].pos.loc;
                rc.maxSpeed = 2;
            }
        }
        else
        {
            Line2D goalLine = Line2D(Vector2D(-2700,-2000), Vector2D(-2700,2000));
            Line2D robotLine = Line2D(Vector2D(wm->oppRobot[attackerID].pos.loc), Vector2D(wm->oppRobot[attackerID].pos.loc +
                                                                                           Vector2D::polar2vector(100, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG)));

            Vector2D s = goalLine.intersection(robotLine);
            if(s.y > 350)
                s.y = 350 - ROBOT_RADIUS;
            else if(s.y < 350)
                s.y = -350 + ROBOT_RADIUS;

            rc.fin_pos.loc = s;
            rc.maxSpeed = 2;
        }
    }
    else
    {
        rc.fin_pos.loc = Vector2D(-2700, 0);
        rc.maxSpeed = 2;
    }

    rc.isBallObs = false;
    rc.isKickObs = false;
    return rc;
}

