#ifndef _WEIGHTED_VALUE_H_
#define _WEIGHTED_VALUE_H_

template <typename TWEIGHT, typename TV>
class weighted_value
{
public:
    TWEIGHT weight;
    TV value;
};

#endif // _WEIGHTED_VALUE_H_