#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    AVLNode<Key, Value>* insertHelper(AVLNode<Key, Value>* parent, const std::pair<const Key, Value>& item);
    void rebalance(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void rebalanceAfterRemove(AVLNode<Key, Value>* node, int diff);

		//
		AVLNode<Key, Value>* avlFind(const Key& key) const {
			return static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
		}
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if(this->root_ == NULL) {
			this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);

			return;
		}

		AVLNode<Key, Value>* inserted = insertHelper(static_cast<AVLNode<Key, Value>*>(this->root_), new_item);

		if(inserted != NULL) {
			AVLNode<Key, Value>* parent = inserted->getParent();

			while(parent != NULL) {
				if(inserted == parent->getLeft()) {
					parent->updateBalance(-1);
				}

				else {
					parent->updateBalance(1);
				}

				int8_t balance = parent->getBalance();

				if(balance == 0) {
					break;
				}

				else if(balance == 2 || balance == -2) {
					rebalance(parent);
					break;
				}

				inserted = parent;
				parent = parent->getParent();
			}
		}
}

/*
 * insertHelper function
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& item) {

    if(item.first< node->getKey()) {
			if(node->getLeft() == NULL) {
				AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(item.first, item.second, node);
				node->setLeft(new_node);

				return new_node;
			}

			else {
				return insertHelper(node->getLeft(), item);
			}
		}

		else if(item.first > node->getKey()) {
			if(node->getRight() == NULL) {
				AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(item.first, item.second, node);
				node->setRight(new_node);

				return new_node;
			}

			else {
				return insertHelper(node->getRight(), item);
			}
		}

		else {
			node->setValue(item.second);

			return NULL;
		}
}

/*
 * rebalance function
*/
template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node) {

    int8_t balance = node->getBalance();

    if(balance < -1) {
        AVLNode<Key, Value>* left = node->getLeft();

        if(left->getBalance() > 0) {
            rotateLeft(left);  
        }
        rotateRight(node);
    }

    else if(balance > 1) {
        AVLNode<Key, Value>* right = node->getRight();

        if(right->getBalance() < 0) {
            rotateRight(right);  
        }
        rotateLeft(node);
    }
}

/* 
 * left rotation
*/
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    
  	AVLNode<Key, Value>* rightChild = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setRight(rightChild->getLeft());

    if(rightChild->getLeft()) {
        rightChild->getLeft()->setParent(node);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);
    rightChild->setParent(parent);

    if(parent == NULL) {
        this->root_ = rightChild;
    } 
    
    else if(parent->getLeft() == node) {
        parent->setLeft(rightChild);
    } 
    
    else {
        parent->setRight(rightChild);
    }

    int8_t b = rightChild->getBalance();

    if(b == 0) {
        node->setBalance(1);
        rightChild->setBalance(-1);
    } 
    
    else {
        node->setBalance(0);
        rightChild->setBalance(0);
    }
}

/*
 * right rotation
*/
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {

    AVLNode<Key, Value>* leftChild = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    node->setLeft(leftChild->getRight());

    if(leftChild->getRight()) {
        leftChild->getRight()->setParent(node);
    }

    leftChild->setRight(node);
    node->setParent(leftChild);
    leftChild->setParent(parent);

    if(parent == NULL) {
        this->root_ = leftChild;
    } 
    
    else if(parent->getLeft() == node) {
        parent->setLeft(leftChild);
    } 
    
    else {
        parent->setRight(leftChild);
    }

    int8_t b = leftChild->getBalance();

    if(b == 0) {
        node->setBalance(-1);
        leftChild->setBalance(1);
    } 
    
    else {
        node->setBalance(0);
        leftChild->setBalance(0);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

    AVLNode<Key, Value>* node = avlFind(key);

    if(node == NULL) {
        return;
    }

    // AVLNode<Key, Value>* toDelete = node;

    if(node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));

        this->nodeSwap(node, pred);
        node = pred;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    AVLNode<Key, Value>* child = node->getLeft() != NULL ? node->getLeft() : node->getRight();

    int diff = 0;

    if(parent != NULL) {
        if(parent->getLeft() == node) {
            diff = 1;
        }

        else {
            diff = -1;
        }
    }

    if(child != NULL) {
        child->setParent(parent);
    }

    if(parent == NULL) {
        this->root_ = child;
    }

    else {
        if(parent->getLeft() == node) {
            parent->setLeft(child);
        }

        else {
            parent->setRight(child);
        }
    }

    delete node;

    rebalanceAfterRemove(parent, diff);
}

/*
 * rebalanceAfterRemove helper function
*/
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterRemove(AVLNode<Key, Value>* node, int diff) {

    while(node != NULL) {
    node->updateBalance(diff);
    int8_t balance = node->getBalance();

    if(balance == -2) {
        AVLNode<Key, Value>* left = node->getLeft();
        int8_t leftBal = left->getBalance();

        if(leftBal <= 0) {
            rotateRight(node);

            if(leftBal == 0) break;
            node = node->getParent();
            diff = 1;
        } 
        
        else {
            rotateLeft(left);
            rotateRight(node);
            node = node->getParent();
            diff = 1;
        }
    }

    else if(balance == 2) {
        AVLNode<Key, Value>* right = node->getRight();
        int8_t rightBal = right->getBalance();

        if(rightBal >= 0) {
            rotateLeft(node);
           
            if(rightBal == 0) break;

            node = node->getParent();
            diff = -1;
        }
        
        else {
            rotateRight(right);
            rotateLeft(node);
            node = node->getParent();
            diff = -1;
        }
    }


    else if(balance == 0) {
        if(node->getParent()) {
            diff = (node->getParent()->getLeft() == node) ? 1 : -1;
        }
        node = node->getParent();
    }

    else {
        break;
    }
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
