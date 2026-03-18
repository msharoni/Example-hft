// Market state stored on-chain
type MarketState {
  highest_price: Int,
  trade_count: Int,
}

// Action submitted by a user
type TradeAction {
  new_price: Int,
}

// Update the market after a trade
fn process_trade(state: MarketState, action: TradeAction) -> MarketState {
  let highest = if action.new_price > state.highest_price {
    action.new_price
  } else {
    state.highest_price
  };

  MarketState {
    highest_price: highest,
    trade_count: state.trade_count + 1,
  }
}
