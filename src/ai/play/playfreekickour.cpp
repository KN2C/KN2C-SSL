#include "playfreekickour.h"

PlayFreeKickOur::PlayFreeKickOur(WorldModel *worldmodel, QObject *parent) :
    Play("PlayFreeKickOur", worldmodel, parent)
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

    // Attacker.
    tAttacker = new TacticAttacker(wm);
    tAttacker->WorkingArea.AddRect(Vector2D(Field::MinX, Field::MaxY), Vector2D(Field::MaxX, Field::MinY));
    tAttacker->WorkingArea.SubtractCircle(Field::ourGoalCenter, 1200);
    tAttacker->setAttackerID(1, 0);
}

int PlayFreeKickOur::enterCondition()
{
    if(wm->cmgs.ourFreeKick() || wm->cmgs.ourIndirectKick())
        return 100;
    else
        return 0;

    return 0;
}

void PlayFreeKickOur::execute()
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

        if(!wm->cmgs.canKickBall())
        {
            roles.append(AgentRole::FixedPositionMid);
        }
        else
        {
            roles.append(AgentRole::AttackerMid);
        }
        break;
    case 5:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);

        if(!wm->cmgs.canKickBall())
        {
            roles.append(AgentRole::FixedPositionMid);
            roles.append(AgentRole::ArcRight);
        }
        else
        {
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::ArcRight);
        }
        break;
    case 6:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);

        if(!wm->cmgs.canKickBall())
        {
            roles.append(AgentRole::FixedPositionMid);
            roles.append(AgentRole::ArcLeft);
            roles.append(AgentRole::ArcRight);
        }
        else
        {
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::ArcLeft);
            roles.append(AgentRole::ArcRight);
        }

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
        case AgentRole::FixedPositionMid:
            tactics[*itAgent] = tFixedPosM;
            break;
        case AgentRole::ArcLeft:
            tactics[*itAgent] = tFixedPosL;
            break;
        case AgentRole::ArcRight:
            tactics[*itAgent] = tFixedPosR;
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
        case AgentRole::AttackerMid:
            tAttacker->setID(*itAgent);
            tactics[*itAgent] = tAttacker;
            break;
        default:
            break;
        }
    }
}
