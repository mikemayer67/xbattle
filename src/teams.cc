#include "teams.h"

#include <algorithm>

using namespace std;

bool Teams::hasHills(void) const
{
  for(const_iterator x=begin(); x!=end(); ++x)
  {
    if((*x)->_hills>0) return true;
  }
  return false;
}

bool Teams::hasForest(void) const
{
  for(const_iterator x=begin(); x!=end(); ++x)
  {
    if((*x)->_forest>0) return true;
  }
  return false;
}

int Teams::maxViewRange(void) const
{
  int rval(0);

  for(const_iterator x=begin(); x!=end(); ++x)
  {
    TeamPtr_t tp = *x;
    if(tp->_hasHorizon && tp->_horizon>rval) rval = tp->_horizon;
  }
  return rval;
}


void Teams::resolveFractionalCosts(void)
{
  int maxValue(0);
  for(const_iterator x=begin(); x!=end(); ++x)
  {
    maxValue = max(maxValue, (*x)->_maxval);
  }
  for(const_iterator x=begin(); x!=end(); ++x)
  {
    TeamPtr_t tp = *x;
    if(tp->_buildCost==0)   tp->_buildCost   = max( 1, int(0.5 + maxValue * tp->_buildCostFrac)   );
    if(tp->_scuttleCost==0) tp->_scuttleCost = max( 1, int(0.5 + maxValue * tp->_scuttleCostFrac) );
    if(tp->_digCost==0)     tp->_digCost     = max( 1, int(0.5 + maxValue * tp->_digCostFrac)     );
    if(tp->_fillCost==0)    tp->_fillCost    = max( 1, int(0.5 + maxValue * tp->_fillCostFrac)    );
  }
}

void Teams::resolveFractionalDistance(int maxRange)
{
  for(const_iterator x=begin(); x!=end(); ++x)
  {
    TeamPtr_t tp = *x;
    if(tp->_gunRange==0)  tp->_gunRange  = max( 1, int(0.5 + maxRange * tp->_gunRangeFrac)  );
    if(tp->_paraRange==0) tp->_paraRange = max( 1, int(0.5 + maxRange * tp->_paraRangeFrac) );
  }
}

void Teams::validateDrawMethod(TileShape_t shape)
{
  for(const_iterator x=begin(); x!=end(); ++x)
    (*x)->validateDrawMethod(shape);
}
