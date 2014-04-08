#include "playgameon.h"

PlayGameOn::PlayGameOn(WorldModel *worldmodel, QObject *parent) :
    Play("PlayGameOn", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

    // Left defender.
    tDefenderLeft = new TacticDefender(wm);

    // Right defender.
    tDefenderRight = new TacticDefender(wm);

    // Mid defender.
    tDefenderMid = new TacticDefender(wm);

    // Blocker
    tBlocker = new TacticBlocker(wm);

    // Left attacker.
    tAttackerLeft = new TacticAttacker(wm);
    tAttackerLeft->WorkingArea.AddRect(Vector2D(Field::MinX, Field::MaxY), Vector2D(Field::MaxX, Field::MinY));
    tAttackerLeft->WorkingArea.SubtractCircle(Field::ourGoalCenter, 1200);

    // Right attacker.
    tAttackerRight = new TacticAttacker(wm);
    tAttackerRight->WorkingArea.AddRect(Vector2D(Field::MinX, Field::MaxY), Vector2D(Field::MaxX, Field::MinY));
    tAttackerRight->WorkingArea.SubtractCircle(Field::ourGoalCenter, 1200);

    // Mid attacker.
    tAttackerMid = new TacticAttacker(wm);
    tAttackerMid->WorkingArea.AddRect(Vector2D(Field::MinX, Field::MaxY), Vector2D(Field::MaxX, Field::MinY));
    tAttackerMid->WorkingArea.SubtractCircle(Field::ourGoalCenter, 1200);
}

int PlayGameOn::enterCondition()
{
    if(/*wm->cmgs.gameOn() && */wm->kn->ActiveAgents().size() > 3)
        return 100;
    else
        return 0;

    return 0;
}

void PlayGameOn::execute()
{
    QList<int> agents = wm->kn->ActiveAgents();
    QList<int> badRoleAgents;
    QList<AgentRole> roles;

    int golieNotInside = 0;
    if(!wm->ourRobot[wm->ref_goalie_our].isValid)
    {
        golieNotInside = 1;
    }

    // Assign golie role to predefined golie id.
    wm->ourRobot[wm->ref_goalie_our].Role = AgentRole::Golie;

    // We always have two defenders in this play mode.
    tDefenderLeft->setDefenderID(2, 0);
    tDefenderRight->setDefenderID(2, 1);

    // Decide we are attacking or defending.
    double minDistOpp = 10000000, minDistOur = 100000000;
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        double dist;
        if((dist = wm->ourRobot[i].pos.loc.dist(wm->ball.pos.loc)) < minDistOur)
        {
            minDistOur = dist;
        }

        if((dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc)) < minDistOpp)
        {
            minDistOpp = dist;
        }
    }

    // We are defending.
    if(minDistOpp < ROBOT_RADIUS + 100 && minDistOpp < minDistOur)
    {
        // Define roles according to agents count.
        switch (agents.size() + golieNotInside) {
        case 4:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerMid);

            tAttackerMid->setAttackerID(1, 0);
            break;
        case 5:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::Blocker);

            tAttackerMid->setAttackerID(1, 0);
            break;
        case 6:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::Blocker);
            roles.append(AgentRole::Blocker);

            tAttackerMid->setAttackerID(1, 0);
            break;
        }
    }
    // We are attacking.
    else
    {
        // Define roles according to agents count.
        switch (agents.size() + golieNotInside) {
        case 4:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerMid);

            tAttackerMid->setAttackerID(1, 0);
            break;
        case 5:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerLeft);
            roles.append(AgentRole::AttackerRight);

            tAttackerLeft->setAttackerID(2, 0);
            tAttackerRight->setAttackerID(2, 1);
            break;
        case 6:
            roles.append(AgentRole::DefenderLeft);
            roles.append(AgentRole::DefenderRight);
            roles.append(AgentRole::AttackerLeft);
            roles.append(AgentRole::AttackerRight);
            roles.append(AgentRole::AttackerMid);

            tAttackerLeft->setAttackerID(3, 0);
            tAttackerRight->setAttackerID(3, 1);
            tAttackerMid->setAttackerID(3, 2);
            break;
        }
    }    

    for(QList<int>::iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent)
    {
        // Correctly assigned roles.
        if(roles.contains(wm->ourRobot[*itAgent].Role))
        {
            roles.removeOne(wm->ourRobot[*itAgent].Role);
        }
        // Agents with bad role.
        else if(wm->ourRobot[*itAgent].Role != AgentRole::Golie)
        {
            badRoleAgents.append(*itAgent);
        }
    }

    // Assign role to badrole agents.
    for(QList<int>::iterator itAgent = badRoleAgents.begin(); itAgent != badRoleAgents.end(); ++itAgent)
    {
        if(roles.size() > 0)
        {
            wm->ourRobot[*itAgent].Role = roles[0];
            roles.removeAt(0);
        }
    }

    // Set agents tactics.
    for(QList<int>::iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent)
    {
        switch (wm->ourRobot[*itAgent].Role) {
        case AgentRole::NoRole:
            tactics[*itAgent] = nullptr;
            break;
        case AgentRole::Golie:
            tGolie->setID(*itAgent);
            tactics[*itAgent] = tGolie;
            break;
        case AgentRole::DefenderLeft:
            tDefenderLeft->setID(*itAgent);
            tactics[*itAgent] = tDefenderLeft;
            break;
        case AgentRole::DefenderRight:
            tDefenderRight->setID(*itAgent);
            tactics[*itAgent] = tDefenderRight;
            break;
        case AgentRole::DefenderMid:
            tDefenderMid->setID(*itAgent);
            tactics[*itAgent] = tDefenderMid;
            break;
        case AgentRole::Blocker:
            tBlocker->setID(*itAgent);
            tactics[*itAgent] = tBlocker;
        break;
        case AgentRole::AttackerLeft:
            tAttackerLeft->setID(*itAgent);
            tactics[*itAgent] = tAttackerLeft;
            break;
        case AgentRole::AttackerRight:
            tAttackerRight->setID(*itAgent);
            tactics[*itAgent] = tAttackerRight;
            break;
        case AgentRole::AttackerMid:
            tAttackerMid->setID(*itAgent);
            tactics[*itAgent] = tAttackerMid;
            break;
        }
    }
}

