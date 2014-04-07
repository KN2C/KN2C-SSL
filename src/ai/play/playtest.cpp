#include "playtest.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play("PlayTest", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);

    // Left defender.
    tDefenderLeft = new TacticDefender(wm);

    // Right defender.
    tDefenderRight = new TacticDefender(wm);

    // Mid defender.
    tDefenderMid = new TacticDefender(wm);

    // Left attacker.
    tAttackerLeft = new TacticAttacker(wm);

    // Right attacker.
    tAttackerRight = new TacticAttacker(wm);

    // Mid attacker.
    tAttackerMid = new TacticAttacker(wm);

    tTestKickPrecision = new TacticTestKickPrecision(wm);
}

int PlayTest::enterCondition()
{
    // Activator condition.
    if(wm->kn->ActiveAgents().size() > 2)
    {
        return 150;
    }

    return 0;
}

void PlayTest::execute()
{
    QList<int> agents = wm->kn->ActiveAgents();
    QList<int> badRoleAgents;
    QList<AgentRole> roles;

    // Assign golie role to predefined golie id.
    wm->ourRobot[wm->ref_goalie_our].Role = AgentRole::Golie;

    // We always have two defenders in this play mode.
    tDefenderLeft->setDefenderID(2, 0);
    tDefenderRight->setDefenderID(2, 1);

    // Define roles according to agents count.
    switch (agents.size()) {
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
            tactics[*itAgent] = tTestKickPrecision;
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

