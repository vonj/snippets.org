/*
**  CARDS.CPP - Playing card classes
**
**  public domain by Bob Stout
*/

#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cards.hpp"

const char *suits[] = {"Diamonds", "Clubs", "Hearts", "Spades"};
const char *cards[] = {"Ace", "Deuce", "Trey", "4", "5", "6", "7", "8", "9",
                       "10", "Jack", "Queen", "King"};


inline card::card(void)
{
      rank_ = Rank_Error;
      suit_ = Suit_Error;
}

inline card::card(cardSuit s, cardRank r)
{
      rank_ = r;
      suit_ = s;
}

inline cardRank card::rank(void)
{
      return rank_;
}

inline cardSuit card::suit(void)
{
      return suit_;
}

inline char * card::rankText(void)
{
      return (char *)cards[(int)rank_ - 1];
}

inline char * card::suitText(void)
{
      return (char *)suits[(int)suit_];
}

inline void card::set_rank(cardRank r)
{
      rank_ = r;
}

inline void card::set_suit(cardSuit s)
{
      suit_ = s;
}

inline void card::set_card(cardSuit s, cardRank r)
{
      rank_ = r;
      suit_ = s;
}

inline void card::get_card(cardSuit &s, cardRank &r)
{
      r = rank_;
      s = suit_;
}


deck::deck(void)
{
      int n = 0;

      for (int s = int(Diamond); s <= int(Spade); ++s)
      {
            for (int c = int(Ace); c <= int(King); ++c)
            {
                  deck::card_[n].set_rank(cardRank(c));
                  deck::card_[n].set_suit(cardSuit(s));
                  ++n;
            }
      }
      top = 0;
}

inline void deck::deal(class card &c)
{
      if (top < Deck_Size)
            c = deck::card_[top++];
}

inline int deck::cards_left(void)
{
      return int(Deck_Size - top);
}

void deck::shuffle(void)
{
      int used[Deck_Size], posn = 0;

      srand((unsigned)time(NULL) | 1);
      memset(used, 0, Deck_Size * sizeof(int));

      for (int s = int(Diamond); s <= int(Spade); ++s)
      {
            for (int c = int(Ace); c <= int(King); ++c)
            {
                  posn = (posn + rand()) % Deck_Size;
                  while (used[posn])
                        posn = ++posn % Deck_Size;
                  deck::card_[posn].set_rank(cardRank(c));
                  deck::card_[posn].set_suit(cardSuit(s));
                  used[posn] = -1;
            }
      }
      top = 0;
}

#ifdef TEST

#include <iostream.h>

void showem(class deck &d);

main()
{
      class deck d;

      cout << "Deck initially:" << endl << endl;
      showem(d);
      d.shuffle();
      cout << endl << "After shuffling:" << endl << endl;
      showem(d);
      return EXIT_SUCCESS;
}

void showem(class deck &d)
{
      for (int i = 0; i <= Deck_Size; ++i)
      {
            class card card_;

            if (0 < d.cards_left())
            {
                  d.deal(card_);
                  cout << "Card #";
                  cout.width(2);
                  cout.setf(ios::left, ios::adjustfield);
                  cout << (i + 1) << " - " <<
                        card_.rankText() << " of " <<
                        card_.suitText() << endl;
            }
            else  cout << "No cards left" << endl;
      }
}

#endif // TEST
