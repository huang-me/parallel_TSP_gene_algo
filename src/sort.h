#ifndef MY_SORT_H
#define MY_SORT_H

typedef std::pair<std::vector<int>, int> my_pair;

// sort vector with pair
struct sort_pred {
  bool operator()(const my_pair &firstElem, const my_pair &secondElem) {
    return firstElem.second < secondElem.second;
  }
};

#endif
