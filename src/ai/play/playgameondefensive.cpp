#include "playgameondefensive.h"

PlayGameOnDefensive::PlayGameOnDefensive(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

    // Left defender.
    tDefenderLeft = new TacticDefender(wm);

    // Right defender.
    tDefenderRight = new TacticDefender(wm);

    // Mid defender.
    tDefenderMid = new TacticDefender(wm);
}

int PlayGameOnDefensive::enterCondition()
{
    // TODO: decide on game state.

    // Activator condition.
    if(wm->kn->ActiveAgents().size() < 4)
    {
        return 100;
    }

    return 0;
}

void PlayGameOnDefensive::execute()
{
    QList<int> agents = wm->kn->ActiveAgents();
    QList<int> badRoleAgents;
    QList<AgentRole> roles;

    // Assign golie role to predefined golie id.
    wm->ourRobot[GOLIE_ID].Role = AgentRole::Golie;

    // Define roles according to agents count.
    switch (agents.size()) {
    case 2:
        roles.append(AgentRole::DefenderMid);

        tDefenderMid->setDefenderID(1, 0);
        break;
    case 3:
        roles.append(AgentRole::DefenderLeft);
        roles.append(AgentRole::DefenderRight);

        tDefenderMid->setDefenderID(2, 0);
        tDefenderMid->setDefenderID(2, 1);
        break;
    }

    // Append golie role at last so it never reassigned during play.
    roles.append(AgentRole::Golie);


    for(QList<int>::iterator itAgent = agents.begin(); itAgent != agents.end(); ++itAgent)
    {
        // Correctly assigned roles.
        if(roles.contains(wm->ourRobot[*itAgent].Role))
        {
            roles.removeOne(wm->ourRobot[*itAgent].Role);
        }
        // Agents with bad role.
        else
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
        }
    }
}
