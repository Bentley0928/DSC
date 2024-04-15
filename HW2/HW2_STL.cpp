#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct Pokemon
{
    int id;
    int damage;
    int health;
    string stance;
    int level;

    Pokemon(int id, int damage, int health, string stance)
        : id(id), damage(damage), health(health), stance(stance), level(1) {}
};

class Circle
{
private:
    list<Pokemon> pokemons;
    list<Pokemon>::iterator target;

public:
    Circle()
    {
        target = pokemons.end();
    }

    void insert(int id, int damage, int health, string stance)
    {
        Pokemon newPokemon(id, damage, health, stance);

        if (pokemons.empty())
        {
            pokemons.push_back(newPokemon);
            target = pokemons.begin();
        }
        else
        {
            auto newitem = pokemons.insert(target, newPokemon);
            int length = pokemons.size();
            if (stance == "Evolve")
            {
                if (length > 2)
                {
                    auto leftNeighbor = newitem;
                    if (newitem != pokemons.begin())
                    {
                        leftNeighbor = prev(newitem);
                    }
                    else
                        leftNeighbor = prev(pokemons.end());
                    auto rightNeighbor = next(newitem);
                    if (rightNeighbor == pokemons.end())
                    {
                        rightNeighbor = pokemons.begin();
                    }
                    if (leftNeighbor->id == id && rightNeighbor->id == id && leftNeighbor->level < 3 && rightNeighbor->level < 3)
                    {
                        newitem->health = max({leftNeighbor->health, newitem->health, rightNeighbor->health});
                        newitem->damage = max({leftNeighbor->damage, newitem->damage, rightNeighbor->damage});
                        newitem->level = max(leftNeighbor->level, rightNeighbor->level) + 1;
                        if (leftNeighbor == target || rightNeighbor == target)
                            target = newitem;
                        pokemons.erase(leftNeighbor);
                        pokemons.erase(rightNeighbor);
                    }
                }
            }
            else if (stance == "Attack")
            {
                if (length == 2)
                {
                    target->health -= damage;
                    if (target->health <= 0)
                    {
                        pokemons.erase(target);
                        target = newitem;
                    }
                }
                else if (length > 2)
                {

                    auto leftNeighbor = newitem;
                    if (newitem != pokemons.begin())
                    {
                        leftNeighbor = prev(newitem);
                    }
                    else
                        leftNeighbor = prev(pokemons.end());
                    auto rightNeighbor = next(newitem);
                    if (rightNeighbor == pokemons.end())
                    {
                        rightNeighbor = pokemons.begin();
                    }
                    leftNeighbor->health -= damage;
                    if (leftNeighbor->health <= 0)
                    {
                        if (leftNeighbor == target)
                            target = newitem;
                        pokemons.erase(leftNeighbor);
                    }
                    rightNeighbor->health -= damage;
                    if (rightNeighbor->health <= 0)
                    {
                        if (rightNeighbor == target)
                            target = newitem;
                        pokemons.erase(rightNeighbor);
                    }
                }
            }
        }
    }

    void deleteCurrent()
    {
        if (pokemons.empty())
            return;

        auto toDelete = target;
        advance(target, 1);
        if (target == pokemons.end())
        {
            target = pokemons.begin();
        }
        pokemons.erase(toDelete);
    }

    void shuffle(char dir, int steps)
    {
        // cout << target -> id << endl;
        int length = pokemons.size();
        if (length < 2)
            return;

        steps %= length;
        auto current = target;
        if (dir == 'c')
        {
            for (int i = 0; i < steps; i++)
            {
                current = std::next(current);
                if (current == pokemons.end())
                {
                    current = pokemons.begin();
                }
            }
        }
        else
        {
            for (int i = 0; i < steps; i++)
            {
                if (current == pokemons.begin())
                {
                    current = pokemons.end();
                }
                current = std::prev(current);
            }
        }

        if (current != target)
        {
            std::iter_swap(target, current);
            target = current;
            // cout << target -> id << endl;
        }
    }

    void check(int distance)
    {
        vector<int> count(101, 0);
        if (pokemons.size() < 2)
            return;

        auto start = target;
        for (int i = 0; i < distance; i++)
        {
            if (start == pokemons.end())
                start = pokemons.begin();
            count[start->id]++;
            advance(start, 1);
        }
        start = target;
        for (int i = 0; i < distance; i++)
        {
            if (start == pokemons.end())
                start = pokemons.begin();
            if (count[start->id] > 1)
            {
                if (target == start)
                {
                    target = next(target);
                    if (target == pokemons.end())
                        target = pokemons.begin();
                }
                auto todel = start;
                advance(start, 1);
                pokemons.erase(todel);
            }
            else
                advance(start, 1);
        }
    }

    void reverse(int groupSize)
    {
        list<Pokemon> pokemonstmp;
        int len2 = pokemons.size();
        int pcdlen = 0;
        if (len2 < 2 || groupSize >= len2)
        {
            vector<decltype(target)> nodes;
            auto it = target;
            while (len2--)
            {

                nodes.push_back(it);
                advance(it, 1);
                if (it == pokemons.end())
                    it = pokemons.begin();
            }
            std::reverse(nodes.begin(), nodes.end());
            for (auto n : nodes)
            {
                // cout << start->id << endl;
                pokemonstmp.push_back(*n);
            }
            pokemons = pokemonstmp;
            target = pokemons.begin();
            return;
        }

        auto start = target;
        do
        {
            auto end = start;
            int count = min(groupSize, len2 - pcdlen);
            pcdlen += count;
            while (count--)
            {
                advance(end, 1);
                if (end == pokemons.end())
                {
                    end = pokemons.begin();
                }
            }
            vector<decltype(start)> nodes;
            auto it = start;
            while (it != end)
            {
                nodes.push_back(it);
                advance(it, 1);
                if (it == pokemons.end())
                    it = pokemons.begin();
            }
            std::reverse(nodes.begin(), nodes.end());
            for (auto n : nodes)
            {
                // cout << start->id << endl;
                pokemonstmp.push_back(*n);
            }
            start = end;
            cout << endl;
        } while (pcdlen < len2);
        pokemons = pokemonstmp;
        target = pokemons.begin();
    }

    void display()
    {
        if (pokemons.empty())
        {
            cout << "no Poke Poke ;-;" << endl;
        }
        else
        {
            auto it = target;
            do
            {
                cout << "ID: " << it->id << " HP: " << it->health << " LVL: " << it->level << endl;
                advance(it, 1);
                if (it == pokemons.end())
                    it = pokemons.begin();
            } while (it != target);
        }
    }
};

int main()
{
    int n;
    cin >> n;

    Circle circle; 
    for (int i = 0; i < n; ++i)
    {
        string command;
        cin >> command;
        if (command == "Insert")
        {
            int id, damage, health;
            string stance;
            cin >> id >> damage >> health >> stance;
            circle.insert(id, damage, health, stance);
        }
        else if (command == "Delete")
        {
            circle.deleteCurrent();
        }
        else if (command == "Shuffle")
        {
            char dir;
            int steps;
            cin >> dir >> steps;
            circle.shuffle(dir, steps);
        }
        else if (command == "Check")
        {
            int distance;
            cin >> distance;
            circle.check(distance);
        }
        else if (command == "Reverse")
        {
            int groupSize;
            cin >> groupSize;
            circle.reverse(groupSize);
        }
    }

    circle.display();
    return 0;
}
