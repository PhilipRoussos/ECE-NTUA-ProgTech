#include <iostream>
#include <string>
#include <list>
#include <algorithm>


using namespace std;


class lexicon
{
private:
    class Node
    {
    public:
        string key;
        Node* left=nullptr;
        Node* right=nullptr;
        int freq=1;
    };

    Node* root;
public:

    lexicon():root(nullptr){}

    ~lexicon(){purge(root);}

    void copy (Node *t) {
        if (t != nullptr) {
            insert(t->key);
            copy(t->left);
            copy(t->right);
        }
    }

    void purge (Node *t) {
        if (t != nullptr) {
            copy(t->left);
            copy(t->right);
            delete t;
        }
    }
    Node* createNode(const string &s)
    {
        Node* newNode = new Node();
        newNode->key=s;
        newNode->left=newNode->right=nullptr;
        newNode->freq = 1;
        return newNode;
    }

    void insert(const string &s)
    {
        if(root==nullptr)
        {
            root=createNode(s);
        }else if(insert(root,s)){}
    }
    bool insert(Node* t, const string &s)
    {
        if(s<t->key)
        {
            if(t->left==nullptr)
            {
                t->left=createNode(s);
                return true;
            }else
                return insert(t->left,s);
        }else if(s>t->key)
        {
            if(t->right==nullptr)
            {
                t->right=createNode(s);
                return true;
            }else
                return insert(t->right,s);
        }else
        {
            ++(t->freq);
            return false;
        }
    }

    Node* min(Node* root)
    {
        Node* temp = root;
        while(temp->left!=nullptr)
            temp= temp->left;
        return temp;
    }
    void remove(const std::string & s)
    {
        root=remove(root,s);
    }
    Node* remove(Node* root, const string &s)
    {
        if(root==nullptr)
            return root;
        if(s<root->key)
            root->left=remove(root->left,s);
        else if(s>root->key)
            root->right=remove(root->right,s);
        else
        {
            if(root->left==nullptr)
            {
                Node* temp= root->right;
                delete root;
                return temp;
            }
            if(root->right==nullptr)
            {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            Node* temp = min(root->left);
            root->key=temp->key;
            root->freq=temp->freq;
            root->left=remove(root->left,temp->key);
        }
        return root;
    }

    int lookup(const string &s) const
    {
        return lookup(root,s);
    }

    int lookup(Node *t, const string &s) const {
        if (t == nullptr) {
            return 0;
        }
        else {
            if (s == t->key) {
                return t->freq;
            }
            else if (s < t->key) {
                return lookup(t->left, s);
            }
            else if (s > t->key) {
                return lookup(t->right, s);
            }
        }
    }


    void replace(const string &s1, const string &s2)
    {

        if(lookup(s1)==0)
            return;
        else
        {
            int s1_freq=0;
            Node*node_1=find_word(root, s1);
            s1_freq=node_1->freq;
            remove(s1);
            if(lookup(s2)==0)
            {
                insert(s2);

                find_word(root,s2)->freq=s1_freq;
            }else
                find_word(root,s2)->freq+=s1_freq;
        }

    }

    void print(ostream &out, Node *t) const{
        if (t == nullptr) {
            return;
        }
        print(out, t->left);
        out << t->key << " " << t->freq << endl;
        print(out , t->right);
    }

    friend ostream & operator << (ostream &out, const lexicon &l) {
        l.print(out, l.root);
        return out;
    }

    Node* find_word(Node *t, const string &s) {
        //node *temp1 = nullptr;
        if (t == nullptr || t->key == s) {
            return t;
        }
        else if (s < t->key) {
            //t1 = t;
            return find_word(t->left, s);
        }
        else if (s > t->key ) {
            //t1 = t;
            return find_word(t->right, s);
        }
    }

    int depth(Node *t, const string &s, int i) { ////////////////
        if (t == nullptr) {
            return 0;
        }
        else {
            if (s < t->key) {
                i++;
                return depth(t->left, s, i);
            }
            else if (s > t->key) {
                i++;
                return depth(t->right, s, i);
            }
            else {
                return i;
            }
        }
    }

    int depth(const string &s) {
        int i = 1;
        return depth(root, s, i)-1;
    }


};




int main() {
    lexicon l;
    l.insert("the");
    l.insert("boy");
    l.insert("and");
    l.insert("the");
    l.insert("wolf");
    cout << "The word 'the' is found " << l.lookup("the") << " time(s) "<< endl;
    cout << "The word 'and' is found at depth " << l.depth("and") << endl;
    cout << l;
    cout << "The word 'dummy' is found at depth " << l.depth("") << " " << l.lookup("dummy") << endl;
    l.replace("and", "dummy");
    cout << "The word 'dummy' is found " << l.lookup("dummy") << " time(s) "<< endl;
    l.replace("boy", "wolf");
    cout << "The word 'dummy' is found " << l.lookup("dummy") << " time(s) "<< endl;
    cout << "After replacement:\n";
    cout << l;
    cout << "Now the word 'dummy' is found at depth " << l.depth("dummy") << endl;


}








