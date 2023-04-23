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
  PlayerState() : track_(kTrackLength) {}
  int pieces_at_start_ = 3;
  int pieces_at_end_ = 0;
  std::vector<int> track_;
};

// A class to represent a game state
class GameState {
 public:
  int player_turn_;  // 1 or 2.
  float probability_;  // The a priority probability of this state (i.e. die roll probability.)
  int spaces_to_move_;
  bool need_to_toss_ = true;

  // Caller owns the returned states.
  const std::vector<GameState> GetNextMoves() const;

  double GetHeuristicValue() const;

  // Mutates this game state, passing the dice to the next player.
  void EndTurn();

 private:
  void AddMoveOutStates(std::vector<GameState>* children) const;
  void AddMoveAlongStates(std::vector<GameState>* children) const;
  void AddMoveHomeStates(std::vector<GameState>* children) const;

  PlayerState player_1_;
  PlayerState player_2_;
};


#endif