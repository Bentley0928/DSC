#include <iostream>
#include <string>

using namespace std;
int maxthree(int a, int b, int c)
{
  if (a > b)
  {
    if (a > c)
    {
      return a; // a是最大的
    }
    else
    {
      return c; // c是最大的
    }
  }
  else
  {
    if (b > c)
    {
      return b; // b是最大的
    }
    else
    {
      return c; // c是最大的
    }
  }
}
int maxtwo(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

namespace custom
{
  template <typename T>
  void swap(T &a, T &b)
  {
    T temp = a;
    a = b;
    b = temp;
  }
} // namespace custom

struct Pokemon
{
  int id;
  int damage;
  int health;
  string stance;
  int level;
  Pokemon *next;
  Pokemon *prev;

  Pokemon(int id, int damage, int health, string stance)
      : id(id), damage(damage), health(health), stance(stance), level(1),
        next(nullptr), prev(nullptr) {}
};

class Circle
{
private:
  Pokemon *target;
  int length = 0;
  void removePokemon(Pokemon *&pokemonToRemove)
  {
    if (!pokemonToRemove || length == 1)
    {
      delete pokemonToRemove;
      target = nullptr;
      length = 0;
      return;
    }

    pokemonToRemove->prev->next = pokemonToRemove->next;
    pokemonToRemove->next->prev = pokemonToRemove->prev;

    delete pokemonToRemove;
    length--;
  }

public:
  Circle() : target(nullptr) {}

  void insert(int id, int damage, int health, string stance)
  {
    Pokemon *newPokemon = new Pokemon(id, damage, health, stance);
    if (length == 0)
    {
      target = newPokemon;
      newPokemon->next = newPokemon->prev = newPokemon;
    }
    else
    {
      newPokemon->next = target;
      newPokemon->prev = target->prev;
      target->prev->next = newPokemon;
      target->prev = newPokemon;
    }
    length++;
    if (stance == "Evolve" && length > 2)
    {
      if (newPokemon->prev->id == id && newPokemon->next->id == id &&
          newPokemon->prev->level < 3 && newPokemon->next->level < 3)
      {
        // 使用最大值更新新Pokemon的屬性
        newPokemon->health = maxthree(newPokemon->prev->health, newPokemon->health, newPokemon->next->health);
        newPokemon->damage = maxthree(newPokemon->prev->damage, newPokemon->damage, newPokemon->next->damage);
        newPokemon->level = max(newPokemon->prev->level, newPokemon->next->level) + 1;

        // 移除兩個相鄰的Pokemon
        Pokemon *leftNeighbor = newPokemon->prev;
        Pokemon *rightNeighbor = newPokemon->next;

        removePokemon(leftNeighbor);
        removePokemon(rightNeighbor);

        // 重設current指向新的進化形態
        target = newPokemon;
      }
    }

    else if (stance == "Attack")
    {

      Pokemon *leftNeighbor = newPokemon->prev;
      Pokemon *rightNeighbor = newPokemon->next;
      if (leftNeighbor == newPokemon)
        return;
      bool onlyOneNeighbor = (leftNeighbor == rightNeighbor);

      if (leftNeighbor != newPokemon)
      {
        leftNeighbor->health -= newPokemon->damage;
        if (leftNeighbor->health <= 0)
        {
          removePokemon(leftNeighbor);
          if (leftNeighbor == target)
          {
            target = newPokemon;
          }
        }
      }

      if (!onlyOneNeighbor && rightNeighbor != newPokemon)
      {
        rightNeighbor->health -= newPokemon->damage;
        if (rightNeighbor->health <= 0)
        {
          removePokemon(rightNeighbor);

          if (rightNeighbor == target)
          {
            target = newPokemon;
          }
        }
      }
    }

    newPokemon->stance = "Neutral";
  }

  void deleteCurrent()
  {
    if (length == 0)
    {
      return;
    }
    if (length == 1)
    {
      delete target;
      target = nullptr;
      length = 0;
    }
    else
    {
      Pokemon *toDelete = target;
      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;
      target = toDelete->next;
      delete toDelete;
      length--;
    }
  }

  void shuffle(char dir, int steps)
  {
    if (length <= 1)
      return;
    steps = steps % length;
    Pokemon *current = target;

    for (int i = 0; i < steps; ++i)
    {
      current = (dir == 'c') ? current->next : current->prev;
    }

    custom::swap(target->id, current->id);
    custom::swap(target->damage, current->damage);
    custom::swap(target->health, current->health);
    custom::swap(target->stance, current->stance);
    custom::swap(target->level, current->level);
    target = current;
  }

  void check(int distance)
  {
    if (length <= 1)
      return;
    // 使用一個數組來記錄每個ID出現的次數
    int idCount[101] = {0}; // 假設最大的id是100
    for (int i = 0; i < 101; i++)
      idCount[i] = 0;
    // 首先，遍歷一遍從target開始的distance個寶可夢，計算每個ID出現的次數
    Pokemon *current = target;
    for (int i = 0; i < distance; ++i)
    {
      idCount[current->id]++;
      current = current->next;
    }

    // 再次遍歷，這次移除所有出現超過一次的寶可夢
    current = target;
    for (int i = 0; i < distance; ++i)
    {
      if (length == 0)
        break;
      // 如果當前寶可夢的ID出現超過一次，則將其移除
      if (idCount[current->id] > 1)
      {
        if (length == 1)
        {
          removePokemon(current);
          target = nullptr;
          length = 0;
          return;
        }
        Pokemon *toRemove = current;
        current = current->next;
        if (toRemove == target)
        {
          target = current;
        }
        removePokemon(toRemove);
      }
      else
      {
        current = current->next;
      }
    }
  }

  void reverseGroupData(Pokemon *start, int groupSize)
  {
    int *ids = new int[groupSize];
    int *damages = new int[groupSize];
    int *healths = new int[groupSize];
    string *stances = new string[groupSize];
    int *levels = new int[groupSize];

    Pokemon *current = start;
    for (int i = 0; i < groupSize; ++i)
    {
      ids[i] = current->id;
      damages[i] = current->damage;
      healths[i] = current->health;
      stances[i] = current->stance;
      levels[i] = current->level;
      current = current->next;
    }

    current = start;
    for (int i = groupSize - 1; i >= 0; --i)
    {
      current->id = ids[i];
      current->damage = damages[i];
      current->health = healths[i];
      current->stance = stances[i];
      current->level = levels[i];
      current = current->next;
    }

    delete[] ids;
    delete[] damages;
    delete[] healths;
    delete[] stances;
    delete[] levels;
  }

  void reverse(int groupSize)
  {
    if (length <= 1)
      return;
    if (groupSize >= length)
    {
      reverseGroupData(target, length);
      return;
    }
    Pokemon *current = target;
    int processedLength = 0;
    while (processedLength < length)
    {
      int actualGroupSize = min(groupSize, length - processedLength);
      if (actualGroupSize == 1)
        break;
      reverseGroupData(current, actualGroupSize);

      for (int i = 0; i < actualGroupSize; ++i)
      {
        current = current->next;
      }
      processedLength += actualGroupSize;
    }
  }

  void display()
  {
    if (length == 0)
    {
      cout << "no Poke Poke ;-;" << endl;
      return;
    }
    Pokemon *curr = target;
    int i = 20;
    do
    {
      cout << "ID: " << curr->id << " HP: " << curr->health
           << " LVL: " << curr->level << endl;
      curr = curr->next;
    } while (curr != target);
  }
};

int main()
{
  // cin.tie(0);
  // ios_base::sync_with_stdio(false);
  int n;
  cin >> n;

  Circle circle;
  // circle.insert(1, 100, 100, "Neutral");
  // circle.insert(1, 100, 120, "Neutral");
  // circle.insert(1, 100, 100, "Evolve");
  /*circle.insert(4, 100, 400, "Neutral");
  circle.insert(5, 100, 500, "Neutral");
  circle.insert(6, 100, 606, "Neutral");
  circle.insert(7, 100, 700, "Neutral");
  circle.insert(8, 100, 800, "Neutral");
  circle.insert(9, 100, 900, "Neutral");
  circle.insert(10, 100, 900, "Neutral");*/
  // circle.display();

  // circle.display();
  // circle.shuffle('c',3);

  // circle.reverse(11);
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
      // circle.display();
    }
  }
  circle.display();
  return 0;
}
