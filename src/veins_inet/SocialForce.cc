#include "SocialForce.h"

using namespace std;
using namespace Utility;

SocialForce::~SocialForce()
{
    removeCrowd();
    removeWalls();
    removeAGVs();
}

void SocialForce::addAgent(Agent *agent) { crowd.push_back(agent); }

void SocialForce::addWall(Wall *wall) { walls.push_back(wall); }

void SocialForce::addAGV(SFMAGV *agv) { agvs.push_back(agv); }

void SocialForce::removeAgent(int agentId)
{
    if (!crowd.empty())
    {
        crowd.erase(remove_if(begin(crowd), end(crowd), [agentId](Agent *u)
                              { return u->getId() == agentId; }),
                    end(crowd));
    }
}

void SocialForce::removeCrowd()
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
        delete crowd[idx];

    crowd.clear();
}

void SocialForce::removeWalls()
{
    for (unsigned int idx = 0; idx < walls.size(); idx++)
        delete walls[idx];

    walls.clear();
}

void SocialForce::removeAGV()
{
    int lastIdx;
    if (!agvs.empty())
    {
        lastIdx = agvs.size() - 1; // Assign index of last element

        delete agvs[lastIdx];
        agvs.pop_back();
    }
}

void SocialForce::removeAGVs()
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
        delete agvs[idx];

    agvs.clear();
}

void SocialForce::moveCrowd(float stepTime)
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
    {
        // if (crowd[idx]->getIsMoving())
        // {
        //     crowd[idx]->move(crowd, walls, agvs, stepTime);
        // }
        crowd[idx]->move(crowd, walls, agvs, stepTime);
    }
}

void SocialForce::moveAGVs(float stepTime)
{
    vector<Point3f> agentsPosition;
    for (Agent *agent : crowd)
    {
        if (agent->getPosition().distance(Vector3f(0, 0, 0)) > 12.0F)
            continue;
        agentsPosition.push_back(agent->getPosition());
    }

    std::map<int, Point3f> mapAGVIdToPosition;
    for (SFMAGV *agv : agvs)
    {
        if (agv->getIsMoving())
        {
            mapAGVIdToPosition[agv->getId()] = agv->getPosition();
        }
    }

    for (SFMAGV *agv : agvs)
    {
        if (agv->getIsMoving())
        {
            mapAGVIdToPosition.erase(agv->getId());
            vector<Point3f> agvsPosition;
            agentsPosition.reserve(mapAGVIdToPosition.size());
            for (auto const &pair : mapAGVIdToPosition)
            {
                agvsPosition.push_back(pair.second);
            }
            agv->move(stepTime, agentsPosition, agvsPosition);
            Utility::updateAGVPoints(agv);
        }
    }
}
