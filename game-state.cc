#include "game-state.h"

#include <algorithm>

static const std::vector<double> kProbabilities({0.125, 0.375, 0.375, 0.125});
static const std::vector<int> kSpaces({4, 1, 2, 3});

void GameState::AddMoveOutStates(std::vector<GameState>* children) const {
  const PlayerState* current_player = player_turn_ == 1 ? &player_1_ : &player_2_;

  if (current_player->pieces_at_start_ == 0) {
    return;
  }
  if (current_player->track_[spaces_to_move_ + 1] == 0) {
    // Space is open, free to move.
    GameState next_state(*this);

   PlayerState* next_current_player_state = player_turn_ == 1 ? 
       &next_state.player_1_ : &next_state.player_2_;

    next_current_player_state->pieces_at_start_--;
    next_current_player_state->track_[spaces_to_move_ + 1] = 1;
    next_state.EndTurn();
    children->push_back(std::move(next_state));
  }
};

void GameState::AddMoveAlongStates(std::vector<GameState>* children) const {
  const PlayerState* current_player = player_turn_ == 1 ? &player_1_ : &player_2_;
  for (size_t i = 0; i < current_player->track_.size(); ++i) {
    if (current_player->track_[i] > 0 &&
        i + spaces_to_move_ < current_player->track_.size() &&
        current_player->track_[i + spaces_to_move_] == 0) {
      GameState next_state(*this);
      PlayerState* next_current_player_state = player_turn_ == 1 ? 
           &next_state.player_1_ : &next_state.player_2_;

      next_current_player_state->track_[i] = 0;
      next_current_player_state->track_[i + spaces_to_move_] = 1;
      next_state.EndTurn();
      children->push_back(std::move(next_state));
    }
  }
};

void GameState::AddMoveHomeStates(std::vector<GameState>* children) const {
  const PlayerState* current_player = player_turn_ == 1 ? &player_1_ : &player_2_;
  for (size_t i = 0; i < current_player->track_.size(); ++i) {
    if (current_player->track_[i] > 0 &&
        i + spaces_to_move_ == current_player->track_.size()) {
      GameState next_state(*this);
      PlayerState* next_current_player_state = player_turn_ == 1 ? 
           &next_state.player_1_ : &next_state.player_2_;

      next_current_player_state->track_[i] = 0;
      next_current_player_state->pieces_at_end_++;
      next_state.EndTurn();
      children->push_back(std::move(next_state));
    }
  }
};

void GameState::EndTurn() {
  need_to_toss_ = true;
  player_turn_ = (player_turn_ == 1) ? 2 : 1;
}

const std::vector<GameState>
GameState::GetNextMoves() const {
  std::vector<GameState> result;
  if (need_to_toss_) {
    // Roll dice case
    for (size_t i = 0; i < kProbabilities.size(); ++i) {
      GameState next(*this);
      next.need_to_toss_ = false;
      next.probability_ = kProbabilities[i];
      next.spaces_to_move_ = kSpaces[i];
      result.push_back(std::move(next));
    }
  } else {
    // Move pieces case
    // Move out
    AddMoveOutStates(&result);
    AddMoveAlongStates(&result);
    AddMoveHomeStates(&result);

    // TODO: If there are no legal moves, just return an EndTurn state.

    std::sort(result.begin(), result.end(),
      [](const GameState& a, const GameState& b){ 
        return b.GetHeuristicValue() - a.GetHeuristicValue(); });
  }
  return result;
}

int SpacesRemaining(const PlayerState& state) {
  int total = state.pieces_at_start_ * kTrackLength;
  for (int i = 0; i < kTrackLength; ++i) {
    if (state.track_[i] > 0) {
      total += kTrackLength - i;
    }
  }
  return total;
}

// Returns a heuristic value for the probability that player 1 will win.
double GameState::GetHeuristicValue() const {
  double spaces_remaining_1 = SpacesRemaining(player_1_);
  double spaces_remaining_2 = SpacesRemaining(player_2_);
  if (spaces_remaining_1 == 0) { return 1.0; }
  if (spaces_remaining_2 == 0) { return 0.0; }
  return spaces_remaining_2 / (spaces_remaining_1 + spaces_remaining_2); 
}