#include "playstop.h"

PlayStop::PlayStop(WorldModel *worldmodel, QObject *parent) :
    Play("PlayStop", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

    // Fixed pos.
    tFixedPosM = new TacticFixedPos(wm);

    // Fixed pos.
    tFixedPosL = new TacticFixedPos(wm);

    // Fixed pos.
    tFixedPosR = new TacticFixedPos(wm);

    // Left defender.
    tDefenderLeft = new TacticDefender(wm);

    // Right defender.
    tDefenderRight = new TacticDefender(wm);

    // Mid defender.
    tDefenderMid = new TacticDefender(wm);
}

int PlayStop::enterCondition()
{
    if(wm->gs == STATE_Stop)
        return 100;
    else if(wm->cmgs.canMove() && wm->cmgs.gameOn()==false && wm->cmgs.allowedNearBall()==false)
        return 10;
    else
        return 0;
}

void PlayStop::execute()
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

    // Define roles according to agents count.
    switch (agents.size() + golieNotInside) {
    case 2:
        roles.append(AgentRole::DefenderMid);
        break;
    case 3:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);
        break;
    case 4:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);
        roles.append(AgentRole::ArcMid);
        break;
    case 5:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);
        roles.append(AgentRole::ArcMid);
        roles.append(AgentRole::ArcRight);
        break;
    case 6:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);
        roles.append(AgentRole::ArcMid);
        roles.append(AgentRole::ArcLeft);
        roles.append(AgentRole::ArcRight);
        break;
    }

    for(QList<int>::iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent)
    {
        // Correctly assigned roles.
        if(roles.contains(wm->ourRobot[*itAgent].Role))
        {
            roles.removeOne(wm->ourRobot[*itAgent].Role);
        }
        // Agents with bad role.
        else if(*itAgent != wm->ref_goalie_our)
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
        case AgentRole::ArcMid:
            tactics[*itAgent] = tFixedPosM;
            break;
        case AgentRole::ArcLeft:
            tactics[*itAgent] = tFixedPosL;
            break;
        case AgentRole::ArcRight:
            tactics[*itAgent] = tFixedPosR;
            break;
        case AgentRole::FixedPositionMid:
            tactics[*itAgent] = tFixedPosM;
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
        default:
            break;
        }
    }
}
