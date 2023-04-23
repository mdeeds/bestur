#ifndef GAME_STATE
#define GAME_STATE

#include <vector>


// Length of track (in squares)
constexpr int kTrackLength = 16;

// The game of Ur
//
// [12]->*11*-> End2 Start2 [  ]->[  ]->[  ]->[  ]  
//  v    ^                                v
// [13]<-[10]<- [ 9]<-[ 8]<-* 7*<-[ 6]<-[ 5]<-[ 4]
//  ^    v                                ^
// [14]->*15*-> End1 Start1 [ 0]->[ 1]->[ 2]->* 3*

class PlayerState {
 public:
  PlayerState() : track(kTrackLength) {}
  int pieces_at_start = 3;
  int pieces_at_end = 0;
  std::vector<int> track;
};

// A class to represent a game state
class GameState {
 public:
  int player_turn;  // 1 or 2.
  float probability;  // The a priority probability of this state (i.e. die roll probability.)
  int spaces_to_move;
  bool need_to_toss = true;

  // Caller owns the returned states.
  const std::vector<GameState> GetNextMoves() const;

  double GetHeuristicValue() const;

 private:
  PlayerState player_1;
  PlayerState player_2;
};


#endif