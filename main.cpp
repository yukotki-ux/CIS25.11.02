#include <algorithm>
#include <deque>
#include <iostream>
#include <random>
#include <string>
#include <vector>

enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

struct Card {
    int rank;
    Suit suit;
};

enum class RoundResult {
    HumanWins,
    ComputerWins
};

const char* rankName(int rank)
{
    switch (rank) {
    case 11: return "Jack";
    case 12: return "Queen";
    case 13: return "King";
    case 14: return "Ace";
    default: return rank >= 2 && rank <= 10 ? "number" : "unknown";
    }
}

const char* suitName(Suit suit)
{
    switch (suit) {
    case Suit::Clubs: return "Clubs";
    case Suit::Diamonds: return "Diamonds";
    case Suit::Hearts: return "Hearts";
    case Suit::Spades: return "Spades";
    }
    return "Unknown";
}

std::string cardName(const Card& card)
{
    if (card.rank >= 2 && card.rank <= 10) {
        return std::to_string(card.rank) + " of " + suitName(card.suit);
    }
    return std::string(rankName(card.rank)) + " of " + suitName(card.suit);
}

void addPileToDeck(std::deque<Card>& deck, const std::vector<Card>& pile)
{
    for (std::vector<Card>::const_iterator card = pile.begin(); card != pile.end(); ++card) {
        deck.push_back(*card);
    }
}

void transferAll(std::deque<Card>& from, std::deque<Card>& to)
{
    while (!from.empty()) {
        to.push_back(from.front());
        from.pop_front();
    }
}

RoundResult playRound(std::deque<Card>& humanDeck,
                      std::deque<Card>& computerDeck,
                      int& comparisons)
{
    std::vector<Card> pile;
    Card humanCard = humanDeck.front();
    humanDeck.pop_front();
    Card computerCard = computerDeck.front();
    computerDeck.pop_front();
    pile.push_back(humanCard);
    pile.push_back(computerCard);
    ++comparisons;

    std::cout << "You: " << cardName(humanCard)
              << " | Computer: " << cardName(computerCard) << '\n';

    while (humanCard.rank == computerCard.rank) {
        std::cout << "War!\n";

        if (humanDeck.size() < 4) {
            std::cout << "You cannot provide four cards for War. The computer wins.\n";
            addPileToDeck(computerDeck, pile);
            transferAll(humanDeck, computerDeck);
            return RoundResult::ComputerWins;
        }
        if (computerDeck.size() < 4) {
            std::cout << "The computer cannot provide four cards for War. You win.\n";
            addPileToDeck(humanDeck, pile);
            transferAll(computerDeck, humanDeck);
            return RoundResult::HumanWins;
        }

        for (int cardIndex = 0; cardIndex < 3; ++cardIndex) {
            pile.push_back(humanDeck.front());
            humanDeck.pop_front();
            pile.push_back(computerDeck.front());
            computerDeck.pop_front();
        }

        humanCard = humanDeck.front();
        humanDeck.pop_front();
        computerCard = computerDeck.front();
        computerDeck.pop_front();
        pile.push_back(humanCard);
        pile.push_back(computerCard);
        ++comparisons;

        std::cout << "War cards: You: " << cardName(humanCard)
                  << " | Computer: " << cardName(computerCard) << '\n';
    }

    if (humanCard.rank > computerCard.rank) {
        addPileToDeck(humanDeck, pile);
        std::cout << "You win the pile.\n";
        return RoundResult::HumanWins;
    }

    addPileToDeck(computerDeck, pile);
    std::cout << "The computer wins the pile.\n";
    return RoundResult::ComputerWins;
}

int main()
{
    std::vector<Card> cards;
    for (int rank = 2; rank <= 14; ++rank) {
        for (int suit = 0; suit < 4; ++suit) {
            cards.push_back(Card{rank, static_cast<Suit>(suit)});
        }
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::shuffle(cards.begin(), cards.end(), generator);

    std::deque<Card> humanDeck;
    std::deque<Card> computerDeck;
    for (std::size_t cardIndex = 0; cardIndex < cards.size(); ++cardIndex) {
        if (cardIndex % 2 == 0) {
            humanDeck.push_back(cards[cardIndex]);
        } else {
            computerDeck.push_back(cards[cardIndex]);
        }
    }

    std::cout << "War: You vs. Computer\n"
              << "Each player starts with 26 cards. Press Enter to flip; enter q to quit.\n";

    int comparisons = 0;
    while (humanDeck.size() < 52 && computerDeck.size() < 52) {
        std::cout << "\nYour cards: " << humanDeck.size()
                  << " | Computer cards: " << computerDeck.size() << '\n'
                  << "Press Enter to play: ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cout << "\nInput ended. Game stopped.\n";
            return 0;
        }
        if (input == "q" || input == "Q") {
            std::cout << "Game stopped.\n";
            return 0;
        }

        playRound(humanDeck, computerDeck, comparisons);
    }

    if (humanDeck.size() == 52) {
        std::cout << "\nYou collected all 52 cards and won in "
                  << comparisons << " comparisons.\n";
    } else {
        std::cout << "\nThe computer collected all 52 cards and won in "
                  << comparisons << " comparisons.\n";
    }

    return 0;
}