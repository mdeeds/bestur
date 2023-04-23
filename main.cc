#include <iostream>

#include "game-state.h"

// The alpha-beta pruning algorithm
double prob_alpha_beta(const GameState& state, int remaining_depth, 
  double alpha, double beta) {
  // If the game is over, return the heuristic value of the state
  if (remaining_depth == 0) {
    return state.GetHeuristicValue();
  }
  const std::vector<GameState> next_moves = state.GetNextMoves();
  if (next_moves.empty()) {
    return state.GetHeuristicValue();
  }

  if (state.need_to_toss) {
    // We need to combine all possibilities.
    double value = 0.0;
    for (const GameState& child : next_moves) {
      value += child.probability *
        prob_alpha_beta(child, remaining_depth - 1, alpha, beta);
    }
    return value;
  } else if (state.player_turn == 1) {
    // If it is the maximizing player's turn,
    // find the maximum possible value of the game state
    double best_value = 0.0;
    for (const GameState &child : next_moves) {
      best_value = std::max(best_value, prob_alpha_beta(child, remaining_depth - 1, alpha, beta));
      alpha = std::max(alpha, best_value);
      if (beta <= alpha) {
        break;
      }
    }
    return best_value;
  } else {
    // If it is the minimizing player's turn,
    // find the minimum possible value of the game state
    double best_value = 1.0;;
    for (const GameState &child : next_moves) {
      best_value = std::min(best_value, prob_alpha_beta(child, remaining_depth - 1, alpha, beta));
      beta = std::min(beta, best_value);
      if (beta <= alpha) {
        break;
      }
    }
    return best_value;
  }
}


int main() {
  // Create the initial game state
  GameState state;

  // Print the minimax value of the game state
  std::cout << "Alpha-Beta value: " << prob_alpha_beta(
    state, /*remaining_depth=*/20, 
    /*initial_alpha=*/0, /*initial_beta=*/100) << std::endl;

  return 0;
}