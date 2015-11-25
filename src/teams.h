#ifndef _TEAMS_H_
#define _TEAMS_H_

#include "team.h"

#include <vector>

class Teams : public std::vector<TeamPtr_t>
{
  public:
    bool hasHills(void)  const;
    bool hasForest(void) const;

    int  maxViewRange(void) const;

    void resolveFractionalCosts(void);
    void resolveFractionalDistance(int maxRange);
};

#endif // _TEAMS_H_
