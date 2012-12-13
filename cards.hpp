/*
**  CARDS.HPP - Declare card classes
**
**  public domain by Bob Stout
*/

#ifndef CARDS__HPP
#define CARDS__HPP

const int Card_Error = -1;
const int Deck_Size  = 52;

typedef enum {Rank_Error = Card_Error, Ace = 1, Deuce, Trey, Spot_4, Spot_5,
              Spot_6, Spot_7, Spot_8, Spot_9, Spot_10, Jack = 11, Queen = 12,
              King = 13} cardRank;

typedef enum {Suit_Error = Card_Error, Diamond, Club, Heart, Spade} cardSuit;


class card
{
private:
      cardRank rank_;
      cardSuit suit_;

public:
      card(void);
      card(cardSuit s, cardRank r);
      cardRank rank(void);
      cardSuit suit(void);
      void get_card(cardSuit &s, cardRank &r);
      char *rankText(void);
      char *suitText(void);
      void set_rank(cardRank r);
      void set_suit(cardSuit s);
      void set_card(cardSuit s, cardRank r);
};

class deck
{
private:
      class card card_[Deck_Size];
      unsigned top;

public:
      deck(void);
      void shuffle(void);
      void deal(class card &c);
      int cards_left(void);
};

#endif // CARDS__HPP
