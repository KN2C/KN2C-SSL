#include "playgameondefensive.h"

PlayGameOnDefensive::PlayGameOnDefensive(WorldModel *worldmodel, QObject *parent) :
    Play("PlayGameOnDefensive", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

    // Left defender.
    tDefenderLeft = new TacticDefender(wm);

    // Right defender.
    tDefenderRight = new TacticDefender(wm);

    // Mid defender.
    tDefenderMid = new TacticDefender(wm);

    // Mid attacker.
    tAttackerMid = new TacticAttacker(wm);
}

int PlayGameOnDefensive::enterCondition()
{
    if(wm->cmgs.gameOn() && wm->kn->ActiveAgents().size() < 4)
        return 100;
    else
        return 0;

    return 0;
}

void PlayGameOnDefensive::execute()
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

    // Define roles according to agents count.
    switch (agents.size() + golieNotInside) {
    case 2:
        roles.append(AgentRole::DefenderMid);

        tDefenderMid->setDefenderID(1, 0);
        break;
    case 3:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);

        tDefenderLeft->setDefenderID(2, 0);
        tDefenderRight->setDefenderID(2, 1);
        break;
    case 4:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);
        roles.append(AgentRole::AttackerMid);

        tDefenderLeft->setDefenderID(2, 0);
        tDefenderRight->setDefenderID(2, 1);
        tAttackerMid->setAttackerID(1, 0);
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
        case AgentRole::AttackerMid:
            tAttackerMid->setID(*itAgent);
            tactics[*itAgent] = tAttackerMid;
        }
    }
}
