#include "playfreekickopp.h"

PlayFreeKickOpp::PlayFreeKickOpp(WorldModel *worldmodel, QObject *parent) :
    Play("PlayFreeKickOpp", worldmodel, parent)
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

int PlayFreeKickOpp::enterCondition()
{
    if(wm->cmgs.theiFreeKick() || wm->cmgs.theirDirectKick())
        return 100;
    else
        return 0;

    return 0;
}

void PlayFreeKickOpp::execute()
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
