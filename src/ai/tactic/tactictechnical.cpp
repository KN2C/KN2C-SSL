#include "tactictechnical.h"

TacticTechnical::TacticTechnical(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTechnical", worldmodel, parent)
{
    direction = 0;
    corner = 0;

    bbl = bbr = btr = btl = false;
}

RobotCommand TacticTechnical::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    bool dontmove = false;

    switch (corner) {
    case 0:
        if(!bbr && wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc, Field::bottomRightCorner, 60))
        {
            bbr = true;
            corner = (corner + 1) % 4;
        }
        else
        {
            bbr = false;
        }
        break;
    case 1:
        if(!btr && wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc, Field::upperRightCorner, 60))
        {
            btr = true;
            corner = (corner + 1) % 4;
        }
        else
        {
            btr = false;
        }

        break;
    case 2:
        if(!btl && wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc, Field::upperLeftCorner, 60))
        {
            btl = true;
            corner = (corner + 1) % 4;
        }
        else
        {
            btl = false;
        }

        break;
    case 3:
        if(!bbl && wm->kn->ReachedToPos(wm->ourRobot[id].pos.loc, Field::bottomLeftCorner, 60))
        {
            bbl = true;
            corner = (corner + 1) % 4;
        }
        else
        {
            bbl = false;
        }
        break;
    }

    switch (corner) {
    case 0:
    {
        double mindist = 100000000;
        int minID = 0;
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->oppRobot[i].isValid && wm->oppRobot[i].pos.loc.x > wm->ourRobot[id].pos.loc.x)
            {
                double dist = wm->ourRobot[id].pos.loc.dist(wm->oppRobot[i].pos.loc);

                if(dist < mindist)
                {
                    mindist = dist;
                    minID = i;
                }
            }
        }

        if(fabs(wm->ourRobot[id].pos.loc.x - wm->oppRobot[minID].pos.loc.x) < 500)
        {
            if(wm->oppRobot[minID].pos.loc.y - wm->ourRobot[id].pos.loc.y  < 350 && wm->oppRobot[minID].vel.loc.dir().radian() <= 0)
            {
                dontmove = true;
            }
        }

        rc.fin_pos.loc = Field::bottomRightCorner;
      //  rc.fin_pos.loc.y += 120;
    }
        break;
    case 1:
    {
        double mindist = 100000000;
        int minID = 0;
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->oppRobot[i].isValid && wm->oppRobot[i].pos.loc.y > wm->ourRobot[id].pos.loc.y)
            {
                double dist = wm->ourRobot[id].pos.loc.dist(wm->oppRobot[i].pos.loc);

                if(dist < mindist)
                {
                    mindist = dist;
                    minID = i;
                }
            }
        }

        if(fabs(wm->ourRobot[id].pos.loc.y - wm->oppRobot[minID].pos.loc.y) < 500)
        {
            if(wm->ourRobot[id].pos.loc.x - wm->oppRobot[minID].pos.loc.x < 350 && fabs(wm->oppRobot[minID].vel.loc.dir().radian()) < 0.5)
            {
                dontmove = true;
            }
        }

        rc.fin_pos.loc = Field::upperRightCorner;
       // rc.fin_pos.loc.x -= 120;
    }
        break;
    case 2:
    {
        double mindist = 100000000;
        int minID = 0;
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->oppRobot[i].isValid && wm->oppRobot[i].pos.loc.x < wm->ourRobot[id].pos.loc.x)
            {
                double dist = wm->ourRobot[id].pos.loc.dist(wm->oppRobot[i].pos.loc);

                if(dist < mindist)
                {
                    mindist = dist;
                    minID = i;
                }
            }
        }

        if(fabs(wm->oppRobot[minID].pos.loc.x - wm->ourRobot[id].pos.loc.x) < 500)
        {
            if(wm->ourRobot[id].pos.loc.y - wm->oppRobot[minID].pos.loc.y  < 350 && wm->oppRobot[minID].vel.loc.dir().radian() >= 0)
            {
                dontmove = true;
            }
        }
        rc.fin_pos.loc = Field::upperLeftCorner;
       // rc.fin_pos.loc.y -= 120;
    }
        break;
    case 3:
    {
        double mindist = 100000000;
        int minID = 0;
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->oppRobot[i].isValid && wm->oppRobot[i].pos.loc.y < wm->ourRobot[id].pos.loc.y)
            {
                double dist = wm->ourRobot[id].pos.loc.dist(wm->oppRobot[i].pos.loc);

                if(dist < mindist)
                {
                    mindist = dist;
                    minID = i;
                }
            }
        }

        if(fabs(wm->ourRobot[id].pos.loc.y - wm->oppRobot[minID].pos.loc.y) < 500)
        {
            if(wm->oppRobot[minID].pos.loc.x -wm->ourRobot[id].pos.loc.x < 350 && fabs(wm->oppRobot[minID].vel.loc.dir().radian()) > 2
                    || wm->oppRobot[minID].vel.loc.dir().radian() == 0)
            {
                dontmove = true;
            }
        }

        rc.fin_pos.loc = Field::bottomLeftCorner;
        //rc.fin_pos.loc.x += 120;
    }
        break;
    }

    if(dontmove)
    {
        rc.fin_pos = wm->ourRobot[id].pos;
    }

    rc.useNav = false;
    rc.maxSpeed = 1;

    return rc;
}

