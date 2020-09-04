#ifndef HASH_TABLE_H
#define HASH_TABLE_H

template <typename K>
struct Node {
    Node(const K& key) : key(key), next(nullptr) {
    }
    K key;
    Node<K> *next;
};

template <typename K>   //key
class HashTable {
  public:
    /******************************
     ** CONSTRUCTOR & DESTRUCTOR **
     ******************************/
    HashTable(int size, int type = 0) : 
        collisions(0), size(size) {
        this->type = (type >= 0 && type <= 3) ? type : 0;
        table = new Node<K>*[size];
        for(int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for(int i = 0; i < size; i++) {
            deleteList(table[i]);
        }
    }

    /***************
     ** ACCESSORS **
     ***************/
    int getCollisions() const {
        return collisions;
    }

    /***********************************
     ** MODIFICATION MEMBER FUNCTIONS **
     ***********************************/
    void insertKey(const K& key, int value) {
        int index = value % size;
        switch(type) {
            case 0:
                insertLinear(key, index);
                break;
            case 1:
                insertQuadratic(key, index);
                break;
            default:
                insertChaining(key, index);
                break;
        }
    }

    void insertLinear(const K& key, int index) {
        if(table[index] == nullptr) {
            table[index] = new Node<K>(key);
        } else {
            collisions++;
            if(++index >= size) {
                index = 0;
            }
            insertLinear(key, index);
        }
    }

    void insertQuadratic(const K& key, int index, int multiplier = 1) {
        if(table[index] == nullptr) {
            table[index] = new Node<K>(key);
        } else {
            collisions++;
            index += multiplier * multiplier;
            if(index >= size) {
                index %= size;
            }
            insertQuadratic(key, index, ++multiplier);
        }
    }

    void insertChaining(const K& key, int index) {
        Node<K> *insert = new Node<K>(key);
        Node<K> *temp = table[index];

        if(temp == nullptr) {
            table[index] = insert;
        } else {
            collisions++;
            while(temp->next != nullptr) {
                collisions++;
                temp = temp->next;
            }
            temp->next = insert;
        }
    }

    void deleteList(Node<K>*& node) {
        Node<K> *temp;
        while(node != nullptr) {
            temp = node;
            node = node->next;
            delete temp;
        }
    }

  private:
    Node<K> **table;
    int collisions, size, type; //0 - linear, 1 - quadratic, 2 - chaining
};
/******************************************************************************
 *  HashTable Class
 *    A hash table contains an dynamic array of node pointers,
 *      number of collisions, size of the table, and type of insertion
 ******************************************************************************/


/******************************
 ** CONSTRUCTOR & DESTRUCTOR **
 ******************************/

/******************************************************************************
 *  HashTable(int size, int type = 0)
 *    Constructor; Initialize member variables with given arguments,
 *      set default values for table and collisions
 *    Parameters: int, int
 *    Return: void
 ******************************************************************************/

/******************************************************************************
 *  ~HashTable()
 *    Destructor; Free all dynamically allocated memories
 *    Parameters: none
 *    Return: void
 ******************************************************************************/


/***************
 ** ACCESSORS **
 ***************/

/******************************************************************************
 *  int getCollisions() const;
 *
 *    Accessor; This method will return collisions
 * ----------------------------------------------------------------------------
 *    Parameters: none
 * ----------------------------------------------------------------------------
 *    Return: int
 ******************************************************************************/


/***********************************
 ** MODIFICATION MEMBER FUNCTIONS **
 ***********************************/

/******************************************************************************
 *  void insertKey(const K& key, int value);
 *
 *    store the key in the hash table with the given key and hash function val,
 *      storing methods depends on the type.
 *      0 - linear, 1 - quadratic, 2 - chaining
 * ----------------------------------------------------------------------------
 *    Parameters: const K&, int 
 * ----------------------------------------------------------------------------
 *    Return: void
 ******************************************************************************/

/******************************************************************************
 *  void insertLinear(const K& key, int index);
 *
 *    Store the key in the hash table with the given key and index,
 *      use linear probing if collision occurs
 * ----------------------------------------------------------------------------
 *    Parameters: const K&, int
 * ----------------------------------------------------------------------------
 *    Return: void
 ******************************************************************************/

/******************************************************************************
 *  void insertQuadratic(const K& key, int index, int multiplier = 1);
 *
 *    Store the key in the hash table with the given key and index,
 *      use quadratic probing if collision occurs
 * ----------------------------------------------------------------------------
 *    Parameters: 
 * ----------------------------------------------------------------------------
 *    Return: void
 ******************************************************************************/

/******************************************************************************
 *  void insertChaining(const K& key, int index);
 *
 *    Store the key in the hash table with the given key and index,
 *      use chaining probing if collision occurs
 * ----------------------------------------------------------------------------
 *    Parameters: const K&, int
 * ----------------------------------------------------------------------------
 *    Return: void
 ******************************************************************************/

/******************************************************************************
 *  void deleteList(Node<K>*& node);
 *
 *    delete the entire node and its list
 * ----------------------------------------------------------------------------
 *    Parameters: Node<K>*&
 * ----------------------------------------------------------------------------
 *    Return: void
 ******************************************************************************/
#endif // HASH_TABLE_H
