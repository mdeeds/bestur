#include "game-state.h"

static const std::vector<double> kProbabilities({0.125, 0.375, 0.375, 0.125});
static const std::vector<int> kSpaces({4, 1, 2, 3});

const std::vector<GameState>
GameState::GetNextMoves() const {
  std::vector<GameState> result;
  if (need_to_toss) {
    // Roll dice case
    for (size_t i = 0; i < kProbabilities.size(); ++i) {
      GameState next(*this);
      next.need_to_toss = false;
      next.probability = kProbabilities[i];
      next.spaces_to_move = kSpaces[i];
      result.push_back(std::move(next));
    }
  } else {
    // Move pieces case
    //TODO
    GameState next(*this);
    next.need_to_toss = true;
    next.player_turn = (next.player_turn == 1) ? 2 : 1;

    result.push_back(std::move(next));
  }
  return result;
}

int SpacesRemaining(const PlayerState& state) {
  int total = state.pieces_at_start * kTrackLength;
  for (int i = 0; i < kTrackLength; ++i) {
    if (state.track[i] > 0) {
      total += kTrackLength - i;
    }
  }
  return total;
}

// Returns a heuristic value for the probability that player 1 will win.
double GameState::GetHeuristicValue() const {
  double spaces_remaining_1 = SpacesRemaining(player_1);
  double spaces_remaining_2 = SpacesRemaining(player_2);
  if (spaces_remaining_1 == 0) { return 1.0; }
  if (spaces_remaining_2 == 0) { return 0.0; }
  return spaces_remaining_2 / (spaces_remaining_1 + spaces_remaining_2); 
}