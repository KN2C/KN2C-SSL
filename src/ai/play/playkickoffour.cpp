#include "playkickoffour.h"

PlayKickoffOur::PlayKickoffOur(WorldModel *worldmodel, QObject *parent) :
    Play("PlayKickoffOur", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

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

    // Fixed pos.
    Position p;
    tFixedPosM = new TacticFixedPos(wm);
    p.loc = Vector2D(-ROBOT_RADIUS - 20, 0);
    tFixedPosM->SetPosition(p);

    tFixedPosL = new TacticFixedPos(wm);
    p.loc = Vector2D(-800, 1200);
    tFixedPosL->SetPosition(p);

    tFixedPosR = new TacticFixedPos(wm);
    p.loc = Vector2D(-800, -1200);
    tFixedPosR->SetPosition(p);
}

int PlayKickoffOur::enterCondition()
{
    if(wm->cmgs.ourKickoff())
        return 100;
    else
        return 0;
}

void PlayKickoffOur::execute()
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
            roles.append(AgentRole::FixedPositionRight);
        }
        else
        {
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::FixedPositionRight);
        }
        break;
    case 6:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);

        if(!wm->cmgs.canKickBall())
        {
            roles.append(AgentRole::FixedPositionMid);
            roles.append(AgentRole::FixedPositionLeft);
            roles.append(AgentRole::FixedPositionRight);
        }
        else
        {
            roles.append(AgentRole::AttackerMid);
            roles.append(AgentRole::FixedPositionLeft);
            roles.append(AgentRole::FixedPositionRight);
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
        case AgentRole::Golie:
            tGolie->setID(*itAgent);
            tactics[*itAgent] = tGolie;
            break;
        case AgentRole::FixedPositionMid:
            tactics[*itAgent] = tFixedPosM;
            break;
        case AgentRole::FixedPositionRight:
            tactics[*itAgent] = tFixedPosR;
            break;
        case AgentRole::FixedPositionLeft:
            tactics[*itAgent] = tFixedPosL;
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
            tAttacker->setID((*itAgent));
            tactics[*itAgent] = tAttacker;
        default:
            break;
        }
    }
}
